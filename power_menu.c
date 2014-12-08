#include "power_menu.h"
#include "lock.h"
//#include <scepower.h>

OSL_IMAGE *background, *cursor, *power, *apollo, *gmail, *messengericon, *browser, *pointer, *powerOff, *cancelPowerOff, *okPowerOff, *power1, *recovery1, *screenshot1;

void powermenu()
{	
	
	power = oslLoadImageFilePNG("system/home/menu/power.png", OSL_IN_RAM, OSL_PF_8888);
	power1 = oslLoadImageFilePNG("system/home/menu/power1.png", OSL_IN_RAM, OSL_PF_8888);
	recovery1 = oslLoadImageFilePNG("system/home/menu/recovery1.png", OSL_IN_RAM, OSL_PF_8888);
	screenshot1 = oslLoadImageFilePNG("system/home/menu/screenshot1.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!power || !power1 || !recovery1 || !screenshot1)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();

		controls();	

		oslClearScreen(RGB(0,0,0));	
		oslDrawImage(background);
		oslDrawImageXY(apollo, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(messengericon, 160, 190);
		oslDrawImageXY(pointer, 231, 180);
		oslDrawImageXY(power, 102, 41);		
		
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 47 && cursor->y <= 106) 
		{
			oslDrawImageXY(power1, 102, 47);
			if (osl_keys->pressed.cross)
			{	
				oslDeleteImage(power);
				oslDeleteImage(power1);
				oslDeleteImage(recovery1);
				oslDeleteImage(screenshot1);
				PowerOffMenu();
			}
		}
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 106 && cursor->y <= 165) 
		{
			oslDrawImageXY(recovery1, 102, 106);
			if (osl_keys->pressed.cross)
			{	
				oslSyncFrame();
				sceKernelDelayThread(3*1000000);
				oslDeleteImage(power);
				oslDeleteImage(power1);
				oslDeleteImage(recovery1);
				oslDeleteImage(screenshot1);
				mainRecoveryMenu();
			}
		}
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 165 && cursor->y <= 224) 
		{
			oslDrawImageXY(screenshot1, 102, 165);
			if (osl_keys->pressed.cross)
			{	
				oslDeleteImage(power);
				oslDeleteImage(power1);
				oslDeleteImage(recovery1);
				oslDeleteImage(screenshot1);
				return;
				screenshot();
			}
		}
		
		oslDrawImage(cursor);
	
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(power);
			oslDeleteImage(power1);
			oslDeleteImage(recovery1);
			oslDeleteImage(screenshot1);
			return;
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

void PowerOffMenu()
{
	powerOff = oslLoadImageFilePNG("system/home/menu/powerOff.png", OSL_IN_RAM, OSL_PF_8888);
	okPowerOff = oslLoadImageFilePNG("system/home/menu/okPowerOff.png", OSL_IN_RAM, OSL_PF_8888);
	cancelPowerOff = oslLoadImageFilePNG("system/home/menu/cancelPowerOff.png", OSL_IN_RAM, OSL_PF_8888);
		
	if (!powerOff || !okPowerOff || !cancelPowerOff)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();

		controls();	

		oslClearScreen(RGB(0,0,0));	
		oslDrawImage(background);
		oslDrawImageXY(apollo, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(messengericon, 160, 190);
		oslDrawImageXY(pointer, 231, 180);
		oslDrawImageXY(powerOff, 90, 41);
		
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 105 && cursor->x <= 239 && cursor->y >= 173 && cursor->y <= 217) //Pressing cancel exits the app
		{
			oslDrawImageXY(cancelPowerOff, 105, 173);
			if (osl_keys->pressed.cross)
			{	
				oslDeleteImage(powerOff);
				oslDeleteImage(cancelPowerOff);
				oslDeleteImage(okPowerOff);
				sceKernelExitGame();
			}
		}
		
		if (cursor->x >= 239 && cursor->x <= 377 && cursor->y >= 173 && cursor->y <= 217) //Pressing ok, shuts down the PSP
		{
			oslDrawImageXY(okPowerOff, 239, 173);
			if (osl_keys->pressed.cross)
			{	
				oslDeleteImage(powerOff);
				oslDeleteImage(cancelPowerOff);
				oslDeleteImage(okPowerOff);
				scePowerRequestStandby();
				//scePower_0442D852(50000);
			}
		}
		
		oslDrawImage(cursor);

		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(powerOff);
			oslDeleteImage(cancelPowerOff);
			oslDeleteImage(okPowerOff);
			powermenu();
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