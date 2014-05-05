CyanogenMod-PSP-C
=================

This is a C port of my CyanogenMod Shell. This isn't a port of cyanogenmod for android, it's a project created from scratch. It will never allow you to run android apps, the psp system doesn't have the hardware capabilities to run android. It's purpose is similar to that of "iR Shell", but with a UI inspired by CyanogenMod. If developers are intrested, feel free to contact me through my email: sauskethunderkid@gmail.com 

This is not ready for release. Kindly wait for more updates.


What's working
=================
- Cursor (using directional and analog buttons)
- Home Menu
- Touch tones, when pressing the cross button on an app/widget.
- App drawer highlighting effect. 
- Battery functions. (Displays the battery percentage, by using icons.)

What's yet to work
=================
- Internet
- App drawer
- Notification slider
- Apollo music player
- Fully functional calculator app
- A little settings tweaker (possibly to change battery icons and such)
- Boot animation (I'll use a set of 20 images in a zip file probably - These images can be replaced by using another zip with the same file names and format)

What's not working
=================
- Everything else

Compiling it yourself 
=================
This method only allows you to compile on Windows. I'm not sure how you'd go about compiling it on Linux sorry.

1. Ensure you have PSP SDK installed. (I recommend the latest minimalist SDK, http://sourceforge.net/projects/minpspw/)
2. Make sure you've set your envoirnemt variables to PSPSDK

   Go to Control Panel -> System -> Advanced system settings -> Advanced tab -> Environment Variables. A window with two     tables should pop up. In the first table select Path and then click Edit. In variable value add:
   
   ;C:\pspsdk\bin
   
3. Install OsLib Mod v2 from here https://github.com/dogo/oslibmodv2 - Instructions included.
4. Run compile.bat


License can be found here https://github.com/joel16/CyanogenMod-PSP-C/blob/master/License

