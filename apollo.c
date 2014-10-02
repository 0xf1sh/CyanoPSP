#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psppower.h>
#include <oslib/oslib.h>
#include "mp3player.h"
#include "fm.h"
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "include/screenshot.h"

#define MAX_MP3_DISPLAY			3 // max amount of files displayed on-screen.
#define MP3_DISPLAY_X			20 // X value of where the filebrowser is displayed.
#define MP3_DISPLAY_Y			108 // Y value of the filebrowser is displayed.
#define MP3_CURR_DISPLAY_Y     	90 
#define MP3DISPLAY_X        20

OSL_IMAGE *mp3bg, *cursor, *wificon, *nowplaying, *mp3_select;
OSL_FONT *pgfFont;

void mp3Up();
void mp3Down();
void mp3Controls();
char * mp3Browse(const char * path);

void mp3Up()
{
	current--; // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll--; // To do with how it scrolls
	}
}

void mp3Down()
{
	if (folderIcons[current+1].active) current++; // Add a value onto current so the ">" goes down
	if (current >= (MAX_MP3_DISPLAY+curScroll)) {
		curScroll++; // To do with how it scrolls
	}
}

void mp3FileDisplay()
{	
	oslDrawImageXY(mp3bg, 0, 19);
	oslDrawImageXY(mp3_select,8,(current - curScroll)*55+MP3_CURR_DISPLAY_Y);

	battery();
	digitaltime(420,4,458);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_MP3_DISPLAY+curScroll;i++) {
	
		char * ext = strrchr(dirScan[i].name, '.'); //For file extension.
	
		// Handles the cursor and the display to not move past the MAX_MP3_DISPLAY.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) {
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) {
			current = curScroll-1;
			break;
		}
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) {
			
			oslDrawStringf(MP3_DISPLAY_X, (i - curScroll)*55+MP3_DISPLAY_Y, folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

void mp3Controls() //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) {
	
		if ((pad.Buttons & PSP_CTRL_DOWN) && (!(oldpad.Buttons & PSP_CTRL_DOWN))) {
			mp3Down();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_UP) && (!(oldpad.Buttons & PSP_CTRL_UP))) {
			mp3Up();
			timer = 0;
		}
		if ((pad.Buttons & PSP_CTRL_CROSS) && (!(oldpad.Buttons & PSP_CTRL_CROSS))) {
			runFile(folderIcons[current].filePath, folderIcons[current].fileType);
		}
		if ((pad.Buttons & PSP_CTRL_TRIANGLE) && (!(oldpad.Buttons & PSP_CTRL_TRIANGLE))) {
			if (!(stricmp(lastDir, "ms0:")==0) || (stricmp(lastDir, "ms0:/")==0)) {
				curScroll = 1;
				current = 1;
			}
		}
		if ((pad.Buttons & PSP_CTRL_CIRCLE) && (!(oldpad.Buttons & PSP_CTRL_CIRCLE))) {
		if(!strcmp("ms0:/", lastDir)) //pressed circle in root folder
			{
			dirBack();
			}
		}
	}
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || ((strcmp(ext ,".MP3") == 0))) && (osl_keys->pressed.cross))
	{
		MP3Play(folderIcons[current].filePath);
	}

	if (osl_pad.held.circle)
	{
		appdrawer();
	}
	
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
		
	if (osl_pad.held.R && osl_pad.held.triangle)
	{
		screenshot();
	}
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP)) {
		mp3Up();
		timer = 25;
	} else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN)) {
		mp3Down();
		timer = 25;
	}

	if (current < 1) current = 1; // Stop the ">" from moving past the minimum files
	if (current > MAX_FILES) current = MAX_FILES; // Stop the ">" from moving past the max files

}

char * mp3Browse(const char * path)
{
	folderScan(path);
	dirVars();

	
	while (!osl_quit)
	{		
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));	
		
		if (!strcmp(".", g_dir.d_name) || !strcmp("..", g_dir.d_name)) 
			{
				memset(&g_dir, 0, sizeof(SceIoDirent));
				continue;
			};
		
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		mp3FileDisplay();
		mp3Controls();
		
		sceDisplayWaitVblankStart();
		
		if (strlen(returnMe) > 4) {
			break;
		}
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
		
	return returnMe;
}

int mp3player()
{	

	nowplaying = oslLoadImageFilePNG("system/app/apollo/nowplaying.png", OSL_IN_RAM, OSL_PF_8888);
	mp3bg = oslLoadImageFilePNG("system/app/apollo/mp3bg.png", OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG("system/app/apollo/mp3_select.png", OSL_IN_RAM, OSL_PF_8888);

	if (!mp3bg || !cursor || !wificon || !nowplaying)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(pgfFont);
		
	char * Directory = mp3Browse("ms0:/");

	while (!osl_quit)
	{	
		oslReadKeys();
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));	
		
		centerText(480/2, 272/2, Directory, 50);

		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
		}
}


