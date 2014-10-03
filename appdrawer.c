#include <pspkernel.h>
#include <oslib/oslib.h>
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "include/screenshot.h"

OSL_IMAGE *background, *cursor, *wificon, *apollo, *gmail, *messengericon, *browser, *google, *calc, *clockx, *email, *people, *calendar, *phone, *gallery, *fb, *settings,*pointer, *pointer1, *isoloadericon;

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
	int apollo_x = 30;
	int browser_x = 95;
	int browser_text_x = 90;
	int calc_x = 157;
	int calc_text_x = 148;
	int calendar_x = 217;
	int calendar_text_x = 218;
	int clock_x = 277;
	int clock_text_x = 283;
	int email_x = 337;
	int email_text_x = 342;
	int fm_x = 397;
	int fm_text_x = 388;
	int gallery_text_x = 29;
	int gmail_x = 97;
	int gmail_text_x = 100;
	int game_icon = 157;
	int game_text_icon = 163;
	int messenger_x = 217;
	int messenger_text_x = 211; 
	int people_x = 277;
	int people_text_x = 279;
	int phone_x = 337;
	int phone_text_x = 340;
	int settings_x = 396;
	int settings_text_x = 397;

	//loads appdrawer icons memory
	appdrawer_loadImages();
	setfont();

	int highlight = 0;
	
	if (!clockx || !settings || !email || !gallery || !calc || !phone || !calendar || !people || !fb || !isoloadericon)
		oslDebug("It seems certain files necessary for the program to run are missing. Please make sure you have all the files required to run the program.");
		
	while (!osl_quit)
	{
		oslStartDrawing();

		controls();	

		oslDrawImage(background);
		
		oslDrawImageXY(wificon, 375, 1);
		oslDrawImageXY(apollo, apollo_x, 35);
		oslDrawString(apollo_x,85,"Apollo");
		oslDrawImageXY(browser, browser_x, 35);
		oslDrawString(browser_text_x,85,"Browser");
		oslDrawImageXY(calc, calc_x, 35);
		oslDrawString(calc_text_x,85,"Calculator");
		oslDrawImageXY(calendar, calendar_x, 38);
		oslDrawString(calendar_text_x,85,"Calendar");
		oslDrawImageXY(clockx, clock_x, 35);
		oslDrawString(clock_text_x,85,"Clock");
		oslDrawImageXY(email, email_x, 35);
		oslDrawString(email_text_x,85,"Email");
		oslDrawImageXY(fb, fm_x, 35);
		oslDrawString(fm_text_x,85,"File Manager");
		oslDrawImageXY(gallery, apollo_x, 120);
		oslDrawString(gallery_text_x,175,"Gallery");
		oslDrawImageXY(gmail, gmail_x, 120);
		oslDrawString(gmail_text_x,175,"Gmail");
		oslDrawImageXY(isoloadericon, game_icon, 120);
		oslDrawString(game_text_icon,175,"Game");
		oslDrawImageXY(messengericon, messenger_x, 120);
		oslDrawString(messenger_text_x,175,"Messages");
		oslDrawImageXY(people, people_x, 120);
		oslDrawString(people_text_x,175,"People");
		oslDrawImageXY(phone, phone_x, 120);
		oslDrawString(phone_text_x,175,"Phone");
		oslDrawImageXY(settings, settings_x, 120);
		oslDrawString(settings_text_x,175,"Settings");
		
		digitaltime(420,4,458);
		battery();
		navbar_buttons();
		
		if (settings_x >= 1 && fm_x >= 1 && settings_x <= 500 && fm_x <= 500)
		{
		oslDrawImageXY(pointer1, 221, 222);
		oslDrawImageXY(pointer, 229, 221);
		oslDrawImageXY(pointer1, 243, 222);
		}
		else if (settings_x <= -10 && fm_x <= -10)
		{
		oslDrawImageXY(pointer1, 221, 222);
		oslDrawImageXY(pointer1, 231, 222);
		oslDrawImageXY(pointer, 240, 221);
		}
		else
		{
		oslDrawImageXY(pointer, 220, 221);
		oslDrawImageXY(pointer1, 234, 222);
		oslDrawImageXY(pointer1, 244, 222);
		}
		
		android_notif();
		oslDrawImage(cursor);
			
		if (osl_pad.held.left && (osl_pad.held.cross))
		{
		apollo_x = apollo_x-10;
		browser_x = browser_x-10;
		browser_text_x = browser_text_x-10;
		calc_x = calc_x-10;
		calc_text_x = calc_text_x-10;
		calendar_x = calendar_x-10;
		calendar_text_x = calendar_text_x-10;
		clock_x = clock_x-10;
		clock_text_x = clock_text_x-10;
		email_x = email_x-10;
		email_text_x = email_text_x-10;
		fm_x = fm_x-10;
		fm_text_x = fm_text_x-10;
		gallery_text_x = gallery_text_x-10;
		gmail_x = gmail_x-10;
		gmail_text_x = gmail_text_x-10;
		game_icon = game_icon-10;
		game_text_icon = game_text_icon-10;
		messenger_x = messenger_x-10;
		messenger_text_x = messenger_text_x-10;
		people_x = people_x-10;
		people_text_x = people_text_x-10;
		phone_x = phone_x-10;
		phone_text_x = phone_text_x-10;
		settings_x = settings_x-10;
		settings_text_x = settings_text_x-10;
		}
		
		else if (osl_pad.held.right && (osl_pad.held.cross))
		{
		apollo_x = apollo_x+10;
		browser_x = browser_x+10;
		browser_text_x = browser_text_x+10;
		calc_x = calc_x+10;
		calc_text_x = calc_text_x+10;
		calendar_x = calendar_x+10;
		calendar_text_x = calendar_text_x+10;
		clock_x = clock_x+10;
		clock_text_x = clock_text_x+10;
		email_x = email_x+10;
		email_text_x = email_text_x+10;
		fm_x = fm_x+10;
		fm_text_x = fm_text_x+10;
		gallery_text_x = gallery_text_x+10;
		gmail_x = gmail_x+10;
		gmail_text_x = gmail_text_x+10;
		game_icon = game_icon+10;
		game_text_icon = game_text_icon+10;
		messenger_x = messenger_x+10;
		messenger_text_x = messenger_text_x+10;
		people_x = people_x+10;
		people_text_x = people_text_x+10;
		phone_x = phone_x+10;
		phone_text_x = phone_text_x+10;
		settings_x = settings_x+10;
		settings_text_x = settings_text_x+10;
		}
		
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
		
		if (cursor->x >= 142 && cursor->x <= 177 && cursor->y >= 44 && cursor->y <= 60 && osl_pad.held.triangle)
		{
			highlight = 1;
			if (highlight == 1)
			{
			oslDrawImageXY(calc, cursor->x - 5, cursor->y - 5);
			}
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
		
		if (cursor->x >= 210 && cursor->x <= 268 && cursor->y >= 118 && cursor->y <= 170 && osl_pad.held.cross)
		{
			appdrawer_deleteImages();
			messenger();
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
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}
				
		oslEndDrawing();
		oslSyncFrame();	
        oslAudioVSync();
	}
}

