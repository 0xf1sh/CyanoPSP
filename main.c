#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>

//PSP Net 
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>

//PSP USB
#include <pspusb.h>
#include <pspusbstor.h>
#include <pspmodulemgr.h>

//OSLib
#include <oslib/oslib.h>

//PSP Time
#include <psprtc.h>

#include "include/pspusbdevice.h"
#include "main.h"
#include "appdrawer.h"
#include "home.h"
#include "lock.h"
#include "clock.h"
#include "calculator.h"
#include "settingsmenu.h"
#include "multi.h"
#include "power_menu.h"
#include "apollo.h"
#include "fm.h"
#include "recoverymenu.h"
#include "mp3player.h"
#include "game.h"
#include "screenshot.h"

#define screenshotpath "ms0:/PSP/GAME/CyanogenMod/screenshots"
#define downloadpath "ms0:/PSP/GAME/CyanogenMod/downloads"

PSP_MODULE_INFO("CyanoPSP - C", 0x200, 2, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-128);

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
OSL_IMAGE *background, *cursor, *appicon, *appicon2, *navbar, *wificon, *apollo, *gmail, *message, *browser, *google, *notif, *batt100, 
		  *batt80, *batt60, *batt40, *batt20, *batt10, *batt0, *battcharge, *pointer, *pointer1, *backicon, *homeicon, *multicon, *usbdebug, 
		  *recoverybg, *welcome, *ok, *transbackground, *notif2;

//definition of our sounds
OSL_SOUND *tone;

int initOSLib(){
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
	oslSetBilinearFilter(1);
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
		
		if (cursor->x <= llimit)
		{cursor->x = llimit;}
		
		else if (cursor->x >= rlimit)
		{cursor->x = rlimit;}
		
		if (cursor->y <= ulimit)
		{cursor->y = ulimit;}
		
		else if (cursor->y >= dlimit)
		{cursor->y = dlimit;}
		
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

void makescreenshotdir()
{
	SceUID dir = sceIoDopen(screenshotpath);
	
	if (dirExists(screenshotpath))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenMod/screenshots",0777);
}
}

void makedownloaddir()
{
	SceUID dir = sceIoDopen(downloadpath);
	
	if (dirExists(downloadpath))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenMod/downloads",0777);
}
}

void appdrawericon()
{
		if (cursor->x  >= 215 && cursor->x  <= 243 && cursor->y >= 195 && cursor->y <= 230)
		oslDrawImageXY(appicon2,223,200);
	
		else
		oslDrawImageXY(appicon,223,200);
}

void navbar_buttons()
{		
		oslDrawImageXY(navbar, 110, 237);
		
		if (cursor->x  >= 144 && cursor->x  <= 204 && cursor->y >= 226 && cursor->y <= 271)
		oslDrawImageXY(backicon,110, 237);
	
		else
		oslDrawImageXY(navbar,110, 237);
		
		if (cursor->x  >= 205 && cursor->x  <= 271 && cursor->y >= 226 && cursor->y <= 271)
		oslDrawImageXY(homeicon,110, 237);
	
		else
		oslDrawImageXY(navbar,110, 237);
		
		if (cursor->x  >= 272 && cursor->x  <= 332 && cursor->y >= 226 && cursor->y <= 271)
		oslDrawImageXY(multicon,110, 237);
	
		else
		oslDrawImageXY(navbar,110, 237);
}

void android_notif()
{		
		oslDrawImageXY(notif,0,notif_y);
		
		if ((osl_pad.held.cross && notif_y == 0 && cursor->x >= 0 && cursor->x <= 240 && cursor->y <= 1) || (osl_pad.held.cross && notif_y == 0 && cursor->y >= 246))
		{
			notif_up = 1;
			notif_down = 0;
		}

		if (cursor->x >= 437 && cursor->x <= 466 && cursor->y >= 12 && cursor->y <= 37 && osl_pad.held.cross)
		{
			android_notif2();
		}
		
		if (notif_up == 1) 
		{
			notif_y=notif_y-10;
		}
		
		if (notif_y <= -272) 
		{
			notif_y = -272;
			notif_enable = 0;
			notif_up = 0;
		}

		if (notif_y < -272)
		{
			notif_y = -272;
		}
		
		if (osl_pad.held.cross && cursor->x >= 0 && cursor->x <= 240  && cursor->y <= 1 && notif_y == -272) 
		{
			notif_down = 1;
			notif_up = 0;
			notif_enable = 1;
		}
		
		if (notif_down == 1) 
		{
			notif_y = notif_y+10;
		}
		
		if (notif_y == 0)
		{
			notif_down = 0;
			notif_enable = 1;
		}
		
		if (notif_y > 0) 
		{
			notif_y = 0;	
		}
}

void android_notif2()
{		
		oslDrawImageXY(notif2,0,notif_y);
		
		if ((osl_pad.held.cross && notif_y == 0 && cursor->x >= 240 && cursor->x <=480 && cursor->y <= 1) || (osl_pad.held.cross && notif_y == 0 && cursor->y >= 246))
		{
			notif_up = 1;
			notif_down = 0;
		}
	
		if (notif_up == 1) 
		{
			notif_y=notif_y-10;
		}
		
		if (notif_y <= -272) 
		{
			notif_y = -272;
			notif_enable = 0;
			notif_up = 0;
		}

		if (notif_y < -272)
		{
			notif_y = -272;
		}
		
		if (osl_pad.held.cross && cursor->x >= 240 && cursor->x <=480 && cursor->y <= 1  && notif_y == -272) 
		{
			notif_down = 1;
			notif_up = 0;
			notif_enable = 1;
		}
		
		if (notif_down == 1) 
		{
			notif_y = notif_y+10;
		}
		
		if (notif_y == 0)
		{
			notif_down = 0;
			notif_enable = 1;
		}
		
		if (notif_y > 0) 
		{
			notif_y = 0;	
		}
}

void __psp_free_heap(void); 

void internet()
{
int browser = 0; 
    int skip=0; 
    oslNetInit(); 
    oslBrowserInit("https://www.google.com/","/PSP/GAME/CyanogenMod/downloads",10*1024*1024, 
                        PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT, 
                        PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS, 
                        PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL, 
                        PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL); 
  
    while(!osl_quit){ 
        browser = oslBrowserIsActive(); 
        if (!skip){ 
            oslStartDrawing(); 
            oslCls(); 
            oslDrawBrowser(); 
            if (browser){ 
                oslDrawBrowser(); 
                if (oslGetBrowserStatus() == PSP_UTILITY_DIALOG_NONE){ 
                    oslEndBrowser(); 
                    break; 
                } 
            } 
            oslEndDrawing(); 
        } 
        oslEndFrame(); 
        skip = oslSyncFrame(); 
    } 
    oslNetTerm();
}

//First Boot Message
void firstBootMessage(){
	
	if (loadConfig() && firstBoot == 1)
	{
	oslDrawImageXY(transbackground, 0, 0);
	oslDrawImageXY(welcome, 140, 40);
	oslDrawImageXY(ok, 360, 200);
	}
	
	if (cursor->x >= 360 && cursor->x <= 460 && cursor->y >= 200 && cursor->y <= 250 && osl_pad.held.cross)
	{
		oslDeleteImage(welcome);
		oslDeleteImage(ok);
		oslDeleteImage(transbackground);
		return firstBoot=0;
		saveConfig();
		home();
	}
}

void setfont()
{
	OSL_FONT *DroidSans = oslLoadIntraFontFile("system/fonts/DroidSans.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(DroidSans, 0.5f, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(DroidSans);
}

void loadicons()
{
	appicon = oslLoadImageFilePNG("system/home/icons/appicon1.png", OSL_IN_RAM, OSL_PF_8888);
	appicon2 = oslLoadImageFile("system/home/icons/appicon2.png", OSL_IN_RAM, OSL_PF_8888);
}

void unloadicons()
{
	oslDeleteImage(appicon);
	oslDeleteImage(appicon2);
}

int main()
{
	initOSLib();
	oslIntraFontInit(INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	
	//loads our sound
	tone = oslLoadSoundFile("system/media/audio/ui/KeypressStandard.wav", OSL_FMT_NONE);

	//loads our images into memory
	loadicons();
	background = oslLoadImageFilePNG("system/framework/framework-res/res/background.png", OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("system/cursor/cursor.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("system/home/icons/nav.png", OSL_IN_RAM, OSL_PF_8888);
	wificon = oslLoadImageFile("system/home/icons/wificon.png", OSL_IN_RAM, OSL_PF_8888);
	apollo = oslLoadImageFilePNG("system/home/icons/apollo.png", OSL_IN_RAM, OSL_PF_8888);
	gmail = oslLoadImageFilePNG("system/home/icons/gmail.png", OSL_IN_RAM, OSL_PF_8888);
	message = oslLoadImageFilePNG("system/home/icons/message.png", OSL_IN_RAM, OSL_PF_8888);
	browser = oslLoadImageFile("system/home/icons/browser.png", OSL_IN_RAM, OSL_PF_8888);
	google = oslLoadImageFile("system/home/icons/google.png", OSL_IN_RAM, OSL_PF_8888);
	notif = oslLoadImageFile("system/home/menu/notif.png", OSL_IN_RAM, OSL_PF_8888);
	notif2 = oslLoadImageFile("system/home/menu/notif2.png", OSL_IN_RAM, OSL_PF_8888);
	batt100 = oslLoadImageFile("system/home/icons/100.png", OSL_IN_RAM, OSL_PF_8888);
	batt80 = oslLoadImageFile("system/home/icons/80.png", OSL_IN_RAM, OSL_PF_8888);
	batt60 = oslLoadImageFile("system/home/icons/60.png", OSL_IN_RAM, OSL_PF_8888);
	batt40 = oslLoadImageFile("system/home/icons/40.png", OSL_IN_RAM, OSL_PF_8888);
	batt20 = oslLoadImageFile("system/home/icons/20.png", OSL_IN_RAM, OSL_PF_8888);
	batt10 = oslLoadImageFile("system/home/icons/10.png", OSL_IN_RAM, OSL_PF_8888);
	batt0 = oslLoadImageFile("system/home/icons/0.png", OSL_IN_RAM, OSL_PF_8888);
	battcharge = oslLoadImageFile("system/home/icons/charge.png", OSL_IN_RAM, OSL_PF_8888);
	pointer = oslLoadImageFilePNG("system/home/icons/pointer.png", OSL_IN_RAM, OSL_PF_8888);
	pointer1 = oslLoadImageFilePNG("system/home/icons/pointer1.png", OSL_IN_RAM, OSL_PF_8888);
	backicon = oslLoadImageFilePNG("system/home/icons/backicon.png", OSL_IN_RAM, OSL_PF_8888);
	homeicon = oslLoadImageFilePNG("system/home/icons/homeicon.png", OSL_IN_RAM, OSL_PF_8888);
	multicon = oslLoadImageFilePNG("system/home/icons/multicon.png", OSL_IN_RAM, OSL_PF_8888);
	usbdebug = oslLoadImageFilePNG("system/home/icons/usbdebug.png", OSL_IN_RAM, OSL_PF_8888);
	welcome = oslLoadImageFilePNG("system/home/icons/welcome.png", OSL_IN_RAM, OSL_PF_8888);
	ok = oslLoadImageFilePNG("system/home/icons/ok.png", OSL_IN_RAM, OSL_PF_8888);
	transbackground = oslLoadImageFilePNG("system/home/icons/transbackground.png", OSL_IN_RAM, OSL_PF_8888);

	//Debugger
	if (!background || !cursor || !appicon || !appicon2 || !navbar || !wificon || !apollo || !gmail || !message || !browser || !google || !notif || !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !pointer || !pointer1 || !backicon || !multicon || !homeicon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	loadConfig();

	firstBoot = 1;
	
	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	setfont();
	
	//Main loop to run the program
	while (!osl_quit)
	{	
		makescreenshotdir();
		
		//Draws images onto the screen
		oslStartDrawing();
		
		controls();
													
		//Initiate the PSP's controls
		oslReadKeys();
			
		//Print the images onto the screen
		oslDrawImage(background);		
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(google, 22, 26);
		oslDrawImageXY(apollo, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(message, 160, 190);
		oslDrawImageXY(pointer, 231, 180);
		
		digitaltime();
		
		//calls the functions	
		appdrawericon();
		battery();
		navbar_buttons();
		android_notif();
		firstBootMessage();
		oslDrawImage(cursor);
		
		if (osl_pad.held.square)
		{
			 powermenu();
		}
		
		//Launching the browser
		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_pad.held.cross)
		{
			unloadicons();
			internet();
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

