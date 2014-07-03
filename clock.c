#include <pspkernel.h>
#include <pspdebug.h>
#include <oslib/oslib.h>
#include <psprtc.h>

//declaration
OSL_IMAGE *clockbg, *cursor, *wificon;

OSL_FONT *DroidSans;

void centerclock()
{
	//Load fonts:
	OSL_FONT *clockFont = oslLoadIntraFontFile("system/fonts/DroidSans.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(clockFont, 0.9f, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
	//Set fonts
	oslSetFont(clockFont);
	
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (time.hour > 12)
		time.hour -= 12;
	
	oslDrawStringf(240,136,"%02d:%02d", time.hour, time.minutes);
	
}

void digitaltime()
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if(time.hour >= 12) 
		oslDrawString(458,4,"PM"); 
	else 
		oslDrawString(458,4,"AM");   
		
	if (time.hour > 12)
		time.hour -= 12;
	
	if (time.hour == 00)
	time.hour = 12;
		
	oslDrawStringf(420,4,"%02d:%02d", time.hour, time.minutes);	
}

int pspclock()
{
	//loads our images into memory
	clockbg = oslLoadImageFilePNG("system/app/clock/clockbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Debugger
	if (!clockbg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();
	
	while (!osl_quit)
  {		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(clockbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		digitaltime();
		
		//calls the functions
		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		digitaltime();
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
			oslDeleteImage(clockbg);
			appdrawer();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(clockbg);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(clockbg);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(clockbg);
			multitask();
		}
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
	}
}



