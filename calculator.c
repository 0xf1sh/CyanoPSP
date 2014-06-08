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
OSL_IMAGE *cursor, *calcbackground, *navbar, *wificon;

//variables
int result;
int x = 1;
int y =1;
int enterednumber;
int numberuse = 0;
int suppression = 1;
int Number = 0;

//definition of our sounds
OSL_SOUND *tone;
 
int calculator()
{
	//loads our images into memory
	calcbackground = oslLoadImageFile("system/app/calculator/calcbg.png", OSL_IN_RAM, OSL_PF_8888);
		
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
	//Set fonts
	oslSetFont(pgfFont);

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
		
		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImageXY(calcbackground, 0, 15);		
		
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
		
		//calls the functions
		controls();	
		battery();
		powermenu();
		back();
		home_icon();
		multi();
		android_notif();
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
		
		oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
		
		if (y == 1 && x == 1)
		{
		oslDrawString(40,75,"sin");
		}
		else if (x == 2)
		{
		oslDrawString(94,75,"cos");
		}
		else if (x  == 3) 
		{
		oslDrawString(148,75,"tan");
		}
		else if (x == 4)
		{
		oslDrawString(218,80,"7");
		}
		else if (x == 5)
		{
		oslDrawString(272,80,"8");
		}
		else if (x  == 6) 
		{
		oslDrawString(326,80,"9");
		}
		else if (x  == 7)
		{
		oslDrawString(380,80,"/");
		}

		else if (y == 2)
		{
		if (x == 1)
		oslDrawString(40,118,"1/x");
		}
		else if (x == 2) 
		{
		oslDrawString(94,118,"sqr");
		}
		else if (x == 3)
		{
		oslDrawString(150,118,"^");
		}
		else if (x == 4) 
		{
		oslDrawString(218,131,"4");
		}
		else if (x == 5)
		{
		oslDrawString(272,131,"5");
		}
		else if (x  == 6) 
		{
		oslDrawString(326,131,"6");
		}
		else if (x  == 7)
		{
		oslDrawString(380,131,"x");
		}
		
		else if (y == 3) 
		{
		if (x == 1)
		oslDrawString(298,238,"");
		}
		else if (x == 2)
		{
		oslDrawString(298,238,"");
		}
		else if (x == 3)
		{
		oslDrawString(298,238,"");
		}
		else if (x == 4)
		{
		oslDrawString(218,182,"1");
		}
		else if (x == 5)
		{
		oslDrawString(272,182,"2");
		}
		else if (x  == 6)
		{
		oslDrawString(326,182,"3");
		}
		else if (x  == 7)
		{
		oslDrawString(380,182,"-");
		}
		
		else if (y == 4)
		{
		if (x == 1)
		oslDrawString(298,238,"");
		}
		else if (x == 2)
		{
		oslDrawString(95,240,"C");
		}
		else if (x == 3)
		{
		oslDrawString(298,238,"");
		}
		else if (x == 4)
		{
		oslDrawString(246,230,"0");
		}
		else if (x == 5)
		{
		oslDrawString(328,230,".");
		}
		else if (x  == 6) 
		{
		oslDrawString(375,235,"+");
		}
		else if (x  == 7)
		{
		oslDrawString(433,208,"=");
		}
		
		if (osl_pad.held.circle)
		{
			appdrawer();
		}
		
		if (osl_pad.held.L)
		{
			lockscreen();
        }
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			appdrawer();
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

