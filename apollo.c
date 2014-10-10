#include "apollo.h"
#include "clock.h"
#include "fm.h"
#include "lock.h"
#include "mp3player.h"
#include "multi.h"
#include "power_menu.h"
#include "include/screenshot.h"
#include "include/utils.h"

int MP3Scan(const char* path )
{
	curScroll = 1;
	sprintf(lastDir, path);

	int i;
	for (i=0; i<=MAX_FILES; i++)	// erase old folders
		dirScan[i].exist = 0;

	int x;
	for (x=0; x<=MAX_FILES; x++) 
	{
		folderIcons[x].active = 0;
	}

	int fd = sceIoDopen(path);

	i = 1;
	
	if (fd) 
	{
		if (!(stricmp(path, "ms0:")==0 || (stricmp(path, "ms0:/")==0))) 
		{
			sceIoDread(fd, &g_dir);		// get rid of '.' and '..'
			sceIoDread(fd, &g_dir);

			// Create our own '..'
			folderIcons[1].active = 1; 
			sprintf(folderIcons[1].filePath, "doesn't matter");
			sprintf(folderIcons[1].name, "..");
			sprintf(folderIcons[1].fileType, "dotdot");

			x = 2;
		} 
		else 
		{
			x = 1;
		}
		
		while ( sceIoDread(fd, &g_dir) && i<=MAX_FILES ) 
		{
			sprintf( dirScan[i].name, g_dir.d_name );
			sprintf( dirScan[i].path, "%s/%s", path, dirScan[i].name );
			
			//skip . and .. entries
			if (!strcmp(".", g_dir.d_name) || !strcmp("..", g_dir.d_name)) 
			{
				memset(&g_dir, 0, sizeof(SceIoDirent));
				continue;
			};

			if (g_dir.d_stat.st_attr & FIO_SO_IFDIR) 
			{
				dirScan[i].directory = 1;
				dirScan[i].exist = 1;
			}
			else 
			{
				dirScan[i].directory = 0;
				dirScan[i].exist = 1;
			}

			dirScan[i].size = g_dir.d_stat.st_size;
			i++;
		}
	}

	sceIoDclose(fd);

	for (i=1; i<MAX_FILES; i++) {
		if (dirScan[i].exist == 0) 
		break;
		folderIcons[x].active = 1;
		sprintf(folderIcons[x].filePath, dirScan[i].path);
		sprintf(folderIcons[x].name, dirScan[i].name);

		char *suffix = strrchr(dirScan[i].name, '.');
		
		if (dirScan[i].directory == 1) 
		{      // if it's a directory
			sprintf(folderIcons[x].fileType, "fld");
		} 
		else if ((dirScan[i].directory == 0) && (suffix)) 
		{		// if it's not a directory
			sprintf(folderIcons[x].fileType, "none");
		}
		else if (!(suffix)) 
		{
			sprintf(folderIcons[x].fileType, "none");
		}
		x++;
	}

	return 1;
}

void mp3Up()
{
	current--; // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) 
	{
		curScroll--; // To do with how it scrolls
	}
}

void mp3Down()
{
	if (folderIcons[current+1].active) current++; // Add a value onto current so the ">" goes down
	if (current >= (MAX_MP3_DISPLAY+curScroll)) 
	{
		curScroll++; // To do with how it scrolls
	}
}

void mp3Upx5()
{
	current -= 5;  // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll -= 5;  // To do with how it scrolls
	}
}

void mp3Downx5()
{
	if (folderIcons[current+1].active) current += 5; // Add a value onto current so the ">" goes down
	if (current >= (MAX_DISPLAY+curScroll)) {
		curScroll += 5; // To do with how it scrolls
	}
}

void mp3FileDisplay()
{	
	oslDrawImageXY(mp3bg, 0, 19);
	oslDrawImageXY(mp3_select,8,(current - curScroll)*55+MP3_CURR_DISPLAY_Y);

	battery();
	digitaltime(420,4,458);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_MP3_DISPLAY+curScroll;i++) 
	{
		char * ext = strrchr(dirScan[i].name, '.'); //For file extension.
	
		// Handles the cursor and the display to not move past the MAX_MP3_DISPLAY.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) 
		{
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) 
		{
			current = curScroll-1;
			break;
		}
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1)
		{	
			oslDrawStringf(MP3_DISPLAY_X, (i - curScroll)*55+MP3_DISPLAY_Y, folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

void mp3Controls() //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) {
	
		if ((pad.Buttons & PSP_CTRL_DOWN) && (!(oldpad.Buttons & PSP_CTRL_DOWN))) 
		{
			mp3Down();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_UP) && (!(oldpad.Buttons & PSP_CTRL_UP))) 
		{
			mp3Up();
			timer = 0;
		}
		if ((pad.Buttons & PSP_CTRL_RIGHT) && (!(oldpad.Buttons & PSP_CTRL_RIGHT))) {
			mp3Downx5();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_LEFT) && (!(oldpad.Buttons & PSP_CTRL_LEFT))) {
			mp3Upx5();
			timer = 0;
		}
		if ((pad.Buttons & PSP_CTRL_TRIANGLE) && (!(oldpad.Buttons & PSP_CTRL_TRIANGLE))) 
		{
			if (!(stricmp(lastDir, "ms0:")==0) || (stricmp(lastDir, "ms0:/")==0)) {
				curScroll = 1;
				current = 1;
			}
		}
	}
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || ((strcmp(ext ,".MP3") == 0))) && (osl_keys->pressed.cross))
	{
		MP3Play(folderIcons[current].filePath);
	}

	if (osl_pad.held.circle)
	{
		appdrawer();
	}
	
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
		
	if (osl_pad.held.R && osl_pad.held.triangle)
	{
		screenshot();
	}
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP)) 
	{
		mp3Up();
		timer = 25;
	}
	else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN)) 
	{
		mp3Down();
		timer = 25;
	}

	if (current < 1) current = 1; // Stop the ">" from moving past the minimum files
	if (current > MAX_FILES) current = MAX_FILES; // Stop the ">" from moving past the max files

}

char * mp3Browse(const char * path)
{
	MP3Scan(path);
	dirVars();
	
	while (!osl_quit)
	{		
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));	
		
		if (!strcmp(".", g_dir.d_name) || !strcmp("..", g_dir.d_name)) 
		{
			memset(&g_dir, 0, sizeof(SceIoDirent));
			continue;
		};
		
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		mp3FileDisplay();
		mp3Controls();
		
		sceDisplayWaitVblankStart();
		
		if (strlen(returnMe) > 4) 
		{
			break;
		}
	oslEndDrawing();
	oslSyncFrame();	
    oslAudioVSync();
	}
	return returnMe;
}

int mp3player()
{	
	nowplaying = oslLoadImageFilePNG("system/app/apollo/nowplaying.png", OSL_IN_RAM, OSL_PF_8888);
	mp3bg = oslLoadImageFilePNG("system/app/apollo/mp3bg.png", OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG("system/app/apollo/mp3_select.png", OSL_IN_RAM, OSL_PF_8888);

	if (!mp3bg || !cursor || !wificon || !nowplaying)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(pgfFont);
	
	makeMusicDir();
		
	char * Directory = mp3Browse("ms0:/MUSIC");
	
	while (!osl_quit)
	{	
		oslReadKeys();
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));	
		
		centerText(480/2, 272/2, Directory, 50);

		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
		}
}