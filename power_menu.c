#include <pspkernel.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *background, *cursor, *wificon, *power, *apollo, *gmail, *message, *browser, *google, *pointer;

void power_off()
{
	scePowerRequestStandby();
}

//definition of our sounds
OSL_SOUND *tone;

int screenshot = 0;

void screencapture()
{	
	if (screenshot == 1)
	{
		oslWriteImageFilePNG(oslGetDrawBuffer(), "ms0:/PSP/GAME/CyanogenMod/screenshot/screenshot.PNG", 0);
	}
}

int powermenu()
{	
	power = oslLoadImageFile("system/home/menu/power.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Debugger
	if (!background || !cursor || !wificon || !power)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	//Main loop to run the program
	while (!osl_quit)
	{
		 __psp_free_heap();
		 
		//Draws images onto the screen
		oslStartDrawing();
		
		//calls the functions
		controls();	
											
		//Initiate the PSP's controls
		oslReadKeys();
			
		//Print the images onto the screen
		oslDrawImage(background);
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(google, 22, 26);
		oslDrawImageXY(apollo, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(message, 160, 190);
		oslDrawImageXY(pointer, 231, 180);
		oslDrawImageXY(power, 114, 10);	
		
		digitaltime();

		battery();
		usb_icon();
		oslDrawImage(cursor);
		
		screencapture();
		
		if (osl_pad.held.circle)
		{
			return;
		}
			
		if (cursor->x >= 128 && cursor->x <= 354 && cursor->y >= 25 && cursor->y <= 86 && osl_pad.held.cross)
		{
			sceKernelExitGame();
		}
		
		if (cursor->x >= 128 && cursor->x <= 354 && cursor->y >= 88 && cursor->y <= 140 && osl_pad.held.cross)
		{
			recoverymain();
		}
		
		screencapture();
		
		if (cursor->x >= 128 && cursor->x <= 354 && cursor->y >= 192 && cursor->y <= 242 && osl_pad.held.cross)
		{	
			return;
			screenshot = 1;
		}
				
		//Ends printing
		oslEndDrawing();

		//Synchronizes the screen 
		oslSyncFrame();	
					   
	    //For sleep
        oslAudioVSync();
	}
}

