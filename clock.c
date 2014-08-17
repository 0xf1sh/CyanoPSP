#include <pspkernel.h>
#include <pspdebug.h>
#include <oslib/oslib.h>
#include <pspctrl.h>
#include <psprtc.h>

OSL_IMAGE *clockbg, *cursor, *wificon, *stop_watch;
OSL_FONT *clockFont;

int hour = 0;
int minute = 0;
int second = 0;
int milisec = 0;

int counter = 0;
int timer = 0;

char shour[2];
char sminute[2];
char ssecond[2];
char smilisec[2];
char stimer[10];

void centerclock()
{
	clockFont = oslLoadIntraFontFile("system/fonts/DroidSans.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(clockFont, 0.9f, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
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

void startCounter()
{
	SceCtrlData newPad, oldPad;

	while (1)
	{
		oldPad = newPad;
		sceCtrlReadBufferPositive(&newPad, 1);
		if (newPad.Buttons != oldPad.Buttons)
		{
			if (newPad.Buttons & PSP_CTRL_CROSS)
			{
				break;
			}
		}

		if (hour == 60)
		{
			hour = 0;
		}
		if (minute == 60)
		{
			hour++;
			minute = 0;
		}
		if (second == 60)
		{
			minute++;
			second = 0;
		}
		if (milisec == 60)
		{
			second++;
			milisec = milisec *1.67;
		}
		oslDrawStringf(200,200,"%d:%d:%d.%d       ", hour, minute, second, milisec);
		milisec++;
	}
}

void stopWatch()
{
	stop_watch = oslLoadImageFilePNG("system/app/clock/stop_watch.png", OSL_IN_RAM, OSL_PF_8888);

	if (!stop_watch)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();
	
	SceCtrlData newPad, oldPad;
	sceCtrlReadBufferPositive(&oldPad, 1);
	
	while (!osl_quit)
  {		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(stop_watch, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		digitaltime();

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
			oslDeleteImage(stop_watch);
			appdrawer();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(stop_watch);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(stop_watch);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(stop_watch);
			multitask();
		}
		
		if (cursor->x >= 114 && cursor->x <= 150  && cursor->y >= 19 && cursor->y <= 50 && osl_pad.held.cross)
		{
			oslDeleteImage(stop_watch);
			pspclock();
		}
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
		
		oldPad = newPad;
		sceCtrlReadBufferPositive(&newPad, 1);

		if (newPad.Buttons != oldPad.Buttons)
		{
			if (newPad.Buttons & PSP_CTRL_CROSS)
			{
				if (counter == 0)
				{
					counter = 1;
					startCounter();
				}
			}
			if (newPad.Buttons & PSP_CTRL_RTRIGGER)
			{
				hour = 0;
				minute = 0;
				second = 0;
				milisec = 0;
				oslDrawStringf(211,175, "0:0:0.00       ");
				counter = 0;
			}
		}

		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
}

int pspclock()
{
	clockbg = oslLoadImageFilePNG("system/app/clock/clockbg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!clockbg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();
	
	while (!osl_quit)
  {		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(clockbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		digitaltime();

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
		
		if (cursor->x >= 285 && cursor->x <= 332  && cursor->y >= 19 && cursor->y <= 50 && osl_pad.held.cross)
		{
			oslDeleteImage(clockbg);
			stopWatch();
		}
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
		
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
}



