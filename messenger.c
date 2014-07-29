#include <pspkernel.h>
#include <oslib/oslib.h>

OSL_IMAGE *messengerbg, *new_message, *cursor, *wificon;
OSL_FONT *pgfFont;

int InitAdhocMessenger();
void joinServer();
void createServer();

char message[500] = "";	

void activateOSK()
{
	int enabled = 1;
	while(1)
	if (enabled){
		oslDrawOsk();
		oslInitOsk("Type Message", "", 128, 1, -1);
		memset(message, 0, sizeof(message));
	}
}

void newmessage()
{	
	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(new_message, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		oslDrawString(30, 200, message);
		
		digitaltime();

		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		oslDrawImage(cursor);
		
		if (cursor->x >= 12 && cursor->x <= 415 && cursor->y >= 233 && cursor->y <= 270 && osl_pad.held.cross)
		{
		activateOSK();
		}
		
		if (oslOskGetResult() == OSL_OSK_CANCEL)
		{
						oslDrawStringf(40,40,message, "Cancel");
		}
					else{
						char userText[100] = "";
						oslOskGetText(userText);
						oslDrawStringf(17,238, message, "%s", userText);
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
			messenger();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			messenger();
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
}

int messenger()
{	
	messengerbg = oslLoadImageFilePNG("system/app/messenger/messengerbg.png", OSL_IN_RAM, OSL_PF_8888);
	new_message = oslLoadImageFilePNG("system/app/messenger/new_message.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!messengerbg || !new_message || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(0,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(pgfFont);
		
	while (!osl_quit)
  {
		//Draws images onto the screen
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(messengerbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		battery();
		digitaltime();
		navbar_buttons();
		android_notif();
		usb_icon();
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
		
		if (cursor->x >= 275 && cursor->x <= 376 && cursor->y >= 20 && cursor->y <= 52 && osl_pad.held.cross)
		{
		newmessage();
		}
		
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
}

