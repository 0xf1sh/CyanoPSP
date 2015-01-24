#include <pspkernel.h>
#include <pspctrl.h>
#include <pspumd.h>

//PSP Net Stuff
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>

//OSLib
#include <oslib/oslib.h>

//File Management
#include <pspiofilemgr.h>
#include <pspiofilemgr_kernel.h>
#include <pspiofilemgr_dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h> 
#include <stdlib.h> 

#include "include/pgeZip.h"
#include "include/ram.h"
#include "include/utils.h"
#include "fm.h"
#include "settingsmenu.h"
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "recoverymenu.h"
#include "screenshot.h"

#define configFile "system/build.prop"
#define Address "www.google.com"
#define MAX 8

//declaration
OSL_IMAGE *settingsbg, *cursor, *usbdebug, *aboutbg, *offswitch, *onswitch, *themebg, *performancebg, *wifibg, *developerbg, *about, *highlight, 
		  *developeroptions, *themes, *wifi, *processorbg, *background, *appicon1, *appicon2, *navbar, *apollo, *gmail, *messengericon, *browser, *cpuset, 
		  *check, *backicon, *homeicon, *multicon, *calc, *clockx, *email, *people, *calendar, *phone, *gallery, *isoloadericon, *fb, *settings, *updatesbg, 
		  *performance, *recoverybg, *easterEggImg;

//definition of our sounds
OSL_SOUND *tone;

char name;
int setclock;
char Version[10] = "3.0";
char lang[12] = "Uk English";
static char Settings_message[100] = "";

const int cpu_list[] = { 20, 75, 100, 133, 166, 222, 266, 300, 333 };
const int bus_list[] = { 10, 37, 50, 66, 83, 111, 133, 150, 166 };
int current = 0;

void onlineUpdater()
{
	int skip = 0;
    int browser = 0;
	char message[100] = "";
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!recoverybg)
		debugDisplay();
	
	oslNetInit();

    while(!osl_quit)
	{
        browser = oslBrowserIsActive();
		if (!skip)
		{
            oslStartDrawing();
			
            if (browser)
			{
                oslDrawBrowser();
                if (oslGetBrowserStatus() == PSP_UTILITY_DIALOG_NONE)
				{
                    oslEndBrowser();
					oslDrawImageXY(recoverybg, 0, 0);
					oslDrawStringf(10,10,"Flashing zip...");
					if (fileExists("ms0:/PSP/GAME/CyanogenMod.zip"))
					{
					pgeZip* zipFiles = pgeZipOpen("../CyanogenMod.zip");
					chdir("..");
					pgeZipExtract(zipFiles, NULL);
					pgeZipClose(zipFiles);
					oslDrawStringf(10,30,"Installed Successfully.");
					oslDrawStringf(10,50,"Exiting..");
					oslSyncFrame();
					sceKernelDelayThread(2*1000000);
					oslSyncFrame();
					oslDrawStringf(10,50,"Enjoy :)");
					sceKernelDelayThread(3*1000000);
					oslDeleteImage(recoverybg);
					sceIoRemove("ms0:/PSP/GAME/CyanogenMod/system/build.prop");
					sceKernelExitGame();
					}
					oslDeleteImage(recoverybg);
					aboutMenu();
                }
            }
            oslEndDrawing();
		}
		oslEndFrame();
		skip = oslSyncFrame();

        if (!browser)
		{
            oslReadKeys();
            int res = oslBrowserInit("http://zone-wifi.fr/psp/PSP/GAME/CyanogenMod.zip", "/PSP/GAME", 5*1024*1024,
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
    }
	oslNetTerm();
}
	
void wlanStatus(int n)
{
	if (n == 1)
	{
		if (sceWlanGetSwitchState() == 0)
			oslDrawImageXY(offswitch, 370, 62);
	
		else
			oslDrawImageXY(onswitch, 370, 62);
	}
	
	else if (n == 2)
	{
		if (sceWlanGetSwitchState() == 0)
			oslDrawImageXY(offswitch, 222, 14);
		
		else
			oslDrawImageXY(onswitch, 222, 14);
	}
}

void loadConfig()
{	
	//load/open file
	FILE * configtxt = fopen(configFile, "rb");

	if(!configtxt) 
		return saveConfig();
	
	//close file
	fclose(configtxt);
}

void saveConfig()
{
	FILE * configtxt = fopen(configFile, "wb"); //create config file if it doesn't exist.
	fprintf(configtxt,
"ro.build.version.release = %s\r\n\
ro.product.model = %d\r\n\
ro.product.locale.language = %s\r\n\
ro.build.user = Joel16\r\n\
ro.product.cpu.frequency =  %d\r\n\
ro.product.bus.frequency =  %d\r\n\
ro.build.date = Fri Oct 31 11:20 PM EST 2014",
	Version, kuKernelGetModel(),lang,getCpuClock(),getBusClock());
	fclose(configtxt);	
}

void changer(int set) 
{
	switch (set) 
	{
		case 0:
		scePowerSetClockFrequency(cpu_list[set], cpu_list[set], bus_list[set]);
		break;
	}
}

int getCpuClock()
{
    return scePowerGetCpuClockFrequency();
}

int getBusClock()
{
    return scePowerGetBusClockFrequency();
}

void pspGetCpuBus()
{	
	if(getCpuClock() == 20 && getBusClock() == 10)
	{
		oslDrawString(35,87,"20/10");
	}
   
	else if(getCpuClock() == 75 && getBusClock() == 37)
	{
		oslDrawString(35,87,"75/37");
	}
		
	else if(getCpuClock() == 100 && getBusClock() == 50)
	{
		oslDrawString(35,87,"100/50");
	}
		
	else if(getCpuClock() == 133 && getBusClock() == 66)
	{
		oslDrawString(35,87,"133/66");
	}
		
	else if(getCpuClock() == 166 && getBusClock() == 83)
	{
		oslDrawString(35,87,"166/83");
	}
		
	else if(getCpuClock() == 200 && getBusClock() == 100)
	{
		oslDrawString(35,87,"200/100");
	}
		
	else if(getCpuClock() == 222 && getBusClock() == 111)
	{
		oslDrawString(35,87,"222/111");
	}
		
	else if(getCpuClock() == 266 && getBusClock() == 133)
	{
		oslDrawString(35,87,"266/133");
	}
		
	else if(getCpuClock() == 300 && getBusClock() == 150)
	{
		oslDrawString(35,87,"300/150");
	}
		
	else if(getCpuClock() == 333 && getBusClock() == 166)
	{
		oslDrawString(35,87,"333/166");
	}
}

void pspGetModel(int x, int y)
{
	char pspmodel = kuKernelGetModel();
	
	if(pspmodel == 0)
	{
		oslDrawString(x,y,"Model: PSP 1000");
	}
   
	else if(pspmodel == 1)
	{
		oslDrawString(x,y,"Model: PSP 2000");
	}
   
	else if (pspmodel == 2)
	{
		oslDrawString(x,y,"Model: PSP 3000");
	}
   
	else if(pspmodel == 3)
	{
		oslDrawString(x,y,"Model: PSP 3000");
	}
		
	else if (pspmodel == 4)
	{
		oslDrawString(x,y,"Model: PSP Go N1000");
	}
   
	else
	{
		oslDrawString(x,y,"Model: PS Vita");
	}
}

void aboutMenu()
{	
	aboutbg = oslLoadImageFilePNG("system/settings/aboutbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!aboutbg || !highlight)
		debugDisplay();
	
	setfont();
	
	int n = 0;

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(aboutbg, 0, 19);

		oslDrawString(37,73,"CyanoPSP Updates");
		oslDrawString(37,87,"Click for, view or install available updates");
		pspGetModel(37,133);
		oslDrawStringf(37,119,"CyanoPSP: %s",Version);
		oslDrawString(37,147,"Build Date - Mon Dec 8 3:10 PM EST 2014");
		oslDrawString(37,172,"Kernel Version");
		oslDrawString(37,186,"Undefined-pspsdk_oslib");
		oslDrawString(37,200,"joellovesanna@psp #1");
		
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 45 && cursor->y <= 90)
		{
			oslDrawImageXY(highlight, 15, 54);
			oslDrawString(37,73,"CyanoPSP Updates");
			oslDrawString(37,87,"Click for, view or install available updates");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 95 && cursor->y <= 151)
		{
			oslDrawImageXY(highlight, 15, 110);
			pspGetModel(37,133);
			oslDrawStringf(37,119,"CyanoPSP: %s",Version);
			oslDrawString(37,147,"Build Date - Mon Dec 8 3:10 PM EST 2014");
		}
		
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			settingsMenu();	
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			settingsMenu();	
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			home();	
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 50 && cursor->y <= 94 && osl_keys->pressed.cross)
		{
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			updatesMenu();
		}
		
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 95 && cursor->y <= 151 && osl_keys->pressed.cross)
		{
			if  (osl_keys->pressed.cross)
				n+=1;
			if (n == 4)
			{
				oslDeleteImage(aboutbg);
				oslDeleteImage(highlight);
				easterEgg();
			}
		}
		
		if(osl_keys->pressed.select)
		{
			enableUsb();
		}
		
		else if(osl_keys->pressed.select)
		{	
			disableUsb();
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

void easterEgg()
{
	easterEggImg = oslLoadImageFilePNG("system/settings/easteregg.png", OSL_IN_RAM, OSL_PF_8888);
		
	if (!easterEggImg)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));
		controls();
		oslDrawImageXY(easterEggImg, 0, 0);
		
		if(osl_keys->pressed.circle)
		{
			oslDeleteImage(easterEggImg);
			aboutMenu();
		}
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
}

void updatesMenu()
{		    
	updatesbg = oslLoadImageFilePNG("system/settings/updatesbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	if (!updatesbg || !highlight)
		debugDisplay();
	
	setfont();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(updatesbg, 0, 19);
		
		oslDrawString(35,73,"Check for Updates");
				
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 45 && cursor->y <= 90)
		{
			oslDrawImageXY(highlight, 16, 54);
			oslDrawString(37,73,"Check for Updates");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 45 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			onlineUpdater();
		}
		
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			aboutMenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			aboutMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
	oslNetTerm();
}

void performanceMenu()
{	
	performancebg = oslLoadImageFilePNG("system/settings/performancebg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	if (!performancebg || !highlight)
		debugDisplay();
	
	setfont();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(performancebg, 0, 19);

		oslDrawString(40,98,"Processor");
		oslDrawString(40,161,"Ram Management");
		oslDrawString(40,215,"Memory Management");
		
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 80 && cursor->y <= 138)
		{
			oslDrawImageXY(highlight, 15, 83);
			oslDrawString(40,98,"Processor");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 140 && cursor->y <= 196)
		{
			oslDrawImageXY(highlight, 15, 139);
			oslDrawString(40,161,"Ram Management");
		}
		
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			settingsMenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			settingsMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 80 && cursor->y <= 138 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			processorMenu();
		}

		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 140 && cursor->y <= 196 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			ramMenu();
		}
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
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

void wait_release(unsigned int buttons) 
{ 
    SceCtrlData pad; 
    sceCtrlReadBufferPositive(&pad, 1); 
	
    while (pad.Buttons & buttons) 
    { 
        sceKernelDelayThread(100000); 
        sceCtrlReadBufferPositive(&pad, 1); 
    } 
} 

unsigned int wait_press(unsigned int buttons) 
{ 
    SceCtrlData pad; 
	sceCtrlReadBufferPositive(&pad, 1); 
    
	while (1) 
    { 
        if (pad.Buttons & buttons) 
            return pad.Buttons & buttons; 
        sceKernelDelayThread(100000); 
        sceCtrlReadBufferPositive(&pad, 1); 
    } 
    return 0;   /* never reaches here, again, just to suppress warning */ 
} 

void set_cpu_clock(int n)
{
	if(n==0)
		scePowerSetClockFrequency(222,222,111);
	else if(n==1)
		scePowerSetClockFrequency(266,266,133);
	else if(n==2)
		scePowerSetClockFrequency(333,333,166);
}

void processorMenu()
{	
	int timer;

	processorbg = oslLoadImageFilePNG("system/settings/processorbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!processorbg || !highlight)
		debugDisplay();

	setfont();
	
	sceCtrlSetSamplingCycle(0); 
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL); 
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		sceCtrlPeekBufferPositive(&pad, 1);
		
		oslDrawImageXY(processorbg, 0, 19);
		
		oslDrawString(35,74,"Current CPU Frequency");
		oslDrawString(35,128,"CPU Overclock");
		oslDrawString(35,184,"Minimum CPU Frequency");
		oslDrawString(35,197,"20 MHz");
		oslDrawString(35,236,"Maximum CPU Frequency");
		oslDrawString(35,249,"333 MHz");
		
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 118 && cursor->y <= 174)
		{
			oslDrawImageXY(highlight, 16, 118);
			oslDrawString(35,128,"CPU Overclock");
			oslDrawString(35,142,"Press R to increase frequency and L to decrease frequency");
			oslDrawString(35,156,"Press triangle to reset to default, 222/111");
		}

		if (osl_keys->pressed.triangle)
		{
		 setclock = 6;
		 scePowerSetClockFrequency(222, 222, 111);
		}
				
		if (current < 0)
		{
			current = MAX;
		}
		if (current > MAX)
		{
			current = 0;
		}
		
		if (osl_keys->pressed.L)
		{
		set_cpu_clock(1);
		}
		if (osl_keys->pressed.R)
		{
		set_cpu_clock(2);
		}
		
		oslDrawStringf(35,87,"%d/%d",cpu_list[current],bus_list[current]);
		
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			performanceMenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			performanceMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
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

void ramMenu()
{	
	performancebg = oslLoadImageFilePNG("system/settings/performancebg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!performancebg || !highlight)
		debugDisplay();

	setfont();
	
	sceCtrlSetSamplingCycle(0); 
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL); 
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		sceCtrlPeekBufferPositive(&pad, 1);
		
		oslDrawImageXY(performancebg, 0, 19);
		
		oslDrawStringf(40,98,"RAM Available: %d MB Available\n",oslGetRamStatus().maxAvailable/1000000); 
	
		navbarButtons();
		digitaltime(420,4,458);
		battery();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
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

/* Work in Progress Theme Manager */

/*

int ThemeZip;
int theme;
char defaultTheme;
char defaultZip;
char InputTheme;
char theme_bootanim[10] = "";
char theme_icons[10] = "";
char theme_style[10] = "";
char theme_fonts[10] = "";

void ThemePackLocation()
{
        defaultTheme = "Themes/Original.zip";
        InputTheme = "Themes/Theme.zip";
}

void GetTheme()
{
        switch(theme)
        {
                case navbar:
                {
                        return GetTextureFromZip("nav.png");
                }
                break;
                case appicon1:
                {
                        return GetTextureFromZip("appicon1.png");
                }
                break;
                case appicon2:
                {
                        return GetTextureFromZip("appicon2.png");
                }
                break;
                case backicon:
                {
                        return GetTextureFromZip("backicon.png");
                }
                break;
                case homeicon:
                {
                        return GetTextureFromZip("homeicon.png");
                }
                break;
                case multicon:
                {
                        return GetTextureFromZip("multicon.png");
                }
                break;
                case wificon:
                {
                        return GetTextureFromZip("wificon.png");
                }
                break;
                case apollo:
                {
                        return GetTextureFromZip("apollo.png");
                }
                break;
                case browser:
                {
                        return GetTextureFromZip("browser.png");
                }
                break;
                case calc:
                {
                        return GetTextureFromZip("calc.png");
                }
                break;
                case calendar:
                {
                        return GetTextureFromZip("calendar.png");
                }
                break;
				case clock:
                {
                        return GetTextureFromZip("clock.png");
                }
                break;
                case email:
                {
                        return GetTextureFromZip("email.png");
                }
                break;
				case fb:
                {
                        return GetTextureFromZip("fb.png");
                }
                break;
                case gallery:
                {
                        return GetTextureFromZip("gallery.png");
                }
                break;
				case gmail:
                {
                        return GetTextureFromZip("gmail.png");
                }
                break;
                case isoloadericon:
                {
                        return GetTextureFromZip("isoloadericon.png");
                }
                break;
				case messengericon:
                {
                        return GetTextureFromZip("messengericon.png");
                }
                break;
                case people:
                {
                        return GetTextureFromZip("people.png");
                }
                break;
				case phone:
                {
                        return GetTextureFromZip("phone.png");
                }
                break;
                case settings:
                {
                        return GetTextureFromZip("settings.png");
                }
                break;
        }

        return "";
}

void SetZipName()
{
        defaultZip = name;
        InputTheme = defaultTheme + defaultZip;

        if(ThemeZip != NULL)
        {
                pgeZipClose(ThemeZip);
        }
        ThemeZip = pgeZipOpen(InputTheme);
}

*/

void displayMenu()
{	
	themebg = oslLoadImageFilePNG("system/settings/themebg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!themebg)
		debugDisplay();

	setfont();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(themebg, 0, 19);

		oslDrawString(65,74,"Themes");
		oslDrawString(65,128,"Styles");
		oslDrawString(65,184,"Icons");
		oslDrawString(65,236,"Fonts");
		
		digitaltime(420,4,458);

		battery();
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(themebg);
			settingsMenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(themebg);
			settingsMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(themebg);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
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

void wifiMenu()
{	
	wifibg = oslLoadImageFilePNG("system/settings/wifibg.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

	if (!wifibg)
		debugDisplay();
	
	setfont();

	int enabled = 1;
    int selectedConfig = 0;
	int wifi_y = 50;
	static char buffer[100] = "";
	
	//Get connections list:
    struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
    int numconfigs = oslGetNetConfigs(configs);
	if (!numconfigs)
	{
        sprintf(Settings_message, "No WiFi configuration found");
        enabled = 0;
    }
	
	oslNetInit(); 
	
	if (!oslIsWlanPowerOn())
        sprintf(Settings_message, "Please turn on the Wlan switch.");

	while (!osl_quit)
	{			
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(wifibg, 0, 19);

		if (enabled)
		{
			sprintf(buffer, "%s", configs[selectedConfig].name);
    		oslDrawString(30, wifi_y+28, configs[selectedConfig].name);
			oslDrawString(30, 195, "Press up or down to navigate through your Wifi configurations.");
        }
		
		oslDrawString(30, 200, Settings_message);
		
		wlanStatus(2);
		digitaltime(420,4,458);

		battery();
		navbarButtons();
		androidQuickSettings();
		
		if (osl_keys->released.cross)
		{
			connectToAP(selectedConfig + 1);
        }
		else if (osl_keys->released.up)
		{
			if (++selectedConfig >= numconfigs)
				selectedConfig = numconfigs - 1;
        } 
		else if (osl_keys->released.down)
		{
			 if (--selectedConfig < 0)
				selectedConfig = 0;
        }
		
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);	
			settingsMenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}	
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
	oslNetTerm();
}

/* Build a path, append a directory slash if requested */
void build_path(char *output, const char *root, const char *path, int append)
{
	while(*root != 0)
	{
		*output++ = *root++;
	}

	if(*(root-1) != '/')
	{
		*output++ = '/';
	}

	while(*path != 0)
	{
		*output++ = *path++;
	}
	if(append)
		*output++ = '/';

	*output++ = 0;
}

/* Define a write buffer */
char write_buffer[128*1024];

void write_file(const char *read_loc, const char *write_loc, const char *name)
{
	int fdin;
	int fdout;
	char readpath[256];
	char writepath[256];

	build_path(readpath, read_loc, name, 0);
	build_path(writepath, write_loc, name, 0);
	printf("Writing %s\n", writepath);

	fdin = sceIoOpen(readpath, PSP_O_RDONLY, 0777);
	if(fdin >= 0)
	{
		int bytesRead = 1;
		fdout = sceIoOpen(writepath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		if(fdout < 0)
		{
			printf("Couldn't open %s\n", writepath);
		}

		bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		while((bytesRead > 0) && (fdout >= 0))
		{
			sceIoWrite(fdout, write_buffer, bytesRead);
			bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		}

		if(fdout >= 0)
		{
			sceIoClose(fdout);
		}

		if(fdin >= 0)
		{
			sceIoClose(fdin);
		}
	}
	else
	{
		printf("Couldn't open %s\n", readpath);
	}
}

void DumpBootBin(void)
{
	int i;
	int fd;

	i = sceUmdCheckMedium();
	if(i == 0)
	{
		oslDrawString(4,5,"Insert UMD");
		i = sceUmdWaitDriveStat(PSP_UMD_PRESENT);
	}

	i = sceUmdActivate(1, "disc0:");
	oslDrawString(4,5,"Mounted disc");
	oslSyncFrame();
	sceKernelDelayThread(2*1000000);

	i = sceUmdWaitDriveStat(PSP_UMD_READY);

	/* Open the UMD_DATA.BIN */
	fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
	if(fd >= 0)
	{
		char game_id[11];
		char path[256];

		sceIoRead(fd, game_id, 10);
		sceIoClose(fd);
		game_id[10] = 0;
		build_path(path, "ms0:/", game_id, 0);
		sceIoMkdir(path, 0777);

		oslDrawStringf(4,15,"Found game %s\n", game_id);
		write_file("disc0:/PSP_GAME/SYSDIR", path, "BOOT.BIN");
		oslSyncFrame();
		sceKernelDelayThread(3*1000000);
	}
}

/* Dump a filing system */
void dump_filesystem(const char *root, const char *write_loc)
{
	int dfd;
	char next_root[256];
	char next_write[256];

	sceIoMkdir(write_loc, 0777);

	dfd = sceIoDopen(root);
	if(dfd > 0)
	{
		SceIoDirent dir;

		while(sceIoDread(dfd, &dir) > 0)
		{
			if(dir.d_stat.st_attr & FIO_SO_IFDIR)
			{
				if(dir.d_name[0] != '.')
				{
					build_path(next_write, write_loc, dir.d_name, 0);
					build_path(next_root, root, dir.d_name, 1);
					dump_filesystem(next_root, next_write);
				}
			}
			else
			{
				write_file(root, write_loc, dir.d_name);
			}
		}
		sceIoDclose(dfd);
	}
}

/* Dump memory */
void dump_memory(void)
{
	int fd;

	oslDrawString(4,5,"Dumping 28Megs from 0x8400000\n");
	fd = sceIoOpen("ms0:/MEMORY.BIN", PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);
	if(fd >= 0)
	{
		sceIoWrite(fd, (void *) 0x8400000, 28*1024*1024);
		sceIoClose(fd);
	}
	oslSyncFrame();
	sceKernelDelayThread(3*1000000);
}

void dumpMenu()
{
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	setfont();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(developerbg, 0, 19);
		
		oslDrawString(35,76,"Dump UMD boot.bin");
		oslDrawString(35,128,"Dump Flash 0");
		oslDrawString(35,188,"Dump Flash 1");
		oslDrawString(35,236,"Dump Memory");
			
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 55 && cursor->y <= 108)
		{
			oslDrawImageXY(highlight, 15, 55);
			oslDrawString(35,76,"Dump UMD boot.bin");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 109 && cursor->y <= 165)
		{
			oslDrawImageXY(highlight, 15, 109);
			oslDrawString(35,128,"Dump Flash 0");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 166 && cursor->y <= 221)
		{
			oslDrawImageXY(highlight, 15, 166);
			oslDrawString(35,188,"Dump Flash 1");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 222 && cursor->y <= 278)
		{
			oslDrawImageXY(highlight, 15, 222);
			oslDrawString(35,236,"Dump Memory");
		}
		
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			developerMenu();
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			developerMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 55 && cursor->y <= 108 && osl_keys->pressed.cross)
		{	
			DumpBootBin();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 109 && cursor->y <= 165 && osl_keys->pressed.cross)
		{
			dump_filesystem("flash0:/", "ms0:/flash0");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 166 && cursor->y <= 221 && osl_keys->pressed.cross)
		{
			dump_filesystem("flash1:/", "ms0:/flash1");
		}

		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 222 && cursor->y <= 278 && osl_keys->pressed.cross)
		{
			dump_memory();
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

void developerMenu()
{
	int RJL;
	int PSPDebug;
	
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	check = oslLoadImageFilePNG("system/settings/check.png", OSL_IN_RAM, OSL_PF_8888);

	if (!developerbg || !highlight || !check)
		debugDisplay();
	
	setfont();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(developerbg, 0, 19);
		oslDrawImageXY(check, 422, 177);
		
		oslDrawString(35,62,"Toggle Remote Joy Lite");
		oslDrawString(35,76,"Displays your PSP screen on your computer via USB.");
		oslDrawString(35,90,"Press Triangle to disable or it may cause the program to crash.");
		oslDrawString(35,128,"Toggle USB Debugging");
		oslDrawString(35,142,"Press Triangle to disable or it may cause the program to crash");
		oslDrawString(35,174,"Advanced Reboot");
		oslDrawString(35,188,"When unlocked, include option in the power menu for");
		oslDrawString(35,202,"rebooting into recovery");
		oslDrawString(35,236,"Dumping Tools");
			
		digitaltime(420,4,458);
		battery();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 55 && cursor->y <= 108)
		{
			oslDrawImageXY(highlight, 15, 55);
			oslDrawString(35,62,"Toggle Remote Joy Lite");
			oslDrawString(35,76,"Displays your PSP screen on your computer via USB.");
			oslDrawString(35,90,"Press Triangle to disable or it may cause the program to crash.");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 109 && cursor->y <= 165)
		{
			oslDrawImageXY(highlight, 15, 109);
			oslDrawString(35,128,"Toggle USB Debugging");
			oslDrawString(35,142,"Press Triangle to disable or it may cause the program to crash.");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 222 && cursor->y <= 278)
		{
			oslDrawImageXY(highlight, 15, 222);
			oslDrawString(35,236,"Dumping Tools");
		}
		
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			oslDeleteImage(highlight);
			settingsMenu();
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			oslDeleteImage(highlight);
			settingsMenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			oslDeleteImage(highlight);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 55 && cursor->y <= 108 && osl_keys->pressed.cross)
		{	
			RJL = 1;
			LoadStartModule("modules/RemoteJoyLite.prx");
		}
		if(RJL == 1 && osl_keys->pressed.triangle)
		{
			StopUnloadModule("modules/RemoteJoyLite.prx");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 109 && cursor->y <= 165 && osl_keys->pressed.cross)
		{
			PSPDebug = 1;
			LoadStartModule("modules/psplink.prx");
		}
		if(PSPDebug == 1  && osl_keys->pressed.triangle)
		{	
			StopUnloadModule("modules/psplink.prx");			
		}

		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 222 && cursor->y <= 278 && osl_keys->pressed.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			oslDeleteImage(highlight);
			dumpMenu();
		}
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}	
}

void settingsHighlight()
{
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 53 && cursor->y <= 98)
		{
			oslDrawImageXY(wifi, 16, 63);
			oslDrawString(55,76,"Wi-Fi");
			wlanStatus(1);
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 99 && cursor->y <= 141)
		{
			oslDrawImageXY(developeroptions, 16, 105);
			oslDrawString(55,118,"Developer Options");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 142 && cursor->y <= 183)
		{
			oslDrawImageXY(themes, 16, 148);
			oslDrawString(55,161,"Display");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 184 && cursor->y <= 227)
		{
			oslDrawImageXY(performance, 16, 190);
			oslDrawString(55,204,"Performance");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 228 && cursor->y <= 250)
		{
			oslDrawImageXY(about, 15, 232);
			oslDrawString(55,246,"About PSP");
		}
}

void settingsDeleteResources()
{
	oslDeleteImage(settingsbg);
	oslDeleteImage(about);
	oslDeleteImage(themes);
	oslDeleteImage(developeroptions);
	oslDeleteImage(wifi);
	oslDeleteImage(offswitch);
	oslDeleteImage(onswitch);
	oslDeleteImage(performance);
}

void settingsMenu()
{
	settingsbg = oslLoadImageFilePNG("system/settings/settingsbg.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);
	about = oslLoadImageFilePNG("system/settings/about.png", OSL_IN_RAM, OSL_PF_8888);
	themes = oslLoadImageFilePNG("system/settings/themes.png", OSL_IN_RAM, OSL_PF_8888);
	developeroptions = oslLoadImageFilePNG("system/settings/developeroptions.png", OSL_IN_RAM, OSL_PF_8888);
	wifi = oslLoadImageFilePNG("system/settings/wifi.png", OSL_IN_RAM, OSL_PF_8888);
	performance = oslLoadImageFilePNG("system/settings/performance.png", OSL_IN_RAM, OSL_PF_8888);
		
	setfont();

	if (!settingsbg || !onswitch || !offswitch || !about || !developeroptions || !wifi || !themes || !performance)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(settingsbg, 0, 19);

		wlanStatus(1);
		oslDrawString(55,76,"Wi-Fi");
		oslDrawString(55,118,"Developer Options");
		oslDrawString(55,161,"Display");
		oslDrawString(55,204,"Performance");
		oslDrawString(55,246,"About PSP");
		
		digitaltime(420,4,458);
		battery();
		settingsHighlight();
		navbarButtons();
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.select)
		{
			enableUsb();
		}
		else if (osl_keys->pressed.select)
		{
			disableUsb();
		}
		
		if (osl_keys->pressed.circle)
		{	
			settingsDeleteResources();
			appdrawer();
		}
			
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			settingsDeleteResources();
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			settingsDeleteResources();
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			multitask();
		}
			
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 53 && cursor->y <= 98 && osl_keys->pressed.cross)
		{	
			settingsDeleteResources();
			wifiMenu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 99 && cursor->y <= 141 && osl_keys->pressed.cross)
		{	
			settingsDeleteResources();
			developerMenu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 142 && cursor->y <= 183 && osl_keys->pressed.cross)
		{	
			settingsDeleteResources();
			displayMenu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 184 && cursor->y <= 227 && osl_keys->pressed.cross)
		{
			settingsDeleteResources();
			performanceMenu();
		}
				
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 228 && cursor->y <= 250 && osl_keys->pressed.cross)
		{	
			settingsDeleteResources();
			aboutMenu();
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