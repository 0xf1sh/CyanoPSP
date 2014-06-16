#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *background, *cursor, *multi_task, *wificon, *navbar;

//definition of our sounds
OSL_SOUND *tone;

int multitask()
{	
	multi_task = oslLoadImageFilePNG("system/home/menu/multi_task.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.6, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
	//Set fonts
	oslSetFont(pgfFont);
	
	//Debugger
	if (!background || !cursor|| !multi_task)
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
		oslDrawImageXY(background, 0,0);
		oslDrawImageXY(navbar, 110, 237);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(multi_task, 0,0);
		
		oslDrawString(240,136,"No recent apps");
		
		//calls the functions
		battery();
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
				
		//Ends printing
		oslEndDrawing();
		
		//Synchronizes the screen 
		oslSyncFrame();	
					   
	    //For sleep
        oslAudioVSync();
	}
	
}

