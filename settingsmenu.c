#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "appdrawer.h"
#include "home.h"
#include "calculator.h"
#include "lock.h"
#include "settingsmenu.h"

//declaration
OSL_IMAGE *settings_bg, *backdrop, *cursor, *navbar, *wificon;

//function declarations
void controls();
void android_notif();
void battery();
void powermenu();

//definition of our sounds
OSL_SOUND *tone;
 
int settingsmenu()
{
	SetupCallbacks();
	
	oslIntraFontInit(INTRAFONT_CACHE_MED);
		
	//loads our images into memory
	settings_bg = oslLoadImageFilePNG("system/home/menu/settings_bg.png", OSL_IN_RAM, OSL_PF_8888);
	backdrop = oslLoadImageFile("system/home/icons/backdrop.png", OSL_IN_RAM, OSL_PF_8888);
		
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);

	//Debugger
	if (!settings_bg || !cursor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	//Main loop to run the program
	while (!osl_quit)
	{
		//Draws images onto the screen
		oslStartDrawing();
				
		//calls the functions
		controls();	
		android_notif();
			
		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImageXY(backdrop, 0, 0 );
		oslDrawImageXY(settings_bg, 0, 17);
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		
		//Set fonts
		oslSetFont(pgfFont);
		
		oslDrawString(37,73,"Model - PSP GO! N1004");
		oslDrawString(37,134,"Android Version 4.4.2 (Unofficial)");
		oslDrawString(37,170,"Kernel Version");
		oslDrawString(37,183,"Undefined-pspsdk_oslib");
		oslDrawString(37,196,"joelluvsanna@psp #1");
		battery();
		android_notif();
		powermenu();
		oslDrawImage(cursor);
		
		if (osl_pad.pressed.L)
		{
			lockscreen();
        }
		
		if (osl_pad.held.circle)
			appdrawer();

        oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
		}
	oslQuit();
	return 1;
}
