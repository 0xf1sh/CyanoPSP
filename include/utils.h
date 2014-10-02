#include <pspkernel.h>
#include <oslib/oslib.h>

//PSP USB
#include <pspusb.h>
#include <pspusbstor.h>
#include <pspmodulemgr.h>
#include "pspusbdevice.h"

#define downloadpath "ms0:/PSP/GAME/CyanogenMod/downloads"

int fileExists(const char* path);
int dirExists(const char* path);
void makedownloaddir();
void fadeOut(OSL_IMAGE* bg,int x, int y);
void fadeIn(OSL_IMAGE* bg, int x, int y);
int disableUsb(void);
int enableUsb();
int enableUsbEx(u32 device);
void isUSBCableConnected();

