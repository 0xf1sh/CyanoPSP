#include "utils.h"

int transp;//Fo transparency
OSL_COLOR *fade;//To hold the colour of the rectangle blitted

char usbStatus = 0;
char usbModuleStatus = 0;

int fileExists(const char* path)
{
	SceUID dir = sceIoOpen(path, PSP_O_RDONLY, 0777);
	if (dir >= 0){
		sceIoClose(dir);
		return 1;
	}
	else {
		return 0;
	}
}

int dirExists(const char* path)
{
	SceUID dir = sceIoDopen(path);
	if (dir >= 0){
		sceIoDclose(dir);
		return 1;
	}
	else {
		return 0;
	}
}

void deleteUpdateFile()
{
	if (fileExists("ms0:/PSP/GAME/CyanogenMod.zip"))
	{
		sceIoRemove("ms0:/PSP/GAME/CyanogenMod.zip");
	}
}

void makeDownloadDir()
{
	SceUID dir = sceIoDopen(downloadPath);
	
	if (dirExists(downloadPath))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenMod/downloads",0777);
}
}

void makeMusicDir()
{
	SceUID dir = sceIoDopen(musicPath);
	
	if (dirExists(musicPath))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/MUSIC",0777);
}
}

/*This was originally coded by Blade_punk. I modified it to add the X, and Y bits. */
void fadeOut(OSL_IMAGE* bg,int x, int y)//Name and params taken
{
	for(transp = 0; transp < 255; transp += 3)//Sets the transparency to 0 aka invisible. Then it increases by 3 until it equals 255 aka completely visible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
	}
}
void fadeIn(OSL_IMAGE* bg, int x, int y)//Name and params taken
{
	for(transp = 255; transp > 0; transp -= 3)//Sets the transparency to 255 aka completely visible. Then it decreases by 3 until it equals 255 aka invisible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
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

int isUSBCableConnected(){
    return (sceUsbGetState() & PSP_USB_CABLE_CONNECTED);
}
