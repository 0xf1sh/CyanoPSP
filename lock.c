#include <pspkernel.h>
#include <oslib/oslib.h>

OSL_IMAGE *background, *cursor, *wificon, *lock, *unlock, *circle, *messenger, *music, *circles, *music2, *messenger2;

int click = 0;
int ending = 0;
int batteryLife;
int style = 0;

void lockscreen_deleteimages()
{
	oslDeleteImage(lock);
	oslDeleteImage(unlock);
	oslDeleteImage(messenger);
    oslDeleteImage(circle);
	oslDeleteImage(messenger2);
	oslDeleteImage(music);
	oslDeleteImage(music2);	
	oslDeleteImage(circles);
}

int lockscreen()
{	
	setfont();
	
	lock = oslLoadImageFilePNG("System/Lockscreen/lock.png", OSL_IN_RAM, OSL_PF_8888);
	unlock = oslLoadImageFilePNG("System/Lockscreen/unlock.png", OSL_IN_RAM, OSL_PF_8888);
	circle = oslLoadImageFilePNG("System/Lockscreen/circle.png", OSL_IN_RAM, OSL_PF_8888);
	messenger = oslLoadImageFilePNG("System/Lockscreen/messenger.png", OSL_IN_RAM, OSL_PF_8888);
	messenger2 = oslLoadImageFilePNG("System/Lockscreen/messenger2.png", OSL_IN_RAM, OSL_PF_8888);
	music = oslLoadImageFilePNG("System/Lockscreen/music.png", OSL_IN_RAM, OSL_PF_8888);
	circles = oslLoadImageFilePNG("System/Lockscreen/circles.png", OSL_IN_RAM, OSL_PF_8888);
	music2 = oslLoadImageFilePNG("System/Lockscreen/music2.png", OSL_IN_RAM, OSL_PF_8888);

	if (!background || !cursor || !lock || !unlock || !circle || !messenger || !music || !circles)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	style = 0;
	click = 0;
	ending = 0;

	while (!osl_quit)
	{		
		oslStartDrawing();
		
		controls();	

		oslReadKeys();

		oslDrawImage(background);	
		
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(circles, 223, 240);
		battery();
		usb_icon();
		
		digitaltime();
		
		oslDrawImage(cursor);
		
		if (osl_pad.held.cross && cursor->x >= 190 && cursor->x <= 290 && cursor->y >= 120 && cursor->y <= 220)
		{
				click = 1;
		}
		
		if (! (osl_pad.held.cross))
			click = 0;
		if (ending == 1)
		if (style == 0)
		{
		lockscreen_deleteimages();
		return;
		}
		
		if (click == 1) {
			oslDrawImageXY(messenger, 115,160);
			oslDrawImageXY(music, 216,76);
			oslDrawImageXY(circle, cursor->x - 50, cursor->y - 50);
			}
			
		if (ending == 1) {
			oslDrawImageXY(unlock, 300,130);
			oslDrawImageXY(circle, 300,130);
			}
			
		else
			{
			oslDrawImageXY(lock, 300,130);
			
			}
			
		if (cursor->x-50 >= 270 && cursor->x - 50 <= 330 && cursor->y - 50 >= 90 && cursor->y - 50 <= 160 && click == 1) 
		{
			ending = 1;
			oslDrawImageXY(unlock, 300,130);
		}
		else 
		{
			ending = 0;
			oslDrawImageXY(lock, 300,130);
		}
		
		if (cursor->x >= 216 && cursor->x <= 276 && cursor->y >= 66 && cursor->y <= 116 && click == 1) 
		{
			ending = 0;
			oslDrawImageXY(music2, 193,57);
		}
		
		if (cursor->x >= 90 && cursor->x <= 150 && cursor->y >= 125 && cursor->y <= 195 && click == 1) 
		{
			ending = 0;
			oslDrawImageXY(messenger2, 85,130);
		}

		if (click == 0)
		{
		oslDrawImageXY(circle, 190,120);
		oslDrawImageXY(lock, 190,120);
		}
		oslDrawImage(cursor);

		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
		
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
}

