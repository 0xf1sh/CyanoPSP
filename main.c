#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <oslib/oslib.h>
 
PSP_MODULE_INFO("OSLib Sample", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(4*1024);
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Globals:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int runningFlag = 1;
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
    int skip = 0;
    char message[100] = "";
    SetupCallbacks();
 
    //Initialize OSLib
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
    oslSetQuitOnLoadFailure(1);
    oslSetKeyAutorepeatInit(40);
    oslSetKeyAutorepeatInterval(10);
 
    //This is for the font system
    oslIntraFontInit(INTRAFONT_CACHE_MED);
	
	    //Load font:
    OSL_FONT *font = oslLoadFontFile("flash0:/font/ltn0.pgf");
    oslSetFont(font);
 
//to load images
OSL_IMAGE * boot[4];
boot[0] = oslLoadImageFilePNG("System/Kernel/Boot/1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
boot[1] = oslLoadImageFilePNG("System/Kernel/Boot/2.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
boot[2] = oslLoadImageFilePNG("System/Kernel/Boot/3.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
boot[3] = oslLoadImageFilePNG("System/Kernel/Boot/4.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

//to draw
int current_frame = 0;
while(runningFlag && !osl_quit)
{
	oslStartDrawing();
	oslDrawImageXY(boot[current_frame], 0, 0); 
	
	sceDisplayWaitVblankStart();
	oslEndDrawing();
	oslEndFrame();
	skip = oslSyncFrame();
	sceKernelDelayThread(1000000 * 1);
	
	current_frame++;
	if(current_frame > 3)
		current_frame = 0;
    //Quit OSL:
    
    oslEndGfx();
    oslQuit();
 
    sceKernelExitGame();
    return 0;
 
}
}


