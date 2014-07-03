#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <oslib/oslib.h>
#include <pspiofilemgr.h>
#include <pspiofilemgr_kernel.h>
#include <pspiofilemgr_dirent.h>
#include <stdio.h>
#include <string.h>
#include "fm.h"

//declaration
OSL_IMAGE *filemanagerbg, *cursor, *wificon, *diricon, *imageicon, *mp3icon, *txticon, *unknownicon;

//Load fonts:
OSL_FONT *pgfFont;

char fileName;
SceIoDirent dirent;
SceUID dirId = 0;
int dirStatus = 1;
int curr;
int amount;

void filemanager_unload()
{
	oslDeleteImage(filemanagerbg);
	oslDeleteImage(diricon);
	oslDeleteImage(imageicon);
	oslDeleteImage(mp3icon);
	oslDeleteImage(txticon);
	oslDeleteImage(unknownicon);
}

int fileExists(const char* path)
{
	SceUID dir = sceIoOpen(path, PSP_O_RDONLY, 0777);
	if (dir >= 0){
		sceIoClose(dir);
		return 1;
	}
	else {
		return 0;
	}
}

int dirExists(const char* path)
{
	SceUID dir = sceIoDopen(path);
	if (dir >= 0){
		sceIoDclose(dir);
		return 1;
	}
	else {
		return 0;
	}
}

unsigned fileSize(const char * path)
{
	//try to open file
	SceUID dir = sceIoOpen(path, PSP_O_RDONLY, 0777);

	if(dir < 0) //failed
		return 0;

	//get file size
	unsigned size = sceIoLseek(dir, 0, SEEK_END); 

	//close file
	sceIoClose(dir);

	return size;
};

int createFolder(const char * path)
{
	return !sceIoMkdir(path, 0);
};

int changeDir(const char* path)
{
	return sceIoChdir(path);
}


int openDir(const char* path)
{
	dirStatus = 1;
	dirId = sceIoDopen(path);
	sceIoDread(dirId, &dirent);
	sceIoDread(dirId, &dirent);
	return dirId;
}

int closeDir()
{
	if (dirId != NULL)
	{
		sceIoDclose(dirId);
		dirId = NULL;
		return 1;
	}
	return 0;
}

int nextFile()
{
	if (dirId > NULL && dirStatus > 0)
	{
		dirStatus = sceIoDread(dirId, &dirent);
	}
	return dirStatus;
}

char* getNextfileName()
{	
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	if (dirId > NULL && dirStatus > 0)
	{	
		if(dirStatus >= 0)strcpy(fileName, dirent.d_name);
	}
	return fileName;
}

int nextDir()
{
	return FIO_S_ISDIR(dirent.d_stat.st_mode);
}

// Thanks to Omega2058, for helping me out with this.

int listFiles(void) {

	int dfd, result = 0, y = 23, iconY = 18, sCurr = 0;;
	
	// Clear out "dir" by setting all it's members to 0 
	memset(&dirent, 0, sizeof(dirent));
	
	// Open up the directory
	dfd = sceIoDopen(rootdir);
	
	// Make sure that the directory was able to open
	if(dfd < 0) {
		return dfd;
	}

	// This portion will continue to read contents in 
	// the directory and print it one-by-one :)
	while (sceIoDread(dfd, &dirent) > 0) {	
		//Confirm that the file is an actual name, blah, blah
		if (strcmp(dirent.d_name, ".") == 0)
			continue;
		if (strcmp(dirent.d_name, ".") == 0)
			continue;
			
		if (curr == sCurr) {
			oslIntraFontSetStyle(pgfFont, 0.5, RGBA(41,118,195,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(80, y+=46, "%s", dirent.d_name);
			goto end;
		}

		// Print the file inside the directory
		oslIntraFontSetStyle(pgfFont, 0.5, RGBA(0,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
		oslDrawStringf(80, y+=46, "%s", dirent.d_name);
		oslDrawImageXY(diricon, 36, 56);

		end:
		sCurr++;
		result++;
	}

	// Close and return, we're done :D
	sceIoDclose(dfd);
	return result;

}

int filemanage(int argc, char *argv[])
{
	//loads our images into memory
	filemanagerbg = oslLoadImageFilePNG("system/app/filemanager/filemanagerbg.png", OSL_IN_RAM, OSL_PF_8888);
	diricon = oslLoadImageFilePNG("system/app/filemanager/dir.png", OSL_IN_RAM, OSL_PF_8888);
	imageicon = oslLoadImageFilePNG("system/app/filemanager/image.png", OSL_IN_RAM, OSL_PF_8888);
	mp3icon = oslLoadImageFilePNG("system/app/filemanager/mp3.png", OSL_IN_RAM, OSL_PF_8888);
	txticon = oslLoadImageFilePNG("system/app/filemanager/txt.png", OSL_IN_RAM, OSL_PF_8888);
	unknownicon= oslLoadImageFilePNG("system/app/filemanager/unknownfile.png", OSL_IN_RAM, OSL_PF_8888);
	
	pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(0,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	//Set fonts
	oslSetFont(pgfFont);
	
	//Debugger
	if (!filemanagerbg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	SceCtrlData pad;
	
	curr = 0;

	// Get the number of files in the directory
	amount = listFiles();
	
	while (!osl_quit)
  {
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(filemanagerbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		listFiles();
		
		//calls the functions
		battery();
		usb_icon();
		
		sceCtrlReadBufferPositive(&pad, 1);
		
		if (pad.Buttons & PSP_CTRL_DOWN) {
			pspDebugScreenClear();
			curr++;
			while(pad.Buttons)(sceCtrlReadBufferPositive(&pad, 1));
		}
		if (pad.Buttons & PSP_CTRL_UP) {
			pspDebugScreenClear();
			curr--;
			while(pad.Buttons)(sceCtrlReadBufferPositive(&pad, 1));
		}
		
		if(curr > amount)
			curr = 0;
		
		if(curr < 0)
			curr = amount;
        
		if (osl_pad.held.square)
		{
			powermenu();
		}
		
		if (osl_pad.held.L)
		{
			lockscreen();
        }
		
		if (osl_pad.held.circle)
		{	
			filemanager_unload();
			appdrawer();
		}
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
	}
}



