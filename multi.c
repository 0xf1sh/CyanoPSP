#include <pspkernel.h>
#include <oslib/oslib.h>
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"

OSL_IMAGE *background, *cursor, *multi_task, *wificon;

int multitask()
{	
	multi_task = oslLoadImageFilePNG("system/home/menu/multi_task.png", OSL_IN_RAM, OSL_PF_8888);
	background = oslLoadImageFilePNG("system/framework/framework-res/res/background.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();

	if (!background || !cursor|| !multi_task)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		controls();

		oslDrawImageXY(background, 0,0);
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(multi_task, 0,10);
		
		oslDrawString(200,136,"No recent apps");

		digitaltime(420,4,458);
		battery();
		navbar_buttons();
		android_notif();
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
			oslDeleteImage(background);
			oslDeleteImage(multi_task);
			return;
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(background);
			oslDeleteImage(multi_task);
			return;
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(background);
			oslDeleteImage(multi_task);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(background);
			oslDeleteImage(multi_task);
			multitask();
		}
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}		
	
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

