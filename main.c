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
 
int main()
{
	//Initialization of the Oslib's library
	oslInit(0);

	//Initialization of Oslib's graphics mode
	oslInitGfx(OSL_PF_8888, 1);
	
	//Sets the transparency color (blue)
	oslSetTransparentColor(RGB(0,0,255));

	//loads our images into memory
	background = oslLoadImageFile("System/Home/Wallpapers/1.png", OSL_IN_RAM, OSL_PF_5551);
	cursor = oslLoadImageFilePNG("System/Cursor/cursor.png", OSL_IN_RAM, OSL_PF_5551);
	appicon = oslLoadImageFilePNG("System/Home/Icons/appicon1.png", OSL_IN_RAM, OSL_PF_5551);
	appicon2 = oslLoadImageFile("System/Home/Icons/appicon2.png", OSL_IN_RAM, OSL_PF_5551);
	navbar = oslLoadImageFile("System/Home/Icons/nav.png", OSL_IN_RAM, OSL_PF_5551);
	wificon = oslLoadImageFile("System/Home/Icons/wificon.png", OSL_IN_RAM, OSL_PF_5551);
	apollo = oslLoadImageFilePNG("System/Home/Icons/apollo.png", OSL_IN_RAM, OSL_PF_5551);
	gmail = oslLoadImageFilePNG("System/Home/Icons/gmail.png", OSL_IN_RAM, OSL_PF_5551);
	message = oslLoadImageFilePNG("System/Home/Icons/message.png", OSL_IN_RAM, OSL_PF_5551);
	browser = oslLoadImageFile("System/Home/Icons/browser.png", OSL_IN_RAM, OSL_PF_5551);
	google = oslLoadImageFile("System/Home/Icons/google.png", OSL_IN_RAM, OSL_PF_5551);
	
	//Disables the transpaent color (blue)
	oslDisableTransparentColor();
	
	//Debugger
	if (!background || !cursor)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	//Main loop to run the program
	while (!osl_quit)
	{
		//Draws images onto the screen
		oslStartDrawing();
		
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Cursor movement
		if (osl_keys->held.down) cursor->y += 4;
		if (osl_keys->held.up) cursor->y -= 4;
		if (osl_keys->held.left) cursor->x -= 4;
		if (osl_keys->held.right) cursor->x += 4;

		//Print the images onto the screen
		oslDrawImage(background);
		oslDrawImage(cursor);
		oslDrawImageXY(navbar, 103, 241);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(google, 22, 26);
		oslDrawImageXY(apollo, 105, 195);
		oslDrawImageXY(browser, 276, 195);
		oslDrawImageXY(gmail, 331, 195);
		oslDrawImageXY(message, 160, 195);
		oslDrawImageXY(appicon2, 223, 205);
		oslDrawImage(cursor);

		//Ends printing
		oslEndDrawing();

		//Synchronizes the screen 
		oslSyncFrame();		
	}
	
	//Terminates/Ends the program
	oslEndGfx();
	oslQuit();
	return 0;
}

