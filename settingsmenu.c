#include <pspkernel.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *settings_bg, *cursor, *navbar, *wificon, *usbdebug;

//definition of our sounds
OSL_SOUND *tone;

int usb_debug = 0;
char pspmodel;
char usbStatus = 0;
char usbModuleStatus = 0;

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
   
		else if (pspmodel == 4)
		{
			oslDrawString(37,73,"Model: PSP Go N1000");
		}
   
		else
		{
			oslDrawString(37,73,"Model: PS Vita");
		}
}

int settingsmenu()
{
	//loads our images into memory
	settings_bg = oslLoadImageFilePNG("system/home/menu/settings_bg.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	//Set fonts
	oslSetFont(pgfFont);
	
	//Debugger
	if (!settings_bg || !cursor || !wificon)
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
		oslDrawImageXY(settings_bg, 0, 19);
		oslDrawImageXY(navbar, 110, 237);
		oslDrawImageXY(wificon, 387, 1);
		
		pspgetmodel();
		oslDrawString(37,123,"CyanoPSP - Alpha build 2");
		oslDrawString(37,172,"Kernel Version");
		oslDrawString(37,185,"Undefined-pspsdk_oslib");
		oslDrawString(37,198,"joellovesanna@psp #1");
			
		//calls the functions
		battery();
		back();
		home_icon();
		multi();
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
}

