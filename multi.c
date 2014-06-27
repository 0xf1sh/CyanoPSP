#include <pspkernel.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *background, *cursor, *multi_task, *wificon;

//definition of our sounds
OSL_SOUND *tone;

int multitask()
{	
	multi_task = oslLoadImageFilePNG("system/home/menu/multi_task.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
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
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(multi_task, 0,10);
		
		oslDrawString(200,136,"No recent apps");

		digitaltime();
		
		//calls the functions
		battery();
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

