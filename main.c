#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
 
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
OSL_IMAGE *background, *cursor, *appicon, *appicon2, *navbar, *wificon, *apollo, *gmail, *message, *browser, *google;

//variables
int cursor_position;
int app_drawer;

//function declarations
void controls();
void internet();
void app_menu();

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
	tone = oslLoadSoundFile("System/Media/Audio/Ui/KeypressStandard.wav", OSL_FMT_NONE);

	//loads our images into memory
	background = oslLoadImageFilePNG("System/Home/Wallpapers/1.png", OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("System/Cursor/cursor.png", OSL_IN_RAM, OSL_PF_8888);
	appicon = oslLoadImageFilePNG("System/Home/Icons/appicon1.png", OSL_IN_RAM, OSL_PF_8888);
	appicon2 = oslLoadImageFile("System/Home/Icons/appicon2.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("System/Home/Icons/nav.png", OSL_IN_RAM, OSL_PF_8888);
	wificon = oslLoadImageFile("System/Home/Icons/wificon.png", OSL_IN_RAM, OSL_PF_5551);
	apollo = oslLoadImageFilePNG("System/Home/Icons/apollo.png", OSL_IN_RAM, OSL_PF_5551);
	gmail = oslLoadImageFilePNG("System/Home/Icons/gmail.png", OSL_IN_RAM, OSL_PF_5551);
	message = oslLoadImageFilePNG("System/Home/Icons/message.png", OSL_IN_RAM, OSL_PF_5551);
	browser = oslLoadImageFile("System/Home/Icons/browser.png", OSL_IN_RAM, OSL_PF_8888);
	google = oslLoadImageFile("System/Home/Icons/google.png", OSL_IN_RAM, OSL_PF_5551);
	
	//Disables the transpaent color (blue)
	oslDisableTransparentColor();
	
	//Debugger
	if (!background || !cursor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	app_drawer = 1;
	
	//Main loop to run the program
	while (!osl_quit)
	{
		//Draws images onto the screen
		oslStartDrawing();
		
		//calls the controls() function
		controls();	
		
		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImage(background);
		
		if (cursor->x  >= 215 && cursor->x  <= 243 && cursor->y >= 195 && cursor->y <= 230)
		oslDrawImageXY(appicon2,223,205);
	
		else
		oslDrawImageXY(appicon,223,205);
		
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(google, 22, 26);
		oslDrawImageXY(apollo, 105, 195);
		oslDrawImageXY(browser, 276, 195);
		oslDrawImageXY(gmail, 331, 195);
		oslDrawImageXY(message, 160, 195);
		oslDrawImage(cursor);
	

				
		//Ends printing
		oslEndDrawing();

		//Synchronizes the screen 
		oslSyncFrame();	

	    //For sleep
        oslAudioVSync();
	}
	
	//Terminates/Ends the program
	oslEndGfx();
	oslQuit();
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
		
		//Launching the browser
		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_pad.held.cross) {
					internet();
		}
				
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
			
    //Quit OsLib:
	oslNetTerm();
	oslEndGfx();
    oslQuit();

    sceKernelExitGame();
	return 0;
}


		
		



