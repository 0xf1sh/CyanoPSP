#include <pspkernel.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *cursor, *calcbackground, *navbar, *wificon;

//variables
int result;
int x;
int y;
int enterednumber;
int numberuse = 0;
int suppression = 1;
int Number = 0;

//definition of our sounds
OSL_SOUND *tone;

OSL_COLOR black = RGB(0,0,0),red = RGB(255,0,0), white = RGB(255,255,255);

void oslPrintText(int x, int y, char * text, OSL_COLOR color) {
	oslSetTextColor(color);
	oslSetBkColor(RGBA(0,0,0,0));
	oslPrintf_xy(x,y,text);
}
 
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
	if (!calcbackground || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImageXY(calcbackground, 0, 19);		
		
		oslDrawImageXY(navbar, 110, 237);
		oslDrawImageXY(wificon, 387, 1);
		
		oslDrawString(40,77,"sin");
		oslDrawString(94,77,"cos");
		oslDrawString(148,77,"tan");
		oslDrawString(218,82,"7");
		oslDrawString(272,82,"8");
		oslDrawString(326,82,"9");
		oslDrawString(380,82,"/");

		oslDrawString(40,120,"1/x");
		oslDrawString(94,120,"sqr");
		oslDrawString(150,120,"^");
		oslDrawString(218,133,"4");
		oslDrawString(272,133,"5");
		oslDrawString(326,133,"6");
		oslDrawString(380,133,"x");

		oslDrawString(298,240,"");
		oslDrawString(298,240,"");
		oslDrawString(298,240,"");
		oslDrawString(218,184,"1");
		oslDrawString(272,184,"2");
		oslDrawString(326,184,"3");
		oslDrawString(380,184,"-");

		oslDrawString(298,240,"");
		oslDrawString(95,242,"C");
		oslDrawString(298,240,"");
		oslDrawString(246,232,"0");
		oslDrawString(328,232,".");
		oslDrawString(380,237,"+");
		oslDrawString(433,210,"=");

		oslDrawString(35,27,"0");
		
		//calls the functions
		battery();
		back();
		home_icon();
		multi();
		android_notif();
		usb_icon();
		oslDrawImage(cursor);
	
		if (osl_pad.pressed.left)
		x = x - 1;
		if (x == 0)
		x = 8;

		if (osl_pad.pressed.right)
		x = x + 1;
		if (x == 9)
		x = 1;

		if (osl_pad.pressed.up)
		y = y - 1;
		if (y == 0)
		y = 4;

		if (osl_pad.pressed.down)
		y = y + 1;
		if (y == 5)
		y = 1;
		
		x = 1;
		y = 1;
		
		if (y == 1)
		{
		oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
		
			if (x ==1)
			{
			oslDrawString(40,77,"sin");
			}
			else if (x == 2)
			{
				oslDrawString(94,77,"cos");
			}
			else if (x  == 3) 
			{
				oslDrawString(148,77,"tan");
			}
			else if (x == 4)
			{
				oslDrawString(218,82,"7");
			}
			else if (x == 5)
			{
				oslDrawString(272,82,"8");
			}
			else if (x  == 6) 
			{
				oslDrawString(326,82,"9");
			}
			else if (x  == 7)
			{
				oslDrawString(380,82,"/");
			}
			else if (x  == 8)
			{
				oslDrawString(380,82,"");
			}
		}
		
		
		else if (y == 2)
		{
		oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
		
			if (x == 1)
			{
				oslDrawString(40,120,"1/x");
			}
			else if (x == 2) 
			{
				oslDrawString(94,120,"sqr");
			}
			else if (x == 3)
			{
				oslDrawString(150,120,"^");
			}
			else if (x == 4) 
			{
				oslDrawString(218,133,"4");
			}
			else if (x == 5)
			{
				oslDrawString(272,133,"5");
			}
			else if (x  == 6) 
			{
				oslDrawString(326,133,"6");
			}
			else if (x  == 7)
			{
				oslDrawString(380,133,"x");
			}
			else if (x  == 8)
			{
				oslDrawString(380,82,"");
			}
		}
		
		else if (y == 3) 
		{
		oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
		
			if (x == 1)
			{
				oslDrawString(298,240,"");
			}
			else if (x == 2)
			{
				oslDrawString(298,240,"");
			}
			else if (x == 3)
			{
				oslDrawString(298,240,"");
			}
			else if (x == 4)
			{
				oslDrawString(218,184,"1");
			}
			else if (x == 5)
			{
				oslDrawString(272,184,"2");
			}
			else if (x  == 6)
			{
				oslDrawString(326,184,"3");
			}
			else if (x  == 7)
			{
				oslDrawString(380,184,"-");
			}
			else if (x  == 8)
			{
				oslDrawString(380,82,"");
			}
		}
		
		else if (y == 4)
		{
		oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
		
			if (x == 1)
			{
				oslDrawString(298,240,"");
			}
			else if (x == 2)
			{
				oslDrawString(95,242,"C");
			}
			else if (x == 3)
			{
				oslDrawString(298,240,"");
			}
			else if (x == 4)
			{
				oslDrawString(246,232,"0");
			}
			else if (x == 5)
			{
				oslDrawString(328,232,".");
			}
			else if (x  == 6) 
			{
				oslDrawString(380,237,"+");
			}
			else if (x  == 7)
			{
				oslDrawString(433,210,"=");
			}
			else if (x  == 8)
			{
				oslDrawString(380,82,"");
			}
		}

		if (osl_pad.held.square)
		{
			powermenu();
		}
		
		if (osl_pad.held.circle)
		{
			appdrawer();
		}
		
		if (osl_pad.held.L)
		{
			lockscreen();
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

