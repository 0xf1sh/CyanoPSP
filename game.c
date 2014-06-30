#include <pspkernel.h>
#include <oslib/oslib.h>
#include "game.h"

//declaration
OSL_IMAGE *gamebg, *cursor, *icon, *noicon;

//definition of our sounds
OSL_SOUND *tone;

int xstrtoi(char * str, int len) 
{
	int val;
	int c;
	int i;
	val = 0;
	for (i = 0; i < len; i++){
		c = *(str + i);
		if (c >= '0' && c <= '9') {
			c -= '0';
		} else if (c >= 'A' && c <= 'F') {
			c = (c - 'A') + 10;
		} else if (c >= 'a' && c <= 'f') {
			c = (c - 'a') + 10;
		} else {
			return 0;
		}
		val *= 16;
		val += c;
	}
	return val;
};

#ifdef CEF

int cef_launch(char * file, int type, int extra)
{
	int runlevel = 0;	

	//create loadexec struct
	SceKernelLoadExecVSHParam param;
	memset(&param, 0, sizeof(param));
	param.size = sizeof(param);

	char disc[34];
	char * exec = file;

	strcpy(disc, "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN");

	if(type == PBP) //pbp files
	{
		switch(extra)
		{
			case entry138::APP:
				runlevel = HOMEBREW_RUNLEVEL;
				param.args = strlen(file) + 1;
				param.argp = file;
				param.key = "game";

				break;

			case entry138::PS1:
				runlevel = POPS_RUNLEVEL;
				param.args = strlen(file) + 1;
				param.argp = file;
				param.key = "pops";

				break;

			case entry138::PSN:
				runlevel = PSN_RUNLEVEL;
				param.args = strlen(disc) + 1;
				param.argp = disc;
				param.key = "umdemu";
				
#ifdef ARK
				sctrlSESetBootConfFileIndex(MODE_NP9660);
				sctrlSESetUmdFile("");
#endif

#ifdef TN
				sctrlSEMountUmdFromFile(file, MODE_NP9660);
#endif
				break;
		};
	}
	else //backups
	{
		if(extra == entry138::PATCHED)
			strcpy(disc, "disc0:/PSP_GAME/SYSDIR/EBOOT.OLD");

		runlevel = ISO_RUNLEVEL;
		param.args = strlen(disc) + 1;
		param.argp = disc;	

#ifdef ARK
		param.key = "umdemu";
		sctrlSESetBootConfFileIndex(MODE_INFERNO);
		sctrlSESetUmdFile(file);
#endif

#ifdef TN

		param.key = "game";
		sctrlSEMountUmdFromFile(file, MODE_MARCH33);
		exec = disc;
#endif		

	};

	//start app
	return sctrlKernelLoadExecVSHWithApitype(runlevel, exec, &param);
};
#endif

int gamemenu()
{	
	gamebg = oslLoadImageFilePNG("system/home/menu/gamebg.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.6, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
	//Set fonts
	oslSetFont(pgfFont);
	
	//Debugger
	if (!gamebg || !cursor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	//Main loop to run the program
	while (!osl_quit)
	{
		//Draws images onto the screen
		oslStartDrawing();
		
		controls();
											
		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImageXY(gamebg, 0,0);
		
		oslDrawString(240,136,"Work in Progress");
		
		//calls the functions
		navbar_buttons();
		android_notif();
		usb_icon();
		oslDrawImage(cursor);
		
		if (osl_pad.held.L)
		{
			lockscreen();
        }
		
		if (osl_pad.held.circle)
		{
			home();
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
				
		//Ends printing
		oslEndDrawing();
		
		//Synchronizes the screen 
		oslSyncFrame();	
					   
	    //For sleep
        oslAudioVSync();
	}
	
}

