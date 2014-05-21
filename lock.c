#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "home.h"
 

//declaration
OSL_IMAGE *background, *cursor, *wificon, *lock, *unlock, *circle, *camera, *music, *circles;

//variables
int click = 0;
int ending = 0;
int batteryLife;
int style = 0;

//function declarations
void controls();
void battery();

//definition of our sounds
OSL_SOUND *tone;

int lockscreen()
{
	//loads our images into memory
	lock = oslLoadImageFilePNG("System/Lockscreen/lock.png", OSL_IN_RAM, OSL_PF_8888);
	unlock = oslLoadImageFilePNG("System/Lockscreen/unlock.png", OSL_IN_RAM, OSL_PF_8888);
	circle = oslLoadImageFilePNG("System/Lockscreen/circle.png", OSL_IN_RAM, OSL_PF_8888);
	camera = oslLoadImageFilePNG("System/Lockscreen/camera.png", OSL_IN_RAM, OSL_PF_8888);
	music = oslLoadImageFilePNG("System/Lockscreen/music.png", OSL_IN_RAM, OSL_PF_8888);
	circles = oslLoadImageFilePNG("System/Lockscreen/circles.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Debugger
	if (!background || !cursor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	style = 0;
	click = 0;
	ending = 0;
	
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
		
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(circles, 223, 240);
		battery();
		oslDrawImage(cursor);
		
		if (osl_pad.held.cross && cursor->x >= 190 && cursor->x <= 290 && cursor->y >= 120 && cursor->y <= 220)
			click = 1;
		else
			click = 0;
		if (ending == 1)
		if (style == 0)
		home();

		if (click == 1) {
			oslDrawImageXY(camera, 80,130);
			oslDrawImageXY(music, 216,76);
			}
		if (ending == 1) {
			oslDrawImageXY(unlock, 300,130);
			oslDrawImageXY(circle, 300,130);
			}
		else
			{
			oslDrawImageXY(lock, 300,130);
			oslDrawImageXY(circle, cursor->x - 50, cursor->y - 50);
			}
			
		if (cursor->x-50 >= 270 && cursor->x - 50 <= 330 && cursor->y - 50 >= 100 && cursor->y - 50 <= 160)
			ending = 1;
		else 
			ending = 0;
		//Ends printing
		oslEndDrawing();

		//Synchronizes the screen 
		oslSyncFrame();	
					   
	    //For sleep
        oslAudioVSync();
	}
	
	//Terminates/Ends the program
	oslQuit();
	return 0;
}

