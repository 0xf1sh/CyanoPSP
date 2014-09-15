#include <pspkernel.h>
#include <oslib/oslib.h>

OSL_IMAGE *background, *cursor, *appicon, *appicon2, *wificon, *apollo, *gmail, *message, *browser, *google, *notif, *batt100, *batt80, *batt60, *batt40, *batt20, *batt10, *batt0, *battcharge, *pointer, *pointer1, *backicon, *homeicon, *multicon;

int home()
{	
	loadicons();

	if (!background || !cursor || !wificon || !apollo || !gmail || !message || !browser || !google|| !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !pointer || !pointer1 || !backicon || !multicon || !homeicon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	setfont();

	while (!osl_quit)
	{
		oslStartDrawing();

		controls();	

		oslDrawImage(background);		
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(google, 22, 26);
		oslDrawImageXY(apollo, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(message, 160, 190);
		oslDrawImageXY(pointer, 231, 180);
		
		digitaltime(420,4,458);
		appdrawericon();
		battery();
		navbar_buttons();
		android_notif();
		oslDrawImage(cursor);
			
		if (osl_pad.held.square)
		{
			powermenu();
		}

		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_pad.held.cross)
		{
			unloadicons();
			internet();
		}
		
		if (cursor->x >= 100 && cursor->x <= 154 && cursor->y >= 195 && cursor->y <= 240 && osl_pad.held.cross)
		{
			unloadicons();
			mp3player();
		}
		
		if (cursor->x >= 155 && cursor->x <= 210 && cursor->y >= 195 && cursor->y <= 240 && osl_pad.held.cross)
		{
			unloadicons();
			messenger();
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
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
		
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
}

