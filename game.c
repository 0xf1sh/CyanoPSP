#include <pspkernel.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *gamebg, *cursor, *navbar;

//definition of our sounds
OSL_SOUND *tone;

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
		oslDrawImageXY(navbar, 110, 237);
		
		oslDrawString(240,136,"Work in Progress");
		
		//calls the functions
		back();
		home_icon();
		multi();
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
			home();
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
