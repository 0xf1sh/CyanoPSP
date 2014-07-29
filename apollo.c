#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psppower.h>
#include <oslib/oslib.h>
#include "mp3player.h"

#define MP3file "ms0:/MUSIC/test.mp3"

OSL_IMAGE *mp3bg, *cursor, *wificon;


int mp3player()
{	

	mp3bg = oslLoadImageFilePNG("system/app/apollo/mp3bg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!mp3bg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	scePowerSetClockFrequency(333, 333, 166);
	
	pspAudioInit();
	
	int i;
	MP3_Init(1);
	MP3_Load(MP3file);
	MP3_Play();
	
	while (!osl_quit)
  {
		//Draws images onto the screen
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslReadKeys();

		oslDrawImageXY(mp3bg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		battery();
		digitaltime();
		navbar_buttons();
		android_notif();
		usb_icon();
		oslDrawImage(cursor);
		
		if(osl_pad.held.triangle) {
		break;
		}
		
		else if(osl_pad.held.cross) 
		{
		MP3_Pause();
		for(i=0; i<10; i++) {
		sceDisplayWaitVblankStart();
		}
		}
		
		if (MP3_EndOfStream() == 1) 
		{
		MP3_Stop();
		}
	
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
			appdrawer();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
		
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
		}
	MP3_Stop();
	MP3_FreeTune();
}


