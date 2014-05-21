#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "appdrawer.h"
#include "home.h"
#include "calculator.h"
#include "lock.h"

//declaration
OSL_IMAGE *background, *cursor, *calcbackground, *navbar, *wificon, *backdrop;

//variables
int result;
int x = 1;
int y =1;
int enterednumber;
int numberuse = 0;
int suppression = 1;
int Number = 0;

//function declarations
void controls();
void internet();
void android_notif();
void battery();
void powermenu();

//definition of our sounds
OSL_SOUND *tone;
 
int calculator()
{
	SetupCallbacks();
	
	oslIntraFontInit(INTRAFONT_CACHE_MED);
		
	//loads our images into memory
	backdrop = oslLoadImageFile("system/home/icons/backdrop.png", OSL_IN_RAM, OSL_PF_8888);
	calcbackground = oslLoadImageFile("system/app/calculator/calcbg.png", OSL_IN_RAM, OSL_PF_8888);
		
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);

	//Debugger
	if (!calcbackground)
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
		oslDrawImage(backdrop);	
		oslDrawImageXY(calcbackground, 0, 15);		
		
		//Set fonts
		oslSetFont(pgfFont);
		
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		
		oslDrawString(40,75,"sin");
		oslDrawString(94,75,"cos");
		oslDrawString(148,75,"tan");
		oslDrawString(218,80,"7");
		oslDrawString(272,80,"8");
		oslDrawString(326,80,"9");
		oslDrawString(380,80,"/");

		oslDrawString(40,118,"1/x");
		oslDrawString(94,118,"sqr");
		oslDrawString(150,118,"^");
		oslDrawString(218,131,"4");
		oslDrawString(272,131,"5");
		oslDrawString(326,131,"6");
		oslDrawString(380,131,"x");

		oslDrawString(298,238,"");
		oslDrawString(298,238,"");
		oslDrawString(298,238,"");
		oslDrawString(218,182,"1");
		oslDrawString(272,182,"2");
		oslDrawString(326,182,"3");
		oslDrawString(380,182,"-");

		oslDrawString(298,238,"");
		oslDrawString(95,240,"C");
		oslDrawString(298,238,"");
		oslDrawString(246,230,"0");
		oslDrawString(328,230,".");
		oslDrawString(375,235,"+");
		oslDrawString(433,208,"=");

		oslDrawString(35,25,"0");
		battery();
		android_notif();
		powermenu();
		oslDrawImage(cursor);
		
		if (osl_pad.pressed.left)
		x = x - 1;
		if (x == 0)
		x = 7;

		if (osl_pad.pressed.right)
		x = x + 1;
		if (x == 8)
		x = 1;

		if (osl_pad.pressed.up)
		y = y - 1;
		if (y == 0)
		y = 4;

		if (osl_pad.pressed.down)
		y = y + 1;
		if (y == 5)
		y = 1;
		
		if (osl_pad.held.circle)
			appdrawer();
			
		if (osl_pad.pressed.L)
		{
			lockscreen();
        }
		
        oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
		}
	oslQuit();
	return 1;
}

