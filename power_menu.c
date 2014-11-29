#include "power_menu.h"
#include "lock.h"

OSL_IMAGE *background, *cursor, *wificon, *power, *apollo, *gmail, *messengericon, *browser, *pointer;

void power_off()
{
	scePowerRequestStandby();
}

int powermenu()
{	
	power = oslLoadImageFilePNG("system/home/menu/power.png", OSL_IN_RAM, OSL_PF_8888);

	if (!cursor || !wificon || !power)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();

		controls();	
		
		oslDrawImage(background);
		oslDrawImageXY(wificon, 350, 1);
		oslDrawImageXY(apollo, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(messengericon, 160, 190);
		oslDrawImageXY(pointer, 231, 180);
		oslDrawImageXY(power, 114, 10);	
		oslDrawImageXY(power, 114, 10);		
		
		digitaltime(420,4,458);
		battery();
	
		oslDrawImage(cursor);

		if (osl_keys->pressed.circle)
		{
			
			return;
		}
			
		if (cursor->x >= 128 && cursor->x <= 354 && cursor->y >= 25 && cursor->y <= 86 && osl_keys->pressed.cross)
		{
			sceKernelExitGame();
		}
		
		if (cursor->x >= 128 && cursor->x <= 354 && cursor->y >= 88 && cursor->y <= 140 && osl_keys->pressed.cross)
		{
			oslSyncFrame();
			sceKernelDelayThread(3*1000000);
			oslDeleteImage(power);
			mainRecoveryMenu();
		}
		
		if (cursor->x >= 128 && cursor->x <= 354 && cursor->y >= 192 && cursor->y <= 242 && osl_keys->pressed.cross)
		{	
			screenshot();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
}

