#include <pspkernel.h>
#include <oslib/oslib.h>

OSL_IMAGE *background, *cursor, *wificon, *apollo, *gmail, *message, *browser, *google, *calc, *clockx, *email, *people, *calendar, *phone, *gallery, *fb, *settings,*pointer, *pointer1, *isoloadericon;

void appdrawer_loadImages()
{
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
}

void appdrawer_deleteImages()
{
	oslDeleteImage(clockx);
	oslDeleteImage(email);
	oslDeleteImage(fb);
	oslDeleteImage(settings);
	oslDeleteImage(gallery);
	oslDeleteImage(phone);
	oslDeleteImage(calc);
	oslDeleteImage(calendar);
	oslDeleteImage(people);
	oslDeleteImage(isoloadericon);
}

int appdrawer()
{	
	//loads appdrawer icons memory
	appdrawer_loadImages();
	
	setfont();

	if (!clockx || !settings || !email || !gallery || !calc || !phone || !calendar || !people || !fb || !isoloadericon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");

	while (!osl_quit)
	{
		oslStartDrawing();

		controls();	

		oslReadKeys();

		oslDrawImage(background);
		
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(apollo, 30, 35);
		oslDrawString(30,85,"Apollo");
		oslDrawImageXY(browser, 95, 35);
		oslDrawString(90,85,"Browser");
		oslDrawImageXY(calc, 157, 35);
		oslDrawString(148,85,"Calculator");
		oslDrawImageXY(calendar, 217, 38);
		oslDrawString(218,85,"Calendar");
		oslDrawImageXY(clockx, 277, 35);
		oslDrawString(283,85,"Clock");
		oslDrawImageXY(email, 337, 35);
		oslDrawString(342,85,"Email");
		oslDrawImageXY(fb, 397, 35);
		oslDrawString(388,85,"File Manager");
		oslDrawImageXY(gallery, 30, 120);
		oslDrawString(29,175,"Gallery");
		oslDrawImageXY(gmail, 97, 120);
		oslDrawString(100,175,"Gmail");
		oslDrawImageXY(isoloadericon, 157, 120);
		oslDrawString(163,175,"Game");
		oslDrawImageXY(message, 217, 120);
		oslDrawString(211,175,"Messages");
		oslDrawImageXY(people, 277, 120);
		oslDrawString(279,175,"People");
		oslDrawImageXY(phone, 337, 120);
		oslDrawString(340,175,"Phone");
		oslDrawImageXY(settings, 396, 120);
		oslDrawString(397,175,"Settings");
		oslDrawImageXY(pointer, 220, 221);
		oslDrawImageXY(pointer1, 234, 222);
		oslDrawImageXY(pointer1, 244, 222);
		
		digitaltime();
		
		battery();
		navbar_buttons();
		android_notif();
		usb_icon();
		oslDrawImage(cursor);
		
		if (osl_pad.held.square)
		{
			powermenu();
		}

		if (cursor->x >= 78 && cursor->x <= 120 && cursor->y >= 24 && cursor->y <= 70 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			internet();
		}
		
		if (osl_pad.held.L)
		{
			lockscreen();
        }
		
		if (osl_pad.held.circle)
		{
			home();
			appdrawer_deleteImages();
		}
		
		if (cursor->x >= 142 && cursor->x <= 177 && cursor->y >= 44 && cursor->y <= 60 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			calculator();
		}
		
		if (cursor->x >= 385 && cursor->x <= 430 && cursor->y >= 103 && cursor->y <= 151 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			settingsmenu();
		}
		
		if (cursor->x >= 266 && cursor->x <= 311 && cursor->y >= 25 && cursor->y <= 70 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			pspclock();
		}
		
		if (cursor->x >= 18 && cursor->x <= 65 && cursor->y >= 25 && cursor->y <= 70 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			mp3player();
		}
		
		if (cursor->x >= 147 && cursor->x <= 191 && cursor->y >= 112 && cursor->y <= 154 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			gamemenu();
		}
		
		if (cursor->x >= 387 && cursor->x <= 432 && cursor->y >= 25 && cursor->y <= 70 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			filemanage();
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			multitask();
		}
				
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
}

