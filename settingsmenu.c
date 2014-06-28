#include <pspkernel.h>
#include <oslib/oslib.h>
#include "include/pgeZip.h"

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

//declaration
OSL_IMAGE *settingsbg, *cursor, *wificon, *usbdebug, *aboutbg, *offswitch, *onswitch, *themebg, *performancebg, *wifibg, *developerbg, *about, *performance, *developeroptions, *themes, *wifi, *processorbg, *processor;

//definition of our sounds
OSL_SOUND *tone;

int usb_debug = 0;
char pspmodel;
char usbStatus = 0;
char usbModuleStatus = 0;

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

void updater()
{
	pgeNetInit();
	
		//download file
        pgeNetGetFile("http://zone-wifi.fr/psp/", "../CyanogenMod.zip");

        oslDrawStringf(20, 20,"Installing update...");
        
        
        pgeZip* zipFiles = pgeZipOpen("../CyanogenMod.zip");
        
        chdir("..");
        
        pgeZipExtract(zipFiles, NULL);
        pgeZipClose(zipFiles);

        oslDrawStringf(20,30,"Installation done - press X to exit");
	
	while (!osl_quit)
	{ 
	
           int result;
                
           if(result != -1)
		   break;
		   
	while(1)
        {
                if(osl_pad.held.circle)
                {
                        break;
                }
        }
        
        pgeNetShutdown();
	
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
			oslDrawString(37,73,"Model: PSP 1000");
		}
   
		else if(pspmodel == 1)
		{
			oslDrawString(37,73,"Model: PSP 2000");
		}
   
		else if (pspmodel == 2)
		{
			oslDrawString(37,73,"Model: PSP 3000");
		}
   
		else if(pspmodel == 3)
		{
			oslDrawString(37,73,"Model: PSP 3000");
		}
		
		else if (pspmodel == 4)
		{
			oslDrawString(37,73,"Model: PSP Go N1000");
		}
   
		else
		{
			oslDrawString(37,73,"Model: PS Vita");
		}
}

void about_menu()
{
//loads our images into memory
	aboutbg = oslLoadImageFilePNG("system/settings/aboutbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
	//Debugger
	if (!aboutbg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(aboutbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		pspgetmodel();
		oslDrawString(37,123,"CyanoPSP - Alpha build 2");
		oslDrawString(37,172,"Kernel Version");
		oslDrawString(37,185,"Undefined-pspsdk_oslib");
		oslDrawString(37,198,"joellovesanna@psp #1");
		
		digitaltime();
			
		//calls the functions
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
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
		
		if(osl_pad.held.select)
		{
			enableUsb();
			usb_debug = 1;
		}
		
		else if(osl_pad.held.select)
		{
			disableUsb();
			usb_debug = 0;
		}
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
}

	oslDeleteImage(aboutbg);

}

void performance_menu()
{
//loads our images into memory
	performancebg = oslLoadImageFilePNG("system/settings/performancebg.png", OSL_IN_RAM, OSL_PF_8888);
	processor = oslLoadImageFilePNG("system/settings/processor.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
	//Debugger
	if (!performancebg || !cursor || !wificon || !processor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(performancebg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		oslDrawString(40,98,"Processor");
		oslDrawString(40,161,"Ram Management");
		oslDrawString(40,215,"Memory Management");
		
		digitaltime();
			
		//calls the functions
		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 80 && cursor->y <= 138)
		{
			oslDrawImageXY(processor, 16, 83);
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
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 80 && cursor->y <= 138 && osl_pad.held.cross)
		{
			processor_menu();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
	
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
}

	oslDeleteImage(performancebg);

}

void processor_menu()
{
//loads our images into memory
	processorbg = oslLoadImageFilePNG("system/settings/processorbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
	//Debugger
	if (!processorbg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(processorbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		oslDrawString(35,74,"Current CPU Frequency");
		pspgetcpu_bus();
		oslDrawString(35,128,"MIPS R4000");
		oslDrawString(35,184,"Minimum CPU Frequency");
		oslDrawString(35,197,"20 MHz");
		oslDrawString(35,236,"Maximum CPU Frequency");
		oslDrawString(35,249,"333 MHz");
		
		digitaltime();
			
		//calls the functions
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
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
}

	oslDeleteImage(processorbg);

}

void theme_menu()
{
//loads our images into memory
	themebg = oslLoadImageFilePNG("system/settings/themebg.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
	//Debugger
	if (!themebg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(themebg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		oslDrawString(65,74,"Theme Packs");
		oslDrawString(65,128,"Styles");
		oslDrawString(65,184,"Icons");
		oslDrawString(65,236,"Fonts");
		
		digitaltime();
		
		//calls the functions
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
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
}

	oslDeleteImage(themebg);
	
}

void wifi_menu()
{
//loads our images into memory
	wifibg = oslLoadImageFilePNG("system/settings/wifibg.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
	//Debugger
	if (!wifibg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(wifibg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);
		
		wlanstatus1();
		digitaltime();
		
		//calls the functions
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
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
}

	oslDeleteImage(wifibg);	
	
}

void developer_menu()
{
//loads our images into memory
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
	//Debugger
	if (!developerbg || !cursor || !wificon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(developerbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		oslDrawString(35,76,"Launch Tools");
		oslDrawString(35,128,"Take bug report");
		oslDrawString(35,184,"Advanced Reboot");
		oslDrawString(35,236,"Backup Password");
			
		digitaltime();
	
		//calls the functions
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
			settingsmenu();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			settingsmenu();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			multitask();
		}
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
}

	oslDeleteImage(developerbg);
	
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

int settingsmenu()
{
	//loads our images into memory
	settingsbg = oslLoadImageFilePNG("system/settings/settingsbg.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);
	about = oslLoadImageFilePNG("system/settings/about.png", OSL_IN_RAM, OSL_PF_8888);
	performance = oslLoadImageFilePNG("system/settings/performance.png", OSL_IN_RAM, OSL_PF_8888);
	themes = oslLoadImageFilePNG("system/settings/themes.png", OSL_IN_RAM, OSL_PF_8888);
	developeroptions = oslLoadImageFilePNG("system/settings/developeroptions.png", OSL_IN_RAM, OSL_PF_8888);
	wifi = oslLoadImageFilePNG("system/settings/wifi.png", OSL_IN_RAM, OSL_PF_8888);
	
	setfont();
	
	//Debugger
	if (!settingsbg || !cursor || !wificon || !onswitch || !offswitch || !about || !developeroptions || !wifi || !themes || !performance)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	//Main loop to run the program
	while (!osl_quit)
	{
		
		//Draws images onto the screen
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(settingsbg, 0, 19);
		oslDrawImageXY(wificon, 375, 1);

		wlanstatus();
		oslDrawString(55,76,"Wi-Fi");
		oslDrawString(55,118,"Developer Options");
		oslDrawString(55,161,"Themes");
		oslDrawString(55,204,"Performance");
		oslDrawString(55,246,"About PSP");
		
		digitaltime();
			
		//calls the functions
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
			appdrawer();
		}
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 53 && cursor->y <= 98 && osl_pad.held.cross)
		{
			wifi_menu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 99 && cursor->y <= 141 && osl_pad.held.cross)
		{
			developer_menu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 142 && cursor->y <= 183 && osl_pad.held.cross)
		{
			theme_menu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 184 && cursor->y <= 227 && osl_pad.held.cross)
		{
			performance_menu();
		}
				
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 228 && cursor->y <= 250 && osl_pad.held.cross)
		{
			about_menu();
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
			
        oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
		}
		
	oslDeleteImage(settingsbg);
	oslDeleteImage(about);
	oslDeleteImage(performance);
	oslDeleteImage(themes);
	oslDeleteImage(developeroptions);
	oslDeleteImage(wifi);
	
}

