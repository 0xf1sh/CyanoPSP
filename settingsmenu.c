#include <pspkernel.h>
#include <pspctrl.h>

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

#include "include/pgeZip.h"
#include "fm.h"
#include "settingsmenu.h"

#define FW_660 0x06060010
#define FW_639 0x06030910
#define FW_635 0x06030510
#define FW_620 0x06020010
#define FW_610 0x06010010
#define FW_600 0x06000010
#define FW_551 0x05050110
#define FW_550 0x05050010
#define FW_503 0x05000310
#define FW_501 0x05000110
#define FW_500 0x05000010
#define FW_401 0x04000110
#define FW_396 0x03090610
#define FW_395 0x03090510
#define FW_393 0x03090310
#define FW_390 0x03090010
#define FW_380 0x03080010
#define FW_372 0x03070210
#define FW_371 0x03070110
#define FW_352 0x03050210
#define FW_351 0x03050110
#define FW_350 0x03050010
#define FW_340 0x03040010
#define FW_330 0x03030010
#define FW_311 0x03010110
#define FW_310 0x03010010
#define FW_303 0x03000310
#define FW_302 0x03000210

#define configFile "config.TXT"
#define Address "www.google.com"
#define MAX 8

//declaration
OSL_IMAGE *settingsbg, *cursor, *wificon, *usbdebug, *aboutbg, *offswitch, *onswitch, *themebg, *performancebg, *wifibg, *developerbg, *about, *highlight, 
		  *developeroptions, *themes, *wifi, *processorbg, *background, *appicon1, *appicon2, *navbar, *apollo, *gmail, *message, *browser, *cpuset, *check,
		  *backicon, *homeicon, *multicon, *calc, *clockx, *email, *people, *calendar, *phone, *gallery, *isoloadericon, *fb, *settings, *updatesbg, *performance;

//definition of our sounds
OSL_SOUND *tone;

int usb_debug = 0;
char pspmodel;
char usbStatus = 0;
char usbModuleStatus = 0;
char defaultTheme;
char defaultZip;
char InputTheme;
int ThemeZip;
int theme;
char name;
int setclock;
int setclockrlimit = 0;
int setclockllimit = 9;
int RJL = 0;
int PSPDebug = 0;
char Version[10] = "2.0 Alpha";
char lang[12] = "Uk English";
char theme_bootanim[10] = "";
char theme_icons[10] = "";
char theme_style[10] = "";
char theme_fonts[10] = "";
static char Settings_message[100] = "";
static char buffer[100] = "";

int OnlineUpdater();
int connectAPCallback(int state);
int connectToAP(int config);

const int cpu_list[] = { 20, 75, 100, 133, 166, 222, 266, 300, 333 };
const int bus_list[] = { 10, 37, 50, 66, 83, 111, 133, 150, 166 };
int current = 0;

int connectAPCallback(int state){
    oslStartDrawing();
    oslDrawString(30, 200, "Connecting to AP...");
    sprintf(buffer, "State: %i", state);
    oslDrawString(30, 230, buffer);
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

    return 0;
}

int connectToAP(int config){
    oslStartDrawing();
    oslDrawString(30, 200, "Connecting to AP...");
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

    int result = oslConnectToAP(config, 30, connectAPCallback);
    if (!result){
        char ip[30] = "";
        char resolvedIP[30] = "";

        oslStartDrawing();
        oslGetIPaddress(ip);
        sprintf(buffer, "IP address: %s", ip);
        oslDrawString(30, 170, buffer);

        sprintf(buffer, "Resolving %s", Address);
        oslDrawString(30, 200, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();

        result = oslResolveAddress(Address, resolvedIP);

        oslStartDrawing();
        oslGetIPaddress(ip);
        if (!result)
            sprintf(buffer, "Resolved IP address: %s", ip);
        else
            sprintf(buffer, "Error resolving address.");
        oslDrawString(30, 230, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }else{
        oslStartDrawing();
        sprintf(buffer, "Error connecting to AP.");
        oslDrawString(200, 200, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }
    oslDisconnectFromAP();
    return 0;
}	

int OnlineUpdater()
{
	int skip = 0;
    int enabled = 1;
    int selectedConfig = 0;
	int download = 0;
    
    oslNetInit();
    
    if (!oslIsWlanPowerOn())
        sprintf(Settings_message, "Please turn on the WLAN.");
    
    //Get connections list:
    struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
    int numconfigs = oslGetNetConfigs(configs);
    if (!numconfigs){
        sprintf(Settings_message, "No configuration found");
        enabled = 0;
    }
    
    while(!osl_quit){
        if (!skip){
                oslStartDrawing();
        
			if (enabled){
					oslDrawStringf(25,127,buffer, "Press X to connect to %s.", configs[selectedConfig].name);
					oslDrawString(25,113, buffer);
					oslDrawString(25, 141, "Press UP and DOWN to change settings.");
						}
        
					oslDrawString(25, 155, "Press trianlge to cancel.");
					oslDrawString(30, 200, Settings_message);

					oslEndDrawing();
				}
        oslEndFrame();
        skip = oslSyncFrame();

        oslReadKeys();
 
        if (osl_keys->released.triangle)
		{
            break;
		}
        if (osl_keys->released.cross)
		{
        connectToAP(selectedConfig + 1);
		download = 1;
		
		if (download == 1)
		{
			//download file
        oslNetGetFile("http://zone-wifi.fr/psp/PSP/GAME/CyanogenMod.zip", "../");

        oslDrawStringf(25, 80,"Installing update...");
    
        pgeZip* zipFiles = pgeZipOpen("../CyanogenMod.zip");
        
        chdir("..");
        
        pgeZipExtract(zipFiles, NULL);
        pgeZipClose(zipFiles);
		
		oslDrawStringf(25,90,"Installation done.");
		}
        }
		
		else if (osl_keys->released.up){
            if (++selectedConfig >= numconfigs)
                selectedConfig = numconfigs - 1;
        }else if (osl_keys->released.down){
            if (--selectedConfig < 0)
                selectedConfig = 0;
        }
    }
    
    oslNetTerm();
    
    return 1;
}

	
void wlanstatus()
{
	if (sceWlanGetSwitchState() == 0)
	oslDrawImageXY(offswitch, 370, 62);
	
	else
	oslDrawImageXY(onswitch, 370, 62);
	
}

void wlanstatus1()
{
	if (sceWlanGetSwitchState() == 0)
	oslDrawImageXY(offswitch, 222, 14);
	
	else
	oslDrawImageXY(onswitch, 222, 14);
	
}

void loadConfig()
{	
	FILE * configtxt = fopen(configFile, "rb");

	if(!configtxt) 
	return saveConfig();
	
	//close file
	fclose(configtxt);
}

void saveConfig()
{
	FILE * configtxt = fopen(configFile, "wb"); //create config file
	fprintf(configtxt,
"\r\Version= %s\r\n\
Language = %s\r\n\
ClockFrequency =  %d\r\n\
BootAnimation = %s\r\n\
Fonts = %s\r\n\
Style =  %s\r\n\
Icons = %s\r\n\
First boot = %d\r\n",
	Version,lang,setclock,theme_bootanim,theme_fonts,theme_style,theme_icons,firstBoot);

	fclose(configtxt);	

}

void changer(int set) {
	switch (set) {
	case 0:
	scePowerSetClockFrequency(cpu_list[set], cpu_list[set], bus_list[set]);
	break;
			}
}

int getCpuClock(){
    return scePowerGetCpuClockFrequency();
}

int getBusClock(){
    return scePowerGetBusClockFrequency();
}

void pspgetcpu_bus()
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

int disableUsb(void)
{
   if(usbStatus)
   {
      sceUsbDeactivate(0);
      pspUsbDeviceFinishDevice();
      sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
      sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
      usbStatus = 0;
      sceKernelDelayThread(300000);
   }
   return 0;
}

int enableUsb()
{
   if (usbStatus == 1)
   {
      disableUsb();
      return 0;
   }

   if(!usbModuleStatus)
   {
      pspSdkLoadStartModule("flash0:/kd/semawm.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstor.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstormgr.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstorms.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstorboot.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
      usbModuleStatus = 1;
   }

   if (!usbStatus)
   {
   sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
   sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
   sceUsbstorBootSetCapacity(0x800000);
   sceUsbActivate(0x1c8);
   usbStatus = 1;
   sceKernelDelayThread(300000);
   }
   return 1;
}

int enableUsbEx(u32 device)
{

   if (usbStatus == 1)
   {
      disableUsb();
      return 0;
   }

   if(!usbModuleStatus)
   {
      pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/semawm.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstor.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstormgr.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstorms.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbstorboot.prx", PSP_MEMORY_PARTITION_KERNEL);
      pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
      usbModuleStatus = 1;
   }
   if (!usbStatus)
   {
   pspUsbDeviceSetDevice(device, 0, 0);
   sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
   sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
   sceUsbActivate(0x1c8);
   usbStatus = 1;
   sceKernelDelayThread(300000);
   }
   return 1;
}

void usb_icon()
{
	if (usb_debug == 1)
	{
		oslDrawImageXY(usbdebug, 10, 1);
	}
}

void pspgetmodel()
{
		pspmodel = kuKernelGetModel();
	
		if(pspmodel == 0)
		{
			oslDrawString(37,133,"Model: PSP 1000");
		}
   
		else if(pspmodel == 1)
		{
			oslDrawString(37,133,"Model: PSP 2000");
		}
   
		else if (pspmodel == 2)
		{
			oslDrawString(37,133,"Model: PSP 3000");
		}
   
		else if(pspmodel == 3)
		{
			oslDrawString(37,133,"Model: PSP 3000");
		}
		
		else if (pspmodel == 4)
		{
			oslDrawString(37,133,"Model: PSP Go N1000");
		}
   
		else
		{
			oslDrawString(37,133,"Model: PS Vita");
		}
}

void about_menu()
{	
	aboutbg = oslLoadImageFilePNG("system/settings/aboutbg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!aboutbg)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(aboutbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		oslDrawString(37,73,"CyanoPSP Updates");
		oslDrawString(37,87,"Click for, view or install available updates");
		pspgetmodel();
		oslDrawStringf(37,119,"CyanoPSP: %s",Version);
		oslDrawString(37,147,"Build Date - Tuesday July 29th 2:42 PM EST");
		oslDrawString(37,172,"Kernel Version");
		oslDrawString(37,186,"Undefined-pspsdk_oslib");
		oslDrawString(37,200,"joellovesanna@psp #1");
		
		digitaltime();

		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 45 && cursor->y <= 90)
		{
			oslDrawImageXY(highlight, 16, 53);
			oslDrawString(37,73,"CyanoPSP Updates");
			oslDrawString(37,87,"Click for, view or install available updates");
		}
		
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
			oslDeleteImage(aboutbg);
			settingsmenu();	
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(aboutbg);
			settingsmenu();	
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(aboutbg);
			home();	
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(aboutbg);
			multitask();	
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 70 && cursor->y <= 138 && osl_pad.held.cross)
		{
			updates_menu();
		}
		
		if(osl_pad.held.select)
		{
			LoadStartModule("modules/RemoteJoyLite.prx");
			enableUsb();
			usb_debug = 1;
		}
		
		else if(osl_pad.held.select)
		{	
			StopUnloadModule("modules/RemoteJoyLite.prx");
			disableUsb();
			usb_debug = 0;
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

void updates_menu()
{		    
	updatesbg = oslLoadImageFilePNG("system/settings/updatesbg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!updatesbg)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(updatesbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		oslDrawString(35,73,"Check for Updates");
				
		digitaltime();

		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 45 && cursor->y <= 90)
		{
			oslDrawImageXY(highlight, 16, 54);
			oslDrawString(37,73,"Check for Updates");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 45 && cursor->y <= 90 && osl_pad.held.cross)
		{
			OnlineUpdater();
		}
		
		
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
			oslDeleteImage(updatesbg);
			about_menu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(updatesbg);
			about_menu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(updatesbg);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(updatesbg);
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
	oslNetTerm();
}

void performance_menu()
{	
	performancebg = oslLoadImageFilePNG("system/settings/performancebg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!performancebg)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(performancebg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		oslDrawString(40,98,"Processor");
		oslDrawString(40,161,"Ram Management");
		oslDrawString(40,215,"Memory Management");
		
		digitaltime();

		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 80 && cursor->y <= 138)
		{
			oslDrawImageXY(highlight, 15, 83);
			oslDrawString(40,98,"Processor");
		}
		
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
			oslDeleteImage(performancebg);
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(performancebg);
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(performancebg);
			home();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 80 && cursor->y <= 138 && osl_pad.held.cross)
		{
			processor_menu();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(performancebg);
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

void processor_menu(int argc, char *argv[])
{	
	processorbg = oslLoadImageFilePNG("system/settings/processorbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!processorbg)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	setfont();
	
	SceCtrlData pad;
	
	while (!osl_quit)
	{

		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		sceCtrlPeekBufferPositive(&pad, 1);
		
		oslDrawImageXY(processorbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		oslDrawString(35,74,"Current CPU Frequency");
		pspgetcpu_bus();
		oslDrawString(35,128,"CPU Overclock");
		oslDrawString(35,184,"Minimum CPU Frequency");
		oslDrawString(35,197,"20 MHz");
		oslDrawString(35,236,"Maximum CPU Frequency");
		oslDrawString(35,249,"333 MHz");
	
		digitaltime();

		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 118 && cursor->y <= 174)
		{
			oslDrawImageXY(highlight, 16, 118);
			oslDrawString(35,128,"CPU Overclock");
			oslDrawString(35,142,"Press R to increase frequency and L to decrease frequency");
			oslDrawString(35,156,"Press triangle to reset to default, 222/111");
		}

		if (osl_pad.held.triangle)
		{
		 setclock = 6;
		 scePowerSetClockFrequency(222, 222, 111);
		}
		
		if (current < 0)
		current = MAX;
		if (current > MAX)
		current = 0;
		if (pad.Buttons & PSP_CTRL_LTRIGGER)
		scePowerSetClockFrequency(cpu_list[current--], cpu_list[current--], bus_list[current--]);
		if (pad.Buttons & PSP_CTRL_RTRIGGER)
		scePowerSetClockFrequency(cpu_list[current++], cpu_list[current++], bus_list[current++]);

		oslDrawStringf(35,87,"%d/%d",cpu_list[current],bus_list[current]);
		
		oslDrawImage(cursor);
		
		if (osl_pad.held.square)
		{
			powermenu();
		}
		
		if (osl_pad.held.circle)
		{
			performance_menu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(processorbg);
			performance_menu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(processorbg);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslDeleteImage(processorbg);
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

/* Work in Progress Theme Manager */

/*

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
				case message:
                {
                        return GetTextureFromZip("message.png");
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

void theme_menu()
{	
	themebg = oslLoadImageFilePNG("system/settings/themebg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!themebg)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	setfont();

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(themebg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		oslDrawString(65,74,"Theme Packs");
		oslDrawString(65,128,"Styles");
		oslDrawString(65,184,"Icons");
		oslDrawString(65,236,"Fonts");
		
		digitaltime();

		battery();
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
			oslDeleteImage(themebg);
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(themebg);
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(themebg);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(themebg);
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

void wifi_menu()
{	
	wifibg = oslLoadImageFilePNG("system/settings/wifibg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!wifibg)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();

	int enabled = 1;
    int selectedConfig = 0;
	int wifi_y = 50;
	
	//Get connections list:
    struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
    int numconfigs = oslGetNetConfigs(configs);
	if (!numconfigs)
	{
        sprintf(Settings_message, "No configuration found!");
        enabled = 0;
    }
	
	oslNetInit(); 
	
	if (!oslIsWlanPowerOn())
        sprintf(Settings_message, "Please turn on the Wlan switch.");

	while (!osl_quit)
	{			
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(wifibg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		 if (enabled){
                sprintf(buffer, "%s", configs[selectedConfig].name);
    			oslDrawString(30, wifi_y+28, configs[selectedConfig].name);
         }
		
		oslDrawString(30, 200, Settings_message);
		
		wlanstatus1();
		digitaltime();

		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		
		 if (osl_keys->released.cross){
            connectToAP(selectedConfig + 1);
        }else if (osl_keys->released.up){
            if (++selectedConfig >= numconfigs)
                selectedConfig = numconfigs - 1;
        }else if (osl_keys->released.down){
            if (--selectedConfig < 0)
                selectedConfig = 0;
        }
		
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
			oslDeleteImage(wifibg);	
			oslNetTerm();
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(wifibg);	
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(wifibg);	
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(wifibg);	
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
	oslNetTerm();
}

void developer_menu()
{
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	check = oslLoadImageFilePNG("system/settings/check.png", OSL_IN_RAM, OSL_PF_8888);

	if (!developerbg || !check)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	setfont();

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(developerbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(check, 422, 177);

		oslDrawString(35,62,"Toggle Remote Joy Lite");
		oslDrawString(35,76,"Displays your PSP screen on your computer via USB.");
		oslDrawString(35,90,"Press Triangle to disable or it may cause the program to crash");
		oslDrawString(35,128,"Toggle USB Debugging");
		oslDrawString(35,142,"Press Triangle to disable or it may cause the program to crash");
		oslDrawString(35,174,"Advanced Reboot");
		oslDrawString(35,188,"When unlocked, include option in the power menu for");
		oslDrawString(35,202,"rebooting into recovery");
		oslDrawString(35,236,"Backup Password");
			
		digitaltime();

		battery();
		android_notif();
		usb_icon();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 55 && cursor->y <= 108)
		{
			oslDrawImageXY(highlight, 15, 55);
			oslDrawString(35,62,"Toggle Remote Joy Lite");
			oslDrawString(35,76,"Displays your PSP screen on your computer via USB.");
			oslDrawString(35,90,"Press Triangle to disable or it may cause the program to crash");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 109 && cursor->y <= 165)
		{
			oslDrawImageXY(highlight, 15, 109);
			oslDrawString(35,128,"Toggle USB Debugging");
			oslDrawString(35,142,"Press Triangle to disable or it may cause the program to crash");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 222 && cursor->y <= 278)
		{
			oslDrawImageXY(highlight, 15, 222);
			oslDrawString(35,236,"Backup Password");
		}
		
		navbar_buttons();
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
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			settingsmenu();
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(check);
			multitask();
		}
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 109 && cursor->y <= 165 && osl_pad.held.cross)
		{

			LoadStartModule("modules/psplink.prx");
			PSPDebug = 1;
				if(PSPDebug == 1  && osl_pad.held.triangle)
				{	
				StopUnloadModule("modules/psplink.prx");			
				}
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 55 && cursor->y <= 108 && osl_pad.held.cross)
		{	
			RJL = 1;
			LoadStartModule("modules/RemoteJoyLite.prx");
			
				if(RJL == 1 && osl_pad.held.triangle)
				{	
				StopUnloadModule("modules/RemoteJoyLite.prx");
				}
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 222 && cursor->y <= 278 && osl_pad.held.cross)
		{
			backupPassword();
		}
		
		oslEndDrawing();
        oslSyncFrame();
        oslAudioVSync();
}	
}

void settings_highlight()
{
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 53 && cursor->y <= 98)
		{
			oslDrawImageXY(wifi, 16, 63);
			oslDrawString(55,76,"Wi-Fi");
			wlanstatus();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 99 && cursor->y <= 141)
		{
			oslDrawImageXY(developeroptions, 16, 105);
			oslDrawString(55,118,"Developer Options");
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 142 && cursor->y <= 183)
		{
			oslDrawImageXY(themes, 16, 148);
			oslDrawString(55,161,"Themes");
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

void settings_deleteImages()
{
	oslDeleteImage(settingsbg);
	oslDeleteImage(about);
	oslDeleteImage(themes);
	oslDeleteImage(developeroptions);
	oslDeleteImage(wifi);
}

int settingsmenu()
{
	settingsbg = oslLoadImageFilePNG("system/settings/settingsbg.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);
	about = oslLoadImageFilePNG("system/settings/about.png", OSL_IN_RAM, OSL_PF_8888);
	themes = oslLoadImageFilePNG("system/settings/themes.png", OSL_IN_RAM, OSL_PF_8888);
	developeroptions = oslLoadImageFilePNG("system/settings/developeroptions.png", OSL_IN_RAM, OSL_PF_8888);
	wifi = oslLoadImageFilePNG("system/settings/wifi.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	performance = oslLoadImageFilePNG("system/settings/performance.png", OSL_IN_RAM, OSL_PF_8888);
		
	setfont();

	if (!settingsbg || !onswitch || !offswitch || !about || !developeroptions || !wifi || !themes || !highlight)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(settingsbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		wlanstatus();
		oslDrawString(55,76,"Wi-Fi");
		oslDrawString(55,118,"Developer Options");
		oslDrawString(55,161,"Themes");
		oslDrawString(55,204,"Performance");
		oslDrawString(55,246,"About PSP");
		
		digitaltime();

		battery();
		android_notif();
		usb_icon();
		settings_highlight();
		navbar_buttons();
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
			settings_deleteImages();
			oslDeleteImage(highlight);
			appdrawer();
		}
			
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			settings_deleteImages();
			oslDeleteImage(highlight);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			settings_deleteImages();
			oslDeleteImage(highlight);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			settings_deleteImages();
			multitask();
		}
			
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 53 && cursor->y <= 98 && osl_pad.held.cross)
		{	
			settings_deleteImages();
			wifi_menu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 99 && cursor->y <= 141 && osl_pad.held.cross)
		{	
			settings_deleteImages();
			developer_menu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 142 && cursor->y <= 183 && osl_pad.held.cross)
		{	
			settings_deleteImages();
			theme_menu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 184 && cursor->y <= 227 && osl_pad.held.cross)
		{
			settings_deleteImages();
			performance_menu();
		}
				
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 228 && cursor->y <= 250 && osl_pad.held.cross)
		{	
			settings_deleteImages();
			about_menu();
		}
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
		
        oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();
        oslAudioVSync();
		}
}

