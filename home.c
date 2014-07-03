#include <pspkernel.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *background, *cursor, *appicon, *appicon2, *wificon, *apollo, *gmail, *message, *browser, *google, *notif, *batt100, *batt80, *batt60, *batt40, *batt20, *batt10, *batt0, *battcharge, *pointer, *pointer1, *backicon, *homeicon, *multicon;

//definition of our sounds
OSL_SOUND *tone;

int home()
{	
	loadicons();
	
	//Debugger
	if (!background || !cursor || !appicon || !appicon2 || !wificon || !apollo || !gmail || !message || !browser || !google || !notif || !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !pointer || !pointer1 || !backicon || !multicon || !homeicon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	setfont();

	//Main loop to run the program
	while (!osl_quit)
	{
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
		
		digitaltime();
		appdrawericon();
		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		oslDrawImage(cursor);
			
		if (osl_pad.held.square)
		{
			powermenu();
		}
		
		//Launching the browser
		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_pad.held.cross)
		{
			unloadicons();
			internet();
		}
			
		if (cursor->x >= 215 && cursor->x <= 243 && cursor->y >= 195 && cursor->y <= 230 && osl_pad.held.cross)
		{
			unloadicons();
			appdrawer();
		}
		
		if (osl_pad.held.L)
		{
			lockscreen();
        }
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			unloadicons();
			multitask();
		}
		
		if (osl_pad.held.R && osl_pad.held.cross)
		{
			unloadicons();
			recoverymain();
        }
		
			
		//Ends printing
		oslEndDrawing();

		//Synchronizes the screen 
		oslSyncFrame();	
					   
	    //For sleep
        oslAudioVSync();
	}
}

