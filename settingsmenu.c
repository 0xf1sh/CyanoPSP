#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "settingsmenu.h"

//declaration
OSL_IMAGE *settings_bg, *cursor, *navbar, *wificon, *backdrop;

//definition of our sounds
OSL_SOUND *tone;

char pspmodel;

void pspgetmodel()
{
		pspmodel = kuKernelGetModel();
	
		if(pspmodel == 0)
		{
			oslDrawString(37,73,"Model: PSP 1000");
		}
   
		else if(pspmodel == 1)
		{
			oslDrawString(37,73,"Model: PSP 2000");
		}
   
		else if (pspmodel == 2)
		{
			oslDrawString(37,73,"Model: PSP 3000");
		}
   
		else if (pspmodel == 4)
		{
			oslDrawString(37,73,"Model: PSP Go N1000");
		}
   
		else
		{
			oslDrawString(37,73,"Model: PS Vita");
		}
}

int settingsmenu()
{
	//loads our images into memory
	settings_bg = oslLoadImageFilePNG("system/home/menu/settings_bg.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	//Set fonts
	oslSetFont(pgfFont);
	
	//Debugger
	if (!settings_bg || !cursor || !backdrop || !wificon)
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
		oslDrawImageXY(backdrop, 0, 0);
		oslDrawImageXY(settings_bg, 0, 17);
		oslDrawImageXY(navbar, 110, 237);
		oslDrawImageXY(wificon, 387, 1);
		
		pspgetmodel();
		oslDrawString(37,121,"CyanogenMod PSP - C Alpha build 1");
		oslDrawString(37,170,"Kernel Version");
		oslDrawString(37,183,"Undefined-pspsdk_oslib");
		oslDrawString(37,196,"joelluvsanna@psp #1");
			
		//calls the functions
		battery();
		back();
		home_icon();
		multi();
		android_notif();
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

