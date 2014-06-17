#include <pspkernel.h>
#include <oslib/oslib.h>

//declaration
OSL_IMAGE *background, *cursor, *navbar, *wificon, *apollo, *gmail, *message, *browser, *google, *calc, *clockx, *email, *people, *calendar, *phone, *gallery, *fb, *settings,*pointer, *pointer1, *isoloadericon;

//definition of our sounds
OSL_SOUND *tone;

void pointerfunc()
{	
	if (osl_pad.pressed.right)
	{
	oslDrawImageXY(pointer1, 220, 221);
	oslDrawImageXY(pointer, 234, 222);
	oslDrawImageXY(pointer1, 244, 222);
	}
	
	else if (osl_pad.pressed.right)
	{
	oslDrawImageXY(pointer1, 220, 221);
	oslDrawImageXY(pointer1, 234, 222);
	oslDrawImageXY(pointer, 244, 222);
	}
	
	else 
	{
	oslDrawImageXY(pointer, 220, 221);
	oslDrawImageXY(pointer1, 234, 222);
	oslDrawImageXY(pointer1, 244, 222);
	}
}

int appdrawer()
{
	oslIntraFontInit(INTRAFONT_CACHE_MED);
		
	//loads our images into memory
	clockx = oslLoadImageFilePNG("system/home/icons/clock.png", OSL_IN_RAM, OSL_PF_8888);
	email = oslLoadImageFilePNG("system/home/icons/email.png", OSL_IN_RAM, OSL_PF_8888);
	fb = oslLoadImageFilePNG("system/home/icons/fb.png", OSL_IN_RAM, OSL_PF_8888);
	settings = oslLoadImageFilePNG("system/home/icons/settings.png", OSL_IN_RAM, OSL_PF_8888);
	gallery = oslLoadImageFilePNG("system/home/icons/gallery.png", OSL_IN_RAM, OSL_PF_8888);
	phone = oslLoadImageFilePNG("system/home/icons/phone.png", OSL_IN_RAM, OSL_PF_8888);
	calc = oslLoadImageFilePNG("system/home/icons/calc.png", OSL_IN_RAM, OSL_PF_8888);
	calendar = oslLoadImageFilePNG("system/home/icons/calendar.png", OSL_IN_RAM, OSL_PF_8888);
	people = oslLoadImageFilePNG("system/home/icons/people.png", OSL_IN_RAM, OSL_PF_8888);
	isoloadericon = oslLoadImageFilePNG("system/home/icons/isoloadericon.png", OSL_IN_RAM, OSL_PF_8888);
		
	//Load fonts:
	OSL_FONT *pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
	oslSetFont(pgfFont);	//Set fonts
	
	//Debugger
	if (!clockx || !settings || !email || !gallery || !calc || !phone || !calendar || !people || !fb || !isoloadericon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
	
	//Main loop to run the program
	while (!osl_quit)
	{
		//Draws images onto the screen
		oslStartDrawing();
		
		//calls the functions
		controls();	

		//Initiate the PSP's controls
		oslReadKeys();

		//Print the images onto the screen
		oslDrawImage(background);
		
		oslDrawImageXY(navbar, 110, 237);
		oslDrawImageXY(wificon, 387, 1);
		oslDrawImageXY(apollo, 30, 35);
		oslDrawString(50,85,"Apollo");
		oslDrawImageXY(browser, 97, 35);
		oslDrawString(115,85,"Browser");
		oslDrawImageXY(calc, 157, 35);
		oslDrawString(180,85,"Calculator");
		oslDrawImageXY(calendar, 217, 38);
		oslDrawString(246,85,"Calendar");
		oslDrawImageXY(clockx, 277, 35);
		oslDrawString(300,85,"Clock");
		oslDrawImageXY(email, 337, 35);
		oslDrawString(358,85,"Email");
		oslDrawImageXY(fb, 397, 35);
		oslDrawString(431,85,"File Manager");
		oslDrawImageXY(gallery, 30, 120);
		oslDrawString(51,175,"Gallery");
		oslDrawImageXY(gmail, 97, 120);
		oslDrawString(117,175,"Gmail");
		oslDrawImageXY(isoloadericon, 157, 120);
		oslDrawString(178,175,"Game");
		oslDrawImageXY(message, 217, 120);
		oslDrawString(239,175,"Messages");
		oslDrawImageXY(people, 277, 120);
		oslDrawString(299,175,"People");
		oslDrawImageXY(phone, 337, 120);
		oslDrawString(361,175,"Phone");
		oslDrawImageXY(settings, 396, 120);
		oslDrawString(422,175,"Settings");
		
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
		
		//Launching the browser
		if (cursor->x >= 78 && cursor->x <= 120 && cursor->y >= 24 && cursor->y <= 70 && osl_pad.held.cross)
		{
			internet();
		}
		
		if (osl_pad.held.L)
		{
			lockscreen();
        }
		
		if (osl_pad.held.circle)
		{
			home();
		}
		
		if (cursor->x >= 142 && cursor->x <= 177 && cursor->y >= 44 && cursor->y <= 60 && osl_pad.held.cross)
		{
			calculator();
		}
		
		if (cursor->x >= 385 && cursor->x <= 430 && cursor->y >= 103 && cursor->y <= 151 && osl_pad.held.cross)
		{
			settingsmenu();
		}
		
		if (cursor->x >= 266 && cursor->x <= 311 && cursor->y >= 25 && cursor->y <= 70 && osl_pad.held.cross)
		{
			pspclock();
		}
		
		if (cursor->x >= 18 && cursor->x <= 65 && cursor->y >= 25 && cursor->y <= 70 && osl_pad.held.cross)
		{
			mp3player();
		}
		
		if (cursor->x >= 387 && cursor->x <= 432 && cursor->y >= 25 && cursor->y <= 70 && osl_pad.held.cross)
		{
			filemanage();
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			home();
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

