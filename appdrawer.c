#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "appdrawer.h"
#include "home.h"

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
	SetupCallbacks();
	
	oslIntraFontInit(INTRAFONT_CACHE_MED);
	
	//loads our images into memory
	background = oslLoadImageFilePNG("system/framework/framework-res/res/background.png", OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("system/Cursor/cursor.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("system/home/icons/nav.png", OSL_IN_RAM, OSL_PF_8888);
	wificon = oslLoadImageFile("system/home/icons/wificon.png", OSL_IN_RAM, OSL_PF_8888);
	apollo = oslLoadImageFilePNG("system/home/icons/apollo.png", OSL_IN_RAM, OSL_PF_8888);
	gmail = oslLoadImageFilePNG("system/home/icons/gmail.png", OSL_IN_RAM, OSL_PF_8888);
	message = oslLoadImageFilePNG("system/home/icons/message.png", OSL_IN_RAM, OSL_PF_8888);
	browser = oslLoadImageFile("system/home/icons/browser.png", OSL_IN_RAM, OSL_PF_8888);
	notif = oslLoadImageFile("system/home/menu/notif.png", OSL_IN_RAM, OSL_PF_8888);
	clockx = oslLoadImageFilePNG("system/home/icons/clock.png", OSL_IN_RAM, OSL_PF_8888);
	email = oslLoadImageFilePNG("system/home/icons/email.png", OSL_IN_RAM, OSL_PF_8888);
	settings = oslLoadImageFilePNG("system/home/icons/settings.png", OSL_IN_RAM, OSL_PF_8888);
	dsp = oslLoadImageFilePNG("system/home/icons/dsp.png", OSL_IN_RAM, OSL_PF_8888);
	gallery = oslLoadImageFilePNG("system/home/icons/gallery.png", OSL_IN_RAM, OSL_PF_8888);
	phone = oslLoadImageFilePNG("system/home/icons/phone.png", OSL_IN_RAM, OSL_PF_8888);
	calc = oslLoadImageFilePNG("system/home/icons/calc.png", OSL_IN_RAM, OSL_PF_8888);
	calendar = oslLoadImageFilePNG("system/home/icons/calendar.png", OSL_IN_RAM, OSL_PF_8888);
	people = oslLoadImageFilePNG("system/home/icons/people.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);

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
		
		//Set fonts
		oslSetFont(pgfFont);
		
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(apollo, 40, 35);
		oslDrawString(60,85,"Apollo");
		oslDrawImageXY(browser, 105, 35);
		oslDrawString(123,85,"Browser");
		oslDrawImageXY(calc, 165, 35);
		oslDrawString(188,85,"Calculator");
		oslDrawImageXY(calendar, 225, 35);
		oslDrawString(253,85,"Calendar");
		oslDrawImageXY(clockx, 285, 35);
		oslDrawString(307,85,"Clock");
		oslDrawImageXY(dsp, 345, 35);
		oslDrawString(365,85,"DSP");
		oslDrawImageXY(email, 405, 35);
		oslDrawString(427,85,"Email");
		oslDrawImageXY(gallery, 40, 110);
		oslDrawString(61,165,"Gallery");
		oslDrawImageXY(gmail, 105, 110);
		oslDrawString(125,165,"Gmail");
		oslDrawImageXY(message, 165, 110);
		oslDrawString(185,165,"Messages");
		oslDrawImageXY(people, 225, 110);
		oslDrawString(246,165,"People");
		oslDrawImageXY(phone, 285, 110);
		oslDrawString(307,165,"Phone");
		oslDrawImageXY(settings, 345, 110);
		oslDrawString(369,165,"Settings");
		battery();
		android_notif();
		oslDrawImage(cursor);
		
		if (osl_pad.held.circle)
			home();

        oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
		}
	oslQuit();
	return 1;
}

