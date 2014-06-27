/* Part of GBOT's 138 Menu/Other.h */

/* Starting from here */

#ifndef OTHER_H
#define OTHER_H

//vhbl related
typedef struct 
{
	unsigned long APIVersion;
	char Credits[32];
	char VersionName[32];
	char *BackgroundFilename;
	char *filename;
} tMenuApi;

int xstrtoi(char * str, int len);

//launcher related
#ifdef ARK
#include <ark_pspsystemctrl_user.h>
#endif
#ifdef TN
#include <pspsystemctrl_user.h>
#endif

#define HOMEBREW_RUNLEVEL 0x141
#define ISO_RUNLEVEL 0x123
#define POPS_RUNLEVEL 0x144
#define PSN_RUNLEVEL ISO_RUNLEVEL

int cef_launch(char * file, int type, int extra);

#endif

/* To here */ 

int gamemenu();
