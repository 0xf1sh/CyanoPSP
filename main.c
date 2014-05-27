#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
#include "appdrawer.h"
#include "lock.h"
#include "clock.h"
 
PSP_MODULE_INFO("CyanogenMod PSP", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(4*1024);

// Globals:

int runningFlag = 1;
 
// Callbacks:

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
    runningFlag = 0;
    return 0;
}
 
/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
    int cbid;
 
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}
 
/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
    int thid = 0;
    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

//declaration
OSL_IMAGE *background, *cursor, *appicon, *appicon2, *navbar, *wificon, *apollo, *gmail, *message, *browser, *google, *notif, *batt100, *batt80, *batt60, *batt40, *batt20, *batt10, *batt0, *battcharge, *power, *pointer, *pointer1;

//variables
int cursor_position;
int app_drawer;
int result;
int notif_y;
int notif_up;
int notif_down;
int notif_enable;
int batx = 415;
int baty = 2;
int batteryLife;

//function declarations
void controls();
void internet();
void android_notif();
void battery();
void appdrawericon();
void powermenu();

//definition of our sounds
OSL_SOUND *tone;

int initOSLib(){
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
    oslInitAudio();
    oslSetQuitOnLoadFailure(1);
    oslSetKeyAutorepeatInit(40);
    oslSetKeyAutorepeatInterval(10);
    return 0;
}
 
void controls()
{
		//Cursor movement
		//Enable default analog handler
		oslSetKeyAnalogToDPad(80);
		
		//Read keys
		oslReadKeys();
		
		//The stick is upwards OR the D-pad's up direction is held
		if (osl_pad.held.up)
        {cursor->y -= 4;}
		if (osl_pad.held.down)
        {cursor->y += 4;}
		if (osl_pad.held.left)
        {cursor->x -= 4;}
		if (osl_pad.held.right)
        {cursor->x += 4;}
		
		//Touch tones
        if (osl_keys->pressed.cross) oslPlaySound(tone, 1);         
        // It loads the sound file defined in the tones variable  when the
        //  cross button is pressed, in channel 1. 
			
}

void battery()
{
		batteryLife = scePowerGetBatteryLifePercent();
 
		if (batteryLife == 100)
			oslDrawImageXY(batt100,batx,baty);
		else if (batteryLife >80 && batteryLife <= 100) 
			oslDrawImageXY(batt80,batx,baty);
		else if (batteryLife >60 && batteryLife <= 80)
			oslDrawImageXY(batt60,batx,baty);
		else if (batteryLife >40 && batteryLife <= 60)
			oslDrawImageXY(batt40,batx,baty);
		else if (batteryLife >20 && batteryLife <= 40) 
			oslDrawImageXY(batt20,batx,baty);
		else if (batteryLife >10 && batteryLife <= 20)
			oslDrawImageXY(batt10,batx,baty);
		else if (batteryLife >0 && batteryLife <= 10)
			oslDrawImageXY(batt0,batx,baty);
			
		if (scePowerIsBatteryCharging() == 1)
			oslDrawImageXY(battcharge,batx,baty);
		
}

void appdrawericon()
{
		if (cursor->x  >= 215 && cursor->x  <= 243 && cursor->y >= 195 && cursor->y <= 230)
		oslDrawImageXY(appicon2,223,205);
	
		else
		oslDrawImageXY(appicon,223,205);
}

void powermenu()
{
		if (osl_pad.held.square)
		oslDrawImageXY(power,125,36);
		if (osl_pad.held.triangle)
		return 0;
		
}

void android_notif()
{
		notif_y = -246;
		oslDrawImageXY(notif,0,notif_y);
		
		if ((osl_pad.held.cross && notif_y == 26 && cursor->y <= 26) || (osl_pad.held.cross && notif_y == 26 && cursor->y >= 246))
			notif_up = 1;
			notif_down = 0;

		if (notif_up == 1) 
			notif_y=notif_y-10;

		if (notif_y <= -246) 
			notif_y = -246;
			notif_enable = 0;
			notif_up = 0;

		if (notif_y < -246)
			notif_y = -246;

		if (osl_pad.held.cross && cursor->y <= 26 && notif_y == -246) 
			notif_down = 1;
			notif_up = 0;
			notif_enable = 1;

		if (notif_down == 1) 
			notif_y = notif_y+10;

		if (notif_y == 26)
			notif_down = 0;
			notif_enable = 1;

		if (notif_y > 26) 
			notif_y = 26;	
}

void internet()
{
int skip = 0;
    char message[100] = "";
    int browser = 0;
    SetupCallbacks();

    initOSLib();
    oslIntraFontInit(INTRAFONT_CACHE_MED);
	oslNetInit();

    //Load font:
    OSL_FONT *font = oslLoadFontFile("flash0:/font/ltn0.pgf");
    oslSetFont(font);

    while(runningFlag && !osl_quit){
		browser = oslBrowserIsActive();
		if (!skip){
            oslStartDrawing();
            oslDrawString(30, 200, message);

            if (browser){
                oslDrawBrowser();
                if (oslGetBrowserStatus() == PSP_UTILITY_DIALOG_NONE){
					sprintf(message, "Browser closed");
                    oslEndBrowser();
                }
            }
            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();

        if (!browser){
            oslReadKeys();
                int res = oslBrowserInit("http://www.google.com", "/PSP/PHOTO", 5*1024*1024,
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
                memset(message, 0, sizeof(message));
				if (res)
					sprintf(message, "Error %i initializing browser!", res);
				else
					sprintf(message, "Browser initialized.");
			}
        }
    //Quit OSL:
	oslNetTerm();
	oslEndGfx();
    oslQuit();
}

int main()
{
	//Initialization of the Oslib's library
	oslInit(0);

	//Initialization of Oslib's graphics mode
	oslInitGfx(OSL_PF_8888, 1);
	
	//Sets the transparency color (blue)
	oslSetTransparentColor(RGB(0,0,255));
	
	//Initialization of Oslib's text console
    oslInitConsole();

    //Initialization of Oslib's audio console
    oslInitAudio();
	
	//loads our sound
	tone = oslLoadSoundFile("system/media/audio/ui/KeypressStandard.wav", OSL_FMT_NONE);

	int defaultimg = 1;

	oslIntraFontInit(INTRAFONT_CACHE_MED);
	
	//loads our images into memory
	background = oslLoadImageFilePNG("system/framework/framework-res/res/background.png", OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("system/cursor/cursor.png", OSL_IN_RAM, OSL_PF_8888);
	appicon = oslLoadImageFilePNG("system/home/icons/appicon1.png", OSL_IN_RAM, OSL_PF_8888);
	appicon2 = oslLoadImageFile("system/home/icons/appicon2.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("system/home/icons/nav.png", OSL_IN_RAM, OSL_PF_8888);
	wificon = oslLoadImageFile("system/home/icons/wificon.png", OSL_IN_RAM, OSL_PF_8888);
	apollo = oslLoadImageFilePNG("system/home/icons/apollo.png", OSL_IN_RAM, OSL_PF_5551);
	gmail = oslLoadImageFilePNG("system/home/icons/gmail.png", OSL_IN_RAM, OSL_PF_5551);
	message = oslLoadImageFilePNG("system/home/icons/message.png", OSL_IN_RAM, OSL_PF_5551);
	browser = oslLoadImageFile("system/home/icons/browser.png", OSL_IN_RAM, OSL_PF_8888);
	google = oslLoadImageFile("system/home/icons/google.png", OSL_IN_RAM, OSL_PF_5551);
	notif = oslLoadImageFile("system/home/menu/notif.png", OSL_IN_RAM, OSL_PF_8888);
	batt100 = oslLoadImageFile("system/home/icons/100.png", OSL_IN_RAM, OSL_PF_5551);
	batt80 = oslLoadImageFile("system/home/icons/80.png", OSL_IN_RAM, OSL_PF_5551);
	batt60 = oslLoadImageFile("system/home/icons/60.png", OSL_IN_RAM, OSL_PF_5551);
	batt40 = oslLoadImageFile("system/home/icons/40.png", OSL_IN_RAM, OSL_PF_5551);
	batt20 = oslLoadImageFile("system/home/icons/20.png", OSL_IN_RAM, OSL_PF_5551);
	batt10 = oslLoadImageFile("system/home/icons/10.png", OSL_IN_RAM, OSL_PF_5551);
	batt0 = oslLoadImageFile("system/home/icons/0.png", OSL_IN_RAM, OSL_PF_5551);
	battcharge = oslLoadImageFile("system/home/icons/charge.png", OSL_IN_RAM, OSL_PF_5551);
	power = oslLoadImageFile("system/home/menu/power.png", OSL_IN_RAM, OSL_PF_8888);
	pointer = oslLoadImageFilePNG("system/home/icons/pointer.png", OSL_IN_RAM, OSL_PF_8888);
	pointer1 = oslLoadImageFilePNG("system/home/icons/pointer1.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
	
	//Disables the transpaent color (blue)
	oslDisableTransparentColor();
	
	//Debugger
	if (!background || !cursor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	//Main loop to run the program
	while (!osl_quit)
	{
		sceRtcGetCurrentClockLocalTime(&time);

		//Draws images onto the screen
		oslStartDrawing();
		
		//calls the functions
		controls();	
											
		//Initiate the PSP's controls
		oslReadKeys();

		//Set fonts
		oslSetFont(pgfFont);
		
		//Print the images onto the screen
		oslDrawImage(background);		
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(google, 22, 26);
		oslDrawImageXY(apollo, 105, 195);
		oslDrawImageXY(browser, 276, 195);
		oslDrawImageXY(gmail, 331, 195);
		oslDrawImageXY(message, 160, 195);
		oslDrawImageXY(pointer, 231, 180);
		appdrawericon();
		battery();
		android_notif();
		powermenu();
		oslDrawImage(cursor);
			
		//Launching the browser
		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_pad.held.cross)
			internet();
			
		if (cursor->x >= 215 && cursor->x <= 243 && cursor->y >= 195 && cursor->y <= 230 && osl_pad.held.cross)
			appdrawer();
		
		if (osl_pad.pressed.L)
		{
			lockscreen();
        }
			
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

