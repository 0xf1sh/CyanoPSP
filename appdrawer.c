#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "appdrawer.h"


int app_drawer_open(int value)
{

    return value + 3;
};

//declaration
OSL_IMAGE *background, *cursor, *appicon, *appicon2, *navbar, *wificon, *apollo, *gmail, *message, *browser, *google;

//variables
int cursor_position;
int app_drawer;

//function declarations

//definition of our sounds
OSL_SOUND *tone;
 
int appdrawer()
{
	//Initialization of the Oslib's library
	oslInit(0);

	//Initialization of Oslib's graphics mode
	oslInitGfx(OSL_PF_8888, 1);
	
	//Sets the transparency color (blue)
	oslSetTransparentColor(RGB(0,0,255));
	
	//Initialization of Oslib's text console
    oslInitConsole();

    //Initialization of Oslib's audio console
    oslInitAudio();
	
	//loads our sound
	tone = oslLoadSoundFile("System/Media/Audio/Ui/KeypressStandard.wav", OSL_FMT_NONE);

	//loads our images into memory
	background = oslLoadImageFilePNG("System/Home/Wallpapers/1.png", OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("System/Cursor/cursor.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("System/Home/Icons/nav.png", OSL_IN_RAM, OSL_PF_8888);
	wificon = oslLoadImageFile("System/Home/Icons/wificon.png", OSL_IN_RAM, OSL_PF_5551);
	apollo = oslLoadImageFilePNG("System/Home/Icons/apollo.png", OSL_IN_RAM, OSL_PF_5551);
	gmail = oslLoadImageFilePNG("System/Home/Icons/gmail.png", OSL_IN_RAM, OSL_PF_5551);
	message = oslLoadImageFilePNG("System/Home/Icons/message.png", OSL_IN_RAM, OSL_PF_5551);
	browser = oslLoadImageFile("System/Home/Icons/browser.png", OSL_IN_RAM, OSL_PF_8888);
	google = oslLoadImageFile("System/Home/Icons/google.png", OSL_IN_RAM, OSL_PF_5551);
	
	//Disables the transpaent color (blue)
	oslDisableTransparentColor();
	
	//Debugger
	if (!background || !cursor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	app_drawer = 1;
	
	//Main loop to run the program
	while (!osl_quit)
	{
		//Draws images onto the screen
		oslStartDrawing();
		
		//calls the controls() function
		controls();	
		
		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImage(background);
		
		if (cursor->x  >= 215 && cursor->x  <= 243 && cursor->y >= 195 && cursor->y <= 230)
		oslDrawImageXY(appicon2,223,205);
	
		else
		oslDrawImageXY(appicon,223,205);
		
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(google, 22, 26);
		oslDrawImageXY(apollo, 105, 195);
		oslDrawImageXY(browser, 276, 195);
		oslDrawImageXY(gmail, 331, 195);
		oslDrawImageXY(message, 160, 195);
		oslDrawImage(cursor);
	

				
		//Ends printing
		oslEndDrawing();

		//Synchronizes the screen 
		oslSyncFrame();	

	    //For sleep
        oslAudioVSync();
	}

	return 1;
}

