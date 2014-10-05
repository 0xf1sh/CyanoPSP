#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psppower.h>
#include <oslib/oslib.h>

#define MAX_MP3_DISPLAY			3 // max amount of files displayed on-screen.
#define MP3_DISPLAY_X			20 // X value of where the filebrowser is displayed.
#define MP3_DISPLAY_Y			108 // Y value of the filebrowser is displayed.
#define MP3_CURR_DISPLAY_Y     	90 
#define MP3DISPLAY_X        20

OSL_IMAGE *mp3bg, *cursor, *wificon, *nowplaying, *mp3_select;
OSL_FONT *pgfFont;

int MP3Scan(const char* path);
void mp3Up();
void mp3Down();
void mp3Controls();
char * mp3Browse(const char * path);
int mp3player();