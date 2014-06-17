#include <pspctrl.h>
#include <pspdisplay.h>
#include <stdio.h>
#include <pspaudio.h>
#include <pspmp3.h>
#include <psputility.h>
#include <oslib/oslib.h>

static int isrunning = 1;
 SceCtrlData pad;

//declaration
OSL_IMAGE *mp3bg, *cursor, *wificon, *navbar;

#define MP3FILE "ms0:/MUSIC/Test.mp3"

/* Define printf, just to make typing easier */
#define printf  pspDebugScreenPrintf

// Input and Output buffers
char	mp3Buf[16*1024]  __attribute__((aligned(64)));
short	pcmBuf[16*(1152/2)]  __attribute__((aligned(64)));


// Macro to allow formatted input without having to use stdargs.h
#define ERRORMSG(...) { char msg[128]; sprintf(msg,__VA_ARGS__); error(msg); }

// Print out an error message and quit after user input
void error( char* msg )
{
	pspDebugScreenClear();
	pspDebugScreenSetXY(0, 0);
	printf(msg);
	printf("Press X to quit.\n");
	while (isrunning)
	{
		if (osl_pad.held.cross)
			break;
		sceDisplayWaitVblankStart();
	}
	sceKernelExitGame();
}

int fillStreamBuffer( int fd, int handle )
{
	char* dst;
	int write;
	int pos;
	// Get Info on the stream (where to fill to, how much to fill, where to fill from)
	int status = sceMp3GetInfoToAddStreamData( handle, &dst, &write, &pos);
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3GetInfoToAddStreamData returned 0x%08X\n", status);
	}

	// Seek file to position requested
	status = sceIoLseek32( fd, pos, SEEK_SET );
	if (status<0)
	{
		ERRORMSG("ERROR: sceIoLseek32 returned 0x%08X\n", status);
	}
	
	// Read the amount of data
	int read = sceIoRead( fd, dst, write );
	if (read < 0)
	{
		ERRORMSG("ERROR: Could not read from file - 0x%08X\n", read);
	}
	
	if (read==0)
	{
		// End of file?
		return 0;
	}
	
	// Notify mp3 library about how much we really wrote to the stream buffer
	status = sceMp3NotifyAddStreamData( handle, read );
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3NotifyAddStreamData returned 0x%08X\n", status);
	}
	
	return (pos>0);
}

int apollo(int argc, char *argv[])
{
    SetupCallbacks();
	
    // Setup Pad
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(0);

	// Load modules
	int status = sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);
	if (status<0)
	{
		ERRORMSG("ERROR: sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC) returned 0x%08X\n", status);
	}
	
	status = sceUtilityLoadModule(PSP_MODULE_AV_MP3);
	if (status<0)
	{
		ERRORMSG("ERROR: sceUtilityLoadModule(PSP_MODULE_AV_MP3) returned 0x%08X\n", status);
	}
	
	// Open the input file
	int fd = sceIoOpen( MP3FILE, PSP_O_RDONLY, 0777 );
	if (fd<0)
	{
		ERRORMSG("ERROR: Could not open file '%s' - 0x%08X\n", MP3FILE, fd);
	}
	
	// Init mp3 resources
	status = sceMp3InitResource();
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3InitResource returned 0x%08X\n", status);
	}
	
	// Reserve a mp3 handle for our playback
	SceMp3InitArg mp3Init;
	mp3Init.mp3StreamStart = 0;
	mp3Init.mp3StreamEnd = sceIoLseek32( fd, 0, SEEK_END );
	mp3Init.unk1 = 0;
	mp3Init.unk2 = 0;
	mp3Init.mp3Buf = mp3Buf;
	mp3Init.mp3BufSize = sizeof(mp3Buf);
	mp3Init.pcmBuf = pcmBuf;
	mp3Init.pcmBufSize = sizeof(pcmBuf);
	
	int handle = sceMp3ReserveMp3Handle( &mp3Init );
	if (handle<0)
	{
		ERRORMSG("ERROR: sceMp3ReserveMp3Handle returned 0x%08X\n", handle);
	}
	
	// Fill the stream buffer with some data so that sceMp3Init has something to work with
	fillStreamBuffer( fd, handle );
	
	status = sceMp3Init( handle );
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3Init returned 0x%08X\n", status);
	}
	
	int channel = -1;
	int samplingRate = sceMp3GetSamplingRate( handle );
	int numChannels = sceMp3GetMp3ChannelNum( handle );
	int lastDecoded = 0;
	int volume = PSP_AUDIO_VOLUME_MAX;
	int numPlayed = 0;
	int paused = 0;
	int lastButtons = 0;
	int loop = 0;
	while (isrunning)
	{
		sceDisplayWaitVblankStart();
		pspDebugScreenSetXY(0, 0);
		printf("PSP Mp3 Sample v1.0 by Raphael\n\n");
		printf("Playing '%s'...\n", MP3FILE);
		printf(" %i Hz\n", samplingRate);
		printf(" %i kbit/s\n", sceMp3GetBitRate( handle ));
		printf(" %s\n", numChannels==2?"Stereo":"Mono");
		printf(" %s\n\n", loop==0?"No loop":"Loop");
		int playTime = samplingRate>0?numPlayed / samplingRate:0;
		printf(" Playtime: %02i:%02i\n", playTime/60, playTime%60 );
		printf("\n\n\nPress CIRCLE to Pause/Resume playback\nPress TRIANGLE to reset playback\nPress CROSS to switch loop mode\nPress SQUARE to stop playback and quit\n");
		
		if (!paused)
		{
			// Check if we need to fill our stream buffer
			if (sceMp3CheckStreamDataNeeded( handle )>0)
			{
				fillStreamBuffer( fd, handle );
			}

			// Decode some samples
			short* buf;
			int bytesDecoded;
			int retries = 0;
			// We retry in case it's just that we reached the end of the stream and need to loop
			for (;retries<1;retries++)
			{
				bytesDecoded = sceMp3Decode( handle, &buf );
				if (bytesDecoded>0)
					break;
				
				if (sceMp3CheckStreamDataNeeded( handle )<=0)
					break;
				
				if (!fillStreamBuffer( fd, handle ))
				{
					numPlayed = 0;
				}
			}
			if (bytesDecoded<0 && bytesDecoded!=0x80671402)
			{
				ERRORMSG("ERROR: sceMp3Decode returned 0x%08X\n", bytesDecoded);
			}
			
			// Nothing more to decode? Must have reached end of input buffer
			if (bytesDecoded==0 || bytesDecoded==0x80671402)
			{
				paused = 1;
				sceMp3ResetPlayPosition( handle );
				numPlayed = 0;
			}
			else
			{
				// Reserve the Audio channel for our output if not yet done
				if (channel<0 || lastDecoded!=bytesDecoded)
				{
					if (channel>=0)
						sceAudioSRCChRelease();
					
					channel = sceAudioSRCChReserve( bytesDecoded/(2*numChannels), samplingRate, numChannels );
				}
				// Output the decoded samples and accumulate the number of played samples to get the playtime
				numPlayed += sceAudioSRCOutputBlocking( volume, buf );
			}
		}
		
		if (pad.Buttons!=lastButtons)
		{
			if (osl_pad.held.circle)
			{
				paused ^= 1;
			}
			
			if (osl_pad.held.triangle)
			{
				// Reset the stream and playback status
				sceMp3ResetPlayPosition( handle );
				numPlayed = 0;
			}
			
			if (osl_pad.held.cross)
			{
				loop = (loop==0?-1:0);
				status = sceMp3SetLoopNum( handle, loop );
				if (status<0)
				{
					ERRORMSG("ERROR: sceMp3SetLoopNum returned 0x%08X\n", status);
				}
			}
			
			if (osl_pad.held.square)
			{
				break;
			}
			
			lastButtons = pad.Buttons;
		}
    }
    
    // Cleanup time...
    if (channel>=0)
		sceAudioSRCChRelease();
	
	status = sceMp3ReleaseMp3Handle( handle );
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3ReleaseMp3Handle returned 0x%08X\n", status);
	}
	
	status = sceMp3TermResource();
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3TermResource returned 0x%08X\n", status);
	}
	
	status = sceIoClose( fd );
	if (status<0)
	{
		ERRORMSG("ERROR: sceIoClose returned 0x%08X\n", status);
	}
	
    sceKernelExitGame();

    return 0;
}

int mp3player()
{
	//loads our images into memory
	mp3bg = oslLoadImageFilePNG("system/home/menu/mp3bg.png", OSL_IN_RAM, OSL_PF_8888);
	
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	//Set fonts
	oslSetFont(pgfFont);
	
	//Debugger
	if (!mp3bg || !cursor || !wificon || !navbar)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	while (!osl_quit)
  {
		//Draws images onto the screen
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
			
		//Initiate the PSP's controls
		oslReadKeys();
		
		//Print the images onto the screen
		oslDrawImageXY(mp3bg, 0, 19);
		oslDrawImageXY(navbar, 110, 237);
		oslDrawImageXY(wificon, 387, 1);
		
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
		
		oslEndDrawing();
        
        oslEndFrame();
        oslSyncFrame();

	    //For sleep
        oslAudioVSync();
	}
}


