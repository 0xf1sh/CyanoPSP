#include <oslib/oslib.h>

//declaration
OSL_IMAGE *filemanagerbg, *cursor, *wificon, *navbar;

int filemanage()
{
	//loads our images into memory
	filemanagerbg = oslLoadImageFilePNG("system/home/menu/filemanagerbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
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
		
		//calls the functions
		battery();
		back();
		home_icon();
		multi();
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



