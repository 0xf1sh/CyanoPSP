#include <oslib/oslib.h>

char checkname[256];
static int lastNumber = -1;

int FileExists(const char *file) //Check if file exists
{
FILE *f;

f = fopen(file, "rb");
if(f)
{
fclose(f);
return 1;
}
return 0;
}

static void GenScreenshotFileName(int lastNumber, char *fileName, const char *ext) //Change the file name starting from 0001
{
int a, b, c, d;

a = lastNumber / 1000;
lastNumber -= a * 1000;
b = lastNumber / 100;
lastNumber -= b * 100;
c = lastNumber / 10;
lastNumber -= c * 10;
d = lastNumber;

sprintf(fileName, "ms0:/PSP/GAME/CyanogenMod/screenshots/screenshot%i%i%i%i%s", a, b, c, d, ext);

}

void screenshot() //Takes screenshot
{
sprintf(checkname, "%s", "screenshot"); 

if(lastNumber == -1)
{
lastNumber = 0;
}

GenScreenshotFileName(lastNumber, checkname, ".png"); 
while (FileExists(checkname))
{
lastNumber++;
GenScreenshotFileName(lastNumber, checkname, ".png");
}
oslWriteImageFile(OSL_SECONDARY_BUFFER, checkname, 0);
lastNumber++;

}

