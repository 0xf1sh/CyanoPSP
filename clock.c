#include <pspkernel.h>
#include <psprtc.h>
#include "glib2d.h"

pspTime time;

void drawBorder() // A circle.
{
  int i, n = 42;
  float size = 80.f;

  g2dBeginLines(G2D_STRIP);

  g2dSetCoordXY(G2D_SCR_W/2,G2D_SCR_H/2);
  g2dSetColor(LITEGRAY);

  for (i=0; i!=n+1; i++)
  {
    g2dPush();
    g2dSetCoordXYRelative(0.f,-size);
    g2dAdd();
    g2dPop();

    g2dSetRotationRelative(360.f/n);
  }

  g2dEnd();
}


void drawClockHands()
{
  g2dBeginLines(G2D_VOID);

  g2dSetCoordXY(G2D_SCR_W/2,G2D_SCR_H/2);

  // Hours
  g2dPush();
  g2dSetColor(BLACK);
  g2dSetRotation((time.hour%12+
                time.minutes/60.f+
                time.seconds/3600.f)*360.f/12.f);
  g2dAdd();
  g2dSetCoordXYRelative(0.f,-30.f);
  g2dAdd();
  g2dPop();

  // Minutes
  g2dPush();
  g2dSetColor(BLACK);
  g2dSetRotation((time.minutes+
                time.seconds/60.f)*360.f/60.f);
  g2dAdd();
  g2dSetCoordXYRelative(0.f,-70.f);
  g2dAdd();
  g2dPop();

  // Seconds
  g2dPush();
  g2dSetColor(RED);
  g2dSetAlpha(255);
  g2dSetRotation(time.seconds*360.f/60.f);
  g2dAdd();
  g2dSetCoordXYRelative(0.f,-70.f);
  g2dSetAlpha(100);
  g2dAdd();
  g2dPop();

  g2dEnd();
}


int analogclock()
{
 
  while (1)
  {
    sceRtcGetCurrentClockLocalTime(&time);

    drawBorder();
    drawClockHands();

    g2dFlip(G2D_VSYNC);
  }

  sceKernelExitGame();
  return 0;
}
