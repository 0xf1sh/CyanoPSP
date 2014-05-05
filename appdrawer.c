#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "appdrawer.h"

OSL_COLOR black = RGB(0,0,0),red = RGB(255,0,0), white = RGB(255,255,255);

int app_drawer_open(int value)
{

    return value + 3;
};

//declaration
OSL_IMAGE *background, *cursor, *appicon, *appicon2, *navbar, *wificon, *apollo, *gmail, *message, *browser, *google, *notif, *calc, *clockx, *email, *people, *calendar, *phone, *gallery, *dsp, *settings;

//variables
int cursor_position;
int app_drawer;

//function declarations
void controls();
void internet();
void android_notif();
void battery();

//definition of our sounds
OSL_SOUND *tone;
 
int appdrawer()
{

	//loads our images into memory
	background = oslLoadImageFilePNG("System/Home/Wallpapers/1.png", OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("System/Cursor/cursor.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("System/Home/Icons/nav.png", OSL_IN_RAM, OSL_PF_8888);
	wificon = oslLoadImageFile("System/Home/Icons/wificon.png", OSL_IN_RAM, OSL_PF_8888);
	apollo = oslLoadImageFilePNG("System/Home/Icons/apollo.png", OSL_IN_RAM, OSL_PF_8888);
	gmail = oslLoadImageFilePNG("System/Home/Icons/gmail.png", OSL_IN_RAM, OSL_PF_8888);
	message = oslLoadImageFilePNG("System/Home/Icons/message.png", OSL_IN_RAM, OSL_PF_8888);
	browser = oslLoadImageFile("System/Home/Icons/browser.png", OSL_IN_RAM, OSL_PF_8888);
	notif = oslLoadImageFile("System/Home/Menu/notif.png", OSL_IN_RAM, OSL_PF_8888);
	clockx = oslLoadImageFilePNG("System/Home/Icons/clock.png", OSL_IN_RAM, OSL_PF_8888);
	email = oslLoadImageFilePNG("System/Home/Icons/email.png", OSL_IN_RAM, OSL_PF_8888);
	settings = oslLoadImageFilePNG("System/Home/Icons/settings.png", OSL_IN_RAM, OSL_PF_8888);
	dsp = oslLoadImageFilePNG("System/Home/Icons/dsp.png", OSL_IN_RAM, OSL_PF_8888);
	gallery = oslLoadImageFilePNG("System/Home/Icons/gallery.png", OSL_IN_RAM, OSL_PF_8888);
	phone = oslLoadImageFilePNG("System/Home/Icons/phone.png", OSL_IN_RAM, OSL_PF_8888);
	calc = oslLoadImageFilePNG("System/Home/Icons/calc.png", OSL_IN_RAM, OSL_PF_8888);
	calendar = oslLoadImageFilePNG("System/Home/Icons/calendar.png", OSL_IN_RAM, OSL_PF_8888);
	people = oslLoadImageFilePNG("System/Home/Icons/people.png", OSL_IN_RAM, OSL_PF_8888);
	
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
		
		//calls the functions
		controls();	
		android_notif();
		
		//Launching the browser
		if (cursor->x >= 105 && cursor->x <= 150 && cursor->y >= 35 && cursor->y <= 80 && osl_pad.held.cross)
			{internet();}
					
		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImage(background);
		
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(apollo, 40, 35);
		oslPrintf_xy (38,85,"Apollo");
		oslDrawImageXY(browser, 105, 35);
		oslPrintf_xy (103,85,"Browser");
		oslDrawImageXY(calc, 165, 35);
		oslPrintf_xy (163,85,"Calculator");
		oslDrawImageXY(calendar, 225, 35);
		oslPrintf_xy (223,85,"Calendar");
		oslDrawImageXY(clockx, 285, 35);
		oslPrintf_xy (283,85,"Clock");
		oslDrawImageXY(dsp, 345, 35);
		oslPrintf_xy (343,85,"DSP");
		oslDrawImageXY(email, 405, 35);
		oslPrintf_xy (403,85,"Email");
		oslDrawImageXY(gallery, 40, 105);
		oslPrintf_xy (38,155,"Gallery");
		oslDrawImageXY(gmail, 105, 105);
		oslPrintf_xy (103,155,"Gmail");
		oslDrawImageXY(message, 165, 105);
		oslPrintf_xy (163,155,"Messages");
		oslDrawImageXY(people, 225, 105);
		oslPrintf_xy (223,155,"People");
		oslDrawImageXY(phone, 285, 105);
		oslPrintf_xy (283,155,"Phone");
		oslDrawImageXY(settings, 345, 105);
		oslPrintf_xy (343,155,"Settings");
		battery();
		android_notif();
		oslDrawImage(cursor);
		
		if (osl_pad.held.circle)
		main();

		//Synchronizes the screen 
		oslSyncFrame();	

	    //For sleep
        oslAudioVSync();
	}

	return 1;
}

