// Thanks to West-Li, this program is based on his PSP Module Checker.
// Thanks DAX for his ipl_update.prx
// Thanks Yoti for his libpspident.a
// Thanks Hellcat for his hc menu lib.
// Thanks Raing3 for his psppower lib.

#include <pspkernel.h>  
#include <pspdebug.h> 
#include <pspctrl.h>
#include <psppower.h>
#include <stdio.h>
#include <oslib/oslib.h>

#include <kubridge.h>

#include "recoverymenu.h"
#include "modules/ipl_update.h"
#include "modules/batman.h"
#include "modules/kuman.h"

#include "menu.h"
#include "kuman_header.h"
#include "systemctrl_se.h"
#include "settingsmenu.h"

#define Version "flash0:/vsh/etc/version.txt"
#define PWD "password.TXT"

extern void CheckerPrintf(char *fmt, ...);

OSL_IMAGE *recoverybg, *Selector;

OSL_FONT *roboto;

int selector_image_x; //Determines the starting x position of the selection
int selector_image_y; //Determines the starting y position of the selection

int color = 0;
u32 value;

SceCtrlData pad;

SceUID fd;

SEConfig config;

char BatSerCurrent[4];

void ShowPage5()
{
    int type, size, psp, pan, cfw;
	
	CheckerInitMainScreen("Advanced Configuration, CFW & PSP Utility Info");
	
	psp = chGetPSPHackable();
	pan = chGetPSPCreatePandora();
    cfw = chDetectCFW();
	
	chGetIplAttribute(&type, &size);
	sctrlSEGetConfig(&config);

	CheckerPrintf("Plain modules in UMD/ISO     (currently: %s)\n", config.umdactivatedplaincheck ? "Enabled" : "Disabled");
    CheckerPrintf("Execute PBOOT.BIN in UMD/ISO (currently: %s)\n", config.executebootbin ? "Enabled" : "Disabled");
    CheckerPrintf("XMB Plugins                  (currently: %s)\n", config.xmbplugins ? "Disabled" : "Enabled");
    CheckerPrintf("GAME Plugins                 (currently: %s)\n", config.gameplugins ? "Disabled" : "Enabled");
    CheckerPrintf("POPS Plugins                 (currently: %s)\n\n", config.popsplugins ? "Disabled" : "Enabled");
    CheckerPrintf("IPL:                %s (Size: 0x%08X)\n", IPLs[type], size);
	CheckerPrintf("CFW:                %s\n\n", CFWs[cfw]);
	CheckerPrintf("PSP Hackable:       %s\n", PSPUtils[psp]);
	CheckerPrintf("PSP Create Pandora: %s\n\n\n", PSPUtils[pan]);
	
	CheckerPrintf("Press L/R to switch sections, X to edit and Circle to return.\n");
	
	sceKernelDelayThread(200000);
	
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_LTRIGGER)
		{
			sceKernelDelayThread(250000);
			ShowPage4();
        }
		if (pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			sceKernelDelayThread(250000);
			ShowPage1();
        }
		if (pad.Buttons & PSP_CTRL_CROSS)
		{
			ShowAdvancedCnfMenu();
        }
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
			mainRecoveryMenu();
        }
		
		sceKernelDelayThread(10000);
	}
}

void ShowPage4()
{
    CheckerInitMainScreen("Miscellaneous");

	sctrlSEGetConfig(&config);

	CheckerPrintf("Skip Sony logo                 (currently: %s)\n", config.skiplogo ? "Enabled" : "Disabled");
    CheckerPrintf("Hide corrupt icons             (currently: %s)\n", config.hidecorrupt ? "Enabled" : "Disabled");
    CheckerPrintf("Game folder homebrew           (currently: %s)\n", config.gamekernel150 ? "1.50 Kernel" : "6.XX Kernel");
    CheckerPrintf("Autoboot program at /PSP/GAME/BOOT/ (currently: %s)\n", config.startupprog ? "Enabled" : "Disabled");
    CheckerPrintf("UMD Mode                       (currently: %s)\n", umdmodes[config.umdmode]);
    CheckerPrintf("Fake region                    (currently: %s)\n", regions[config.fakeregion]);
    CheckerPrintf("Use VshMenu                    (currently: %s)\n", vshmenuoptions[config.vshmenu]);
    CheckerPrintf("XMB Usb Device                 (currently: %s)\n", usbdevices[config.usbdevice]);
    CheckerPrintf("Use network update             (currently: %s)\n", config.notusedaxupd ? "Disabled" : "Enabled");
    CheckerPrintf("Hide PIC1.PNG and PIC0.PNG     (currently: %s)\n", config.hidepics ? "Enabled" : "Disabled");
    CheckerPrintf("Use version.txt                (currently: %s)\n", config.useversiontxt ? "Enabled" : "Disabled");
    CheckerPrintf("Use Slim colors on Classic PSP (currently: %s)\n", config.slimcolors ? "Enabled" : "Disabled");
    CheckerPrintf("Hide MAC address               (currently: %s)\n\n\n", config.hidemac ? "Enabled" : "Disabled");
	
	CheckerPrintf("Press L/R to switch sections, X to edit and Circle to return.\n");
	
	sceKernelDelayThread(200000);
	
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_LTRIGGER)
		{
			sceKernelDelayThread(250000);
			ShowPage3();
        }
		if (pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			sceKernelDelayThread(250000);
			ShowPage5();
        }
		if (pad.Buttons & PSP_CTRL_CROSS)
		{
			ShowCnfMenu();
        }
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
			mainRecoveryMenu();
        }
		
		sceKernelDelayThread(10000);
	}
}

void backupPassword()
{	
	char pass[5];
	memset(pass, 0, sizeof(pass));
	
	FILE * configtxt = fopen(PWD, "wb"); //create config file
	fprintf("Password: %s\n", GetRegistryValue("/CONFIG/SYSTEM/LOCK", "password", &pass, sizeof(pass)));
	fclose(PWD);	
}

//Based on raing3s CheckSysInfoP4 
void ShowPage3()
{
    int cpu, bus;
	char name[32], pass[5], button[256], wma[256], flash[256];
	u8 kirk[4], spock[4], macaddr[512];
	
	*(u32 *)kirk = chGetKirk();
	*(u32 *)spock = chGetSpock();
	
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	
	GetRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", &value);

    if (value == 1) 
    {
        oslDrawStringf(10,100,button, "Swap buttons: Yes (currently: O is enter)\n");		
    } 
	else 
	{
        oslDrawStringf(10,100,button, "Swap buttons: No (currently: X is enter)\n");		
    }
	
	GetRegistryValue("/CONFIG/MUSIC", "wma_play", &value);

    if (value == 1)
	{
		oslDrawStringf(10,110,wma, "WMA:          Yes\n");	
	}
	else
	{
	    oslDrawStringf(10,110,wma, "WMA:          No\n");		
	}
	
	GetRegistryValue("/CONFIG/BROWSER", "flash_activated", &value);

    if (value == 1)
	{
		oslDrawStringf(10,120,flash, "Flash player: Yes\n\n\n");	
	}
	else
	{
	    oslDrawStringf(10,120,flash, "Flash player: No\n\n\n");		
	}

	memset(name, 0, sizeof(name));
	memset(pass, 0, sizeof(pass));
	
	chGetCpuSpeed(&cpu, &bus);
	chGetMACAddress(macaddr);
	
	oslDrawStringf(10,5,"System Information");

	oslDrawStringf(10,20,"CPU Speed:    %i/%i Mhz\n", cpu, bus);
	oslDrawStringf(10,30,"WLAN:         %s\n\n", sceWlanGetSwitchState() == 0 ? "Off" : "On");
	oslDrawStringf(10,40,"Name:         %s\n", GetRegistryValue("/CONFIG/SYSTEM", "owner_name", &name, sizeof(name)));
	oslDrawStringf(10,50,"Password:     %s\n", GetRegistryValue("/CONFIG/SYSTEM/LOCK", "password", &pass, sizeof(pass)));
	
	if(VerifyFile("machide.chk") == -1){
	   oslDrawStringf(10,60,"MAC Address:  %02X:%02X:%02X:%02X:%02X:%02X\n\n", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	}
	else{
	   oslDrawStringf(10,60,"MAC Address:  00:00:00:00:00:00 (Hidden)\n\n");   
	}
	   
	oslDrawStringf(10,70,"Scramble:     0x%08X\n\n", chGetScramble());
	oslDrawStringf(10,80,"Kirk:         %c%c%c%c\n", kirk[3], kirk[2], kirk[1], kirk[0]);
	
	if(kuKernelGetModel() == 4){
	oslDrawStringf(10,90,"Spock:        -\n\n");
	}
	else{
	oslDrawStringf(10,90,"Spock:        %c%c%c%c\n\n", spock[3], spock[2], spock[1], spock[0]); 
	}
	
	oslDrawStringf(10,200,"Press L/R to switch sections, X to edit and Circle to return.\n");
		
		if (osl_keys->pressed.L)
		{
			ShowPage2();
        }
		if (osl_keys->pressed.R)
		{
			ShowPage4();
        }
		if (osl_keys->pressed.cross)
		{
			ShowSystemMenu();
        }
		if (osl_keys->pressed.circle)
		{
			mainRecoveryMenu();
		}
		
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
}

//Based on raing3s CheckSysInfoP2
void ShowPage2()
{
    int batser, batmode;
	
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	
	oslDrawStringf(10,5,"Battery Information");
	
	batmode = GetBatType();
	batser = GetBatSer();

	oslDrawStringf(10,20,"Battery Status:  %s\n", scePowerIsBatteryCharging() == 1 ? "Charging" : "Using");
	oslDrawStringf(10,30,"Battery %%:       %i%%\n\n", scePowerGetBatteryLifePercent() < 0 ? 0 : scePowerGetBatteryLifePercent()); //Here I used printf because my CheckerPrintf doesn't printf %
	oslDrawStringf(10,40,"Hours Left:   %i:%02i\n\n", scePowerGetBatteryLifeTime() < 0 ? 0 : (scePowerGetBatteryLifeTime() / 60), scePowerGetBatteryLifeTime() < 0 ? 0 : (scePowerGetBatteryLifeTime() - (scePowerGetBatteryLifeTime() / 60 * 60)));
	oslDrawStringf(10,50,"Battery Temp:    %i�C\n", scePowerGetBatteryTemp() < 0 ? 0 : scePowerGetBatteryTemp());
	oslDrawStringf(10,60,"Battery Voltage: %0.3fV\n\n", scePowerGetBatteryVolt() < 0 ? 0 : (float)scePowerGetBatteryVolt() / 1000.0);
	oslDrawStringf(10,70,"Remain Capacity: %i mAh\n", scePowerGetBatteryRemainCapacity() < 0 ? 0 : scePowerGetBatteryRemainCapacity()); //From raing3s psppower
	
	if(chGetPSPCreatePandora() == 0) //Yes so we can show the serial, mode of battery
	{
	    oslDrawStringf(10,80,"Battery Type:    %s\n", Batterys[batmode]);
	    oslDrawStringf(10,90,"Battery Serial:  0x%08X\n\n\n", batser);
	}
	else
	{
	    oslDrawStringf(10,80,"Battery Type:    -\n");
	    oslDrawStringf(10,90,"Battery Serial:  -\n\n\n");
	}
	
	oslDrawStringf(10,200,"Press L/R to switch sections, X to edit and Circle to return.\n");

		if (osl_keys->pressed.L)
		{
			ShowPage1();
        }
		if (osl_keys->pressed.R)
		{
			ShowPage3();
        }
		if (osl_keys->pressed.cross)
		{
			ShowBatteryMenu();
        }
		if (osl_keys->pressed.circle)
		{
			mainRecoveryMenu();
		}
		
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
}

// Don't judge ;) I prefer the long and easy way lol.
// I tried using oslDrawStringf("Kernel Version: %s (0x%08X)\n\n", FWs[type], sceKernelDevkitVersion());
// Returned null, so yeah.

void sceKernelVersion()
{
	if(sceKernelDevkitVersion() == 0x01000300) 
	{
		oslDrawStringf(10,20,"Kernel Version: 1.00 (0x01000300)");
	}
    else if(sceKernelDevkitVersion() == 0x01050001) 
	{
		oslDrawStringf(10,20,"Kernel Version: 1.50 (0x01050001)");
	}
	else if(sceKernelDevkitVersion() == 0x01050100) 
	{
		oslDrawStringf(10,20,"Kernel Version: 1.51 (0x01050100)");
	}
	else if(sceKernelDevkitVersion() == 0x01050200) 
	{
		oslDrawStringf(10,20,"Kernel Version: 1.52 (0x01050200)");
	}
	else if(sceKernelDevkitVersion() == 0x02000010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.00 (0x02000010)");
	}
	else if(sceKernelDevkitVersion() == 0x02050010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.50 (0x02050010)");
	}
	else if(sceKernelDevkitVersion() == 0x02060010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.60 (0x02060010)");
	}
	else if(sceKernelDevkitVersion() == 0x02070010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.70 (0x02070010)");
	}
	else if(sceKernelDevkitVersion() == 0x02070110) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.71 (0x02070110)");
	}
	else if(sceKernelDevkitVersion() == 0x02080010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.80 (0x02080010)");
	}
	else if(sceKernelDevkitVersion() == 0x02080110) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.81 (0x02080110)");
	}
	else if(sceKernelDevkitVersion() == 0x02080210) 
	{
		oslDrawStringf(10,20,"Kernel Version: 2.82 (0x02080210)");
	}
	else if(sceKernelDevkitVersion() == 0x03000010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.00 (0x03000010)");
	}
	else if(sceKernelDevkitVersion() == 0x03000110) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.01 (0x03000110)");
	}
	else if(sceKernelDevkitVersion() == 0x03000210) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.02 (0x03000210)");
	}
	else if(sceKernelDevkitVersion() == 0x03000310) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.03 (0x03000310)");
	}
	else if(sceKernelDevkitVersion() == 0x03010010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.10 (0x03010010)");
	}
	else if(sceKernelDevkitVersion() == 0x03010110) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.11 (0x03010110)");
	}
	else if(sceKernelDevkitVersion() == 0x03030010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.30 (0x03030010)");
	}
	else if(sceKernelDevkitVersion() == 0x03040010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.40 (0x03040010)");
	}
	else if(sceKernelDevkitVersion() == 0x03050010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.50 (0x03050010)");
	}
	else if(sceKernelDevkitVersion() == 0x03050110) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.51 (0x03050110)");
	}
	else if(sceKernelDevkitVersion() == 0x03050210) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.52 (0x03050210)");
	}
	else if(sceKernelDevkitVersion() == 0x03070010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.70 (0x03070010)");
	}
	else if(sceKernelDevkitVersion() == 0x03070110) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.71 (0x03070110)");
	}
	else if(sceKernelDevkitVersion() == 0x03070210) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.72 (0x03070210)");
	}
	else if(sceKernelDevkitVersion() == 0x03070310) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.73 (0x03070310)");
	}
	else if(sceKernelDevkitVersion() == 0x03080010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.80 (0x03080010)");
	}
	else if(sceKernelDevkitVersion() == 0x03090010) 
	{
		oslDrawStringf(10,20,"Kernel Version: 3.90 (0x03090010)");
	}
}

void ShowPage1()
{
    int baryon, pommel, tachyon, fuseid, fusecfg, mb, model, type, region;
	char *unk_minor = "-";
	
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	
	oslDrawStringf(10,5,"PSP General Information");
	
	chGetVersions(&baryon, &pommel, &tachyon, &fuseid, &fusecfg);
	
	mb = chDetectMotherboard();
    model = chDetectModel();	
    type = chDetectOFW();
    region = chGetRegion();		

	oslDrawStringf(10,20,"Kernel Version: %s (0x%08X)\n\n", FWs[type], sceKernelDevkitVersion());
	pspgetmodel();
	oslDrawStringf(10,60,"Module:         %s\n", Modules[model]);      
	oslDrawStringf(10,70,"Motherboard:    %s\n\n", MBs[mb]);
	oslDrawStringf(10,80,"Tachyon:        0x%08X\n", tachyon);
	oslDrawStringf(10,90,"Baryon:         0x%08X\n", baryon);
	oslDrawStringf(10,100,"Pommel:         0x%08X\n\n", pommel);
	oslDrawStringf(10,110,"Fuse ID:        0x%04X%08X\n", fuseid);
	oslDrawStringf(10,120,"Fuse CFG:       0x%08X\n\n", fusecfg);
	oslDrawStringf(10,130,"Region:         %s\n\n\n", Regions[region]);
	
	oslDrawStringf(10,200,"Press L/R to switch sections and Circle to return.\n");
		
		if (osl_keys->pressed.L)
		{
			ShowPage5();
        }
		if (osl_keys->pressed.R)
		{
			ShowPage2();
        }
		if (osl_keys->pressed.circle)
		{
			mainRecoveryMenu();
		}
	   
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
}

void ShowVersionTxt()
{
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	
	oslDrawStringf(10,5,"PSP Version.txt");
	
	if(CheckVersion(Version) == -1)
	   oslDrawStringf(10,40,"Version.txt not found...");
	     
	oslDrawStringf(10,40,"Version.txt:\n");
    oslDrawStringf(10,60," \n%s\n\n", GetVersion());	
	
	oslDrawStringf(10,200,"Press Circle to return to Main Menu.\n\n");

	if (osl_keys->pressed.circle)
		{
		    mainRecoveryMenu();
		}
		
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
}

void ConfigurationMenu()
{
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y; //Determines the starting y position of the selection
	int numMenuItems = 5; //Amount of items in the menu
		
	int selection = 0;
	
	sctrlSEGetConfig(&config);
	
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	oslDrawImage(Selector);
	
	oslDrawStringf(10,5,"Configuration Menu");
	
	oslDrawStringf(10,20,"- advanced configuration");
	oslDrawStringf(10,30,"- general configuration");
	oslDrawStringf(10,40,"- system");
	oslDrawStringf(10,50,"- battery");
	oslDrawStringf(10,60,"- *****Go Back*****");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			ShowAdvancedCnfMenu();
        }
	
		if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
			ShowCnfMenu();
        }
		
		if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
			ShowSystemMenu();
        }
		
		if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
			ShowBatteryMenu();
        }
		
		if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
			mainRecoveryMenu();
        }
		
		if (osl_keys->pressed.circle)
		{
		    mainRecoveryMenu();
		}	
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
	return selection;
}

void ShowAdvancedCnfMenu(void)
{	
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y; //Determines the starting y position of the selection
	int numMenuItems = 6; //Amount of items in the menu

	int selection = 0;
	
	sctrlSEGetConfig(&config);
	
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	oslDrawImage(Selector);
	
	oslDrawStringf(10,5,"Advanced Configuration Menu");
    oslDrawStringf(10,20,"- plain modules in UMD/ISO     (currently: %s)", config.umdactivatedplaincheck ? "enabled" : "disabled");
	oslDrawStringf(10,30,"- execute PBOOT.BIN in UMD/ISO (currently: %s)", config.executebootbin ? "enabled" : "disabled");
	oslDrawStringf(10,40,"- XMB plugins                  (currently: %s)", config.xmbplugins ? "disabled" : "enabled");
	oslDrawStringf(10,50,"- GAME plugins                 (currently: %s)", config.gameplugins ? "disabled" : "enabled");
	oslDrawStringf(10,60,"- POPS plugins                 (currently: %s)", config.popsplugins ? "disabled" : "enabled");
    oslDrawStringf(10,70,"- *****Go Back*****");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			config.umdactivatedplaincheck = !config.umdactivatedplaincheck;
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }
        if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
			config.executebootbin = !config.executebootbin;
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }	
        if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
			config.xmbplugins = !config.xmbplugins;
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }	
        if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
			config.gameplugins = !config.gameplugins;
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }
        if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
			config.popsplugins = !config.popsplugins;
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }
		if (osl_keys->pressed.L)
		{
			ShowBatteryMenu();
        }
		if (osl_keys->pressed.R)
		{
			ShowCnfMenu();
        }
		if (MenuSelection == 6 && osl_keys->pressed.cross)
        {
		    ConfigurationMenu();
        }	
		if (osl_keys->pressed.circle)
		{
		    ConfigurationMenu();
		}		
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
	return selection;
}

void ShowCnfMenu(void)
{
    int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y; //Determines the starting y position of the selection
	int numMenuItems = 14; //Amount of items in the menu
		
	int selection = 0;
	
	sctrlSEGetConfig(&config);
	
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	oslDrawImage(Selector);
	
	oslDrawStringf(10,5,"Configurations Edit Menu");
	
    oslDrawStringf(10,20,"Skip Sony logo                 (currently: %s)", config.skiplogo ? "Enabled" : "Disabled");
	oslDrawStringf(10,30,"Hide corrupt icons             (currently: %s)", config.hidecorrupt ? "Enabled" : "Disabled");
	oslDrawStringf(10,40,"Game folder homebrew           (currently: %s)", config.gamekernel150 ? "1.50 Kernel" : "6.XX Kernel");
	oslDrawStringf(10,50,"Autoboot program at /PSP/GAME/BOOT/ (currently: %s)", config.startupprog ? "Enabled" : "Disabled");
	oslDrawStringf(10,60,"UMD Mode                       (currently: %s)", umdmodes[config.umdmode]);
	oslDrawStringf(10,70,"Fake region                    (currently: %s)", regions[config.fakeregion]);
	oslDrawStringf(10,80,"Use VshMenu                    (currently: %s)", vshmenuoptions[config.vshmenu]);
	oslDrawStringf(10,90,"XMB Usb Device                 (currently: %s)", usbdevices[config.usbdevice]);
	oslDrawStringf(10,100,"Use network update             (currently: %s)", config.notusedaxupd ? "Disabled" : "Enabled");
	oslDrawStringf(10,110,"Hide PIC1.PNG and PIC0.PNG     (currently: %s)", config.hidepics ? "Enabled" : "Disabled");
    oslDrawStringf(10,120,"Use version.txt                (currently: %s)", config.useversiontxt ? "Enabled" : "Disabled");
	oslDrawStringf(10,130,"Use Slim colors on Classic PSP (currently: %s)", config.slimcolors ? "Enabled" : "Disabled");
    oslDrawStringf(10,140,"Hide MAC address               (currently: %s)", config.hidemac ? "Enabled" : "Disabled");
	oslDrawStringf(10,150,"- *****Go Back*****");

		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			config.skiplogo = !config.skiplogo;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
			config.hidecorrupt = !config.hidecorrupt;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }	
        if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
			config.gamekernel150 = !config.gamekernel150;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }	
        if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
			config.startupprog = !config.startupprog;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
			if(config.umdmode == MODE_UMD)
                config.umdmode = MODE_MARCH33;
            else if(config.umdmode == MODE_MARCH33)
                config.umdmode = MODE_NP9660;
            else if(config.umdmode == MODE_NP9660)
                config.umdmode = MODE_OE_LEGACY;
            else if(config.umdmode == MODE_OE_LEGACY)
                config.umdmode = MODE_UMD;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if (MenuSelection == 6 && osl_keys->pressed.cross)
        {
			if(config.fakeregion == FAKE_REGION_DISABLED)
                config.fakeregion = FAKE_REGION_JAPAN;
            else if(config.fakeregion == FAKE_REGION_JAPAN)
                config.fakeregion = FAKE_REGION_AMERICA;
            else if(config.fakeregion == FAKE_REGION_AMERICA)
                config.fakeregion = FAKE_REGION_EUROPE;
            else if(config.fakeregion == FAKE_REGION_EUROPE)
                config.fakeregion = FAKE_REGION_KOREA;
            else if(config.fakeregion == FAKE_REGION_KOREA)
                config.fakeregion = FAKE_REGION_UNK;
            else if(config.fakeregion == FAKE_REGION_UNK)
                config.fakeregion = FAKE_REGION_UNK2;
            else if(config.fakeregion == FAKE_REGION_UNK2)
                config.fakeregion = FAKE_REGION_AUSTRALIA;
            else if(config.fakeregion == FAKE_REGION_AUSTRALIA)
                config.fakeregion = FAKE_REGION_HONGKONG;
            else if(config.fakeregion == FAKE_REGION_HONGKONG)
                config.fakeregion = FAKE_REGION_TAIWAN;
            else if(config.fakeregion == FAKE_REGION_TAIWAN)
                config.fakeregion = FAKE_REGION_RUSSIA;
            else if(config.fakeregion == FAKE_REGION_RUSSIA)
                config.fakeregion = FAKE_REGION_CHINA;
            else if(config.fakeregion == FAKE_REGION_CHINA)
                config.fakeregion = FAKE_REGION_DEBUG_TYPE_I;
            else if(config.fakeregion == FAKE_REGION_DEBUG_TYPE_I)
                config.fakeregion = FAKE_REGION_DEBUG_TYPE_II;
            else if(config.fakeregion == FAKE_REGION_DEBUG_TYPE_II)
                config.fakeregion = FAKE_REGION_DISABLED;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if (MenuSelection == 8 && osl_keys->pressed.cross)
        {
			if(config.usbdevice == USBDEVICE_MEMORYSTICK)
                config.usbdevice = USBDEVICE_FLASH0;
            else if(config.usbdevice == USBDEVICE_FLASH0)
                config.usbdevice = USBDEVICE_FLASH1;
            else if(config.usbdevice == USBDEVICE_FLASH1)
                config.usbdevice = USBDEVICE_FLASH2;
            else if(config.usbdevice == USBDEVICE_FLASH2)
                config.usbdevice = USBDEVICE_FLASH3;
            else if(config.usbdevice == USBDEVICE_FLASH3)
                config.usbdevice = USBDEVICE_UMD9660;
            else if(config.usbdevice == USBDEVICE_UMD9660)
                config.usbdevice = USBDEVICE_MEMORYSTICK;
				
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if (MenuSelection == 9 && osl_keys->pressed.cross)
        {
			config.notusedaxupd = !config.notusedaxupd;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if (MenuSelection == 10 && osl_keys->pressed.cross)
        {
			config.hidepics = !config.hidepics;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if (MenuSelection == 11 && osl_keys->pressed.cross)
        {
			config.useversiontxt = !config.useversiontxt;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if (MenuSelection == 12 && osl_keys->pressed.cross)
        {
			config.slimcolors = !config.slimcolors;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if (MenuSelection == 13 && osl_keys->pressed.cross)
        {
			config.hidemac = !config.hidemac;
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if (MenuSelection == 14 && osl_keys->pressed.cross)
        {
		    ConfigurationMenu();
        }	
		if (osl_keys->pressed.L)
		{
			ShowAdvancedCnfMenu();
        }
		if (osl_keys->pressed.R)
		{
			ShowSystemMenu();
        }
		if (osl_keys->pressed.circle)
		{
		    ConfigurationMenu();
		}			
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
	return selection;
}

void ShowSystemMenu(void)
{
    int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y; //Determines the starting y position of the selection
	int numMenuItems = 6; //Amount of items in the menu

	int selection = 0;
	
	while (!osl_quit)
  {

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	oslDrawImage(Selector);
	
	oslDrawStringf(10,5,"System Menu");
	
    oslDrawStringf(10,20,"- swap buttons");
	oslDrawStringf(10,30,"- activate WMA");
	oslDrawStringf(10,40,"- activate flash player");
	oslDrawStringf(10,50,"- use fake name");
	oslDrawStringf(10,60,"- hide MAC address");
	oslDrawStringf(10,70,"- *****Go Back*****");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Assigning...");
			swap_buttons();
			ShowSystemMenu();
        }
		if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Activating...");
			active_wma();
			ShowSystemMenu();
        }
		if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Activating...");
			active_flash();
			ShowSystemMenu();
        }
		if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Faking...");
			fake_name();
			ShowSystemMenu();
        }
		if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Hiding...");
			hide_mac();
			ShowSystemMenu();
        }
		if (MenuSelection == 6 && osl_keys->pressed.cross)
        {
		    ConfigurationMenu();
        } 
		if (osl_keys->pressed.L)
		{
			ShowCnfMenu();
        }
		if (osl_keys->pressed.R)
		{
			ShowBatteryMenu();
        }
		if (osl_keys->pressed.circle)
		{
		    ConfigurationMenu();
		}		
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
	return selection;
}

void ShowBatteryMenu(void)
{
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y; //Determines the starting y position of the selection
	int numMenuItems = 5; //Amount of items in the menu
		
	int selection = 0;
    
	while (!osl_quit)
	{

	oslStartDrawing();	
	
	oslReadKeys();
	
	oslClearScreen(RGB(0,0,0));
		
	oslDrawImageXY(recoverybg, 0, 0);
	oslDrawImage(Selector);
	
	oslDrawStringf(10,5,"Battery Menu");
	
    oslDrawStringf(10,20,"- make battery pandora");
	oslDrawStringf(10,30,"- make battery autoBoot");
	oslDrawStringf(10,40,"- make battery normal");
	oslDrawStringf(10,50,"- dump battery serial to file");
	oslDrawStringf(10,60,"- *****Go Back*****");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {			
			if(chGetPSPCreatePandora() == 1) oslDrawStringf(10,190,"Your hardware is not supported.");
			
			oslDrawStringf(10,200,"Creating Pandora Battery...");
			SetBatSer(0xFFFF, 0xFFFF);
	        oslDrawStringf(10,210,"Done");
			ShowBatteryMenu();
        }
		if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
			if(chGetPSPCreatePandora() == 1) oslDrawStringf(10,190,"Your hardware is not supported.");
			
			oslDrawStringf(10,200,"Creating AutoBoot Battery...");
			SetBatSer(0x0000, 0x0000);
			oslDrawStringf(10,210,"Done");
			ShowBatteryMenu();
        }
		if (MenuSelection == 3 && osl_keys->pressed.cross)
        {			
			if(chGetPSPCreatePandora() == 1) oslDrawStringf(10,190,"Your hardware is not supported.");
			
			oslDrawStringf(10,210,"Creating Normal Battery...");
			SetBatSer(0x5241, 0x4E44);
			oslDrawStringf(10,200,"Done");
			ShowBatteryMenu();
        }
		if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
			if(chGetPSPCreatePandora() == 1) oslDrawStringf(10,190,"Your hardware is not supported.");
			
			oslDrawStringf(10,210,"Dumping Battery Serial...");
			GetBatSer(BatSerCurrent);
			WriteFile("ms0:/batser.bin", BatSerCurrent, 4);
			oslDrawStringf(10,200,"Done");
			ShowBatteryMenu();
        }
		if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
		    ConfigurationMenu();
        } 
		if (osl_keys->pressed.L)
		{
			ShowSystemMenu();
        }
		if (osl_keys->pressed.R)
		{
			ShowAdvancedCnfMenu();
        }
		if (osl_keys->pressed.circle)
		{
		    ConfigurationMenu();
		}	
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
	return selection;
}

int mainRecoveryMenu()
{
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int numMenuItems = 9; //Amount of items in the menu
		
	int selection = 0;
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	Selector = oslLoadImageFile("android_bootable_recovery/res/images/selector.png", OSL_IN_RAM, OSL_PF_8888);
	
	roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(roboto, 0.5f, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(roboto);

	while (!osl_quit)
  {
		oslStartDrawing();	

		oslReadKeys();
		
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawImage(Selector);
		oslDrawStringf(10,5,"CWM-based Recovery v1.0\n");
		
		oslDrawStringf(10,20,"- toggle USB");
		oslDrawStringf(10,30,"- system information");
		oslDrawStringf(10,40,"- configuration");
        oslDrawStringf(10,50,"- show version.txt");
		oslDrawStringf(10,60,"- plugins");
		oslDrawStringf(10,70,"- exit");
		oslDrawStringf(10,80,"- exit to XMB");
		oslDrawStringf(10,90,"- standby");
		oslDrawStringf(10,100,"- shutdown device");
		
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
		{
			USB_Toggle();
		}
		
		if (MenuSelection == 2 && osl_keys->pressed.cross)
		{
			ShowPage1();
		}
		
		if (MenuSelection == 3 && osl_keys->pressed.cross)
		{
			ConfigurationMenu();
		}
		
		if (MenuSelection == 4 && osl_keys->pressed.cross)
		{
			ShowVersionTxt();
		}
		
		if (MenuSelection == 6 && osl_keys->pressed.cross)
		{
			oslSyncFrame();
			sceKernelDelayThread(3*1000000);
			home();
		}
		
		if (MenuSelection == 7 && osl_keys->pressed.cross)
		{
			CheckerExit();
		}
		
		if (MenuSelection == 8 && osl_keys->pressed.cross)
		{
			standby_device();
		}
		
		if (MenuSelection == 9 && osl_keys->pressed.cross)
		{
			shutdown_device();
		}
		
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
	return selection;
}