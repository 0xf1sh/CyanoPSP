#include <oslib/oslib.h>
#include <pspiofilemgr.h>
#include <string.h>

//declaration
OSL_IMAGE *filemanagerbg, *cursor, *wificon, *navbar;

char fileName;
SceIoDirent dirent;
SceUID dirId = NULL;
int dirStatus = 1;

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

int filemanage()
{
	//loads our images into memory
	filemanagerbg = oslLoadImageFilePNG("system/home/menu/filemanagerbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(0,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
	//Set fonts
	oslSetFont(pgfFont);
	
	//Debugger
	if (!filemanagerbg || !cursor || !wificon || !navbar)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

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
		oslDrawImageXY(navbar, 110, 237);
		oslDrawImageXY(wificon, 387, 1);
		
		oslDrawString(240,136,"Work in Progress");
		
		//calls the functions
		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		oslDrawImage(cursor);

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
			appdrawer();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
	}
}



