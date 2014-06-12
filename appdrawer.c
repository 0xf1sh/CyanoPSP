#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *background, *cursor, *navbar, *wificon, *apollo, *gmail, *message, *browser, *google, *calc, *clockx, *email, *people, *calendar, *phone, *gallery, *dsp, *settings,*pointer, *pointer1;

//definition of our sounds
OSL_SOUND *tone;

int appdrawer()
{
	oslIntraFontInit(INTRAFONT_CACHE_MED);
		
	//loads our images into memory
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
	oslSetFont(pgfFont);	//Set fonts
	
	//Debugger
	if (!clockx || !settings || !email || !dsp || !gallery || !calc || !phone || !calendar || !people)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
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
		
		oslDrawImageXY(navbar, 110, 237);
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
		oslDrawImageXY(pointer, 220, 221);
		oslDrawImageXY(pointer1, 234, 222);
		oslDrawImageXY(pointer1, 244, 222);
		battery();
		back();
		home_icon();
		multi();
		android_notif();
		oslDrawImage(cursor);
		
		if (osl_pad.held.square)
		{
			powermenu();
		}
		
		//Launching the browser
		if (cursor->x >= 105 && cursor->x <= 150 && cursor->y >= 35 && cursor->y <= 80 && osl_pad.held.cross)
		{
			internet();
		}
		
		if (osl_pad.held.L)
		{
			lockscreen();
        }
		
		if (osl_pad.held.circle)
		{
			home();
		}
		
		if (cursor->x >= 162 && cursor->x <= 219 && cursor->y >= 35 && cursor->y <= 80 && osl_pad.held.cross)
		{
			calculator();
		}
		
		if (cursor->x >= 344 && cursor->x <= 393 && cursor->y >= 102 && cursor->y <= 166 && osl_pad.held.cross)
		{
			settingsmenu();
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
				
        oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
		}
}

