TARGET = CyanogenMod
OBJS = main.o appdrawer.o home.o calculator.o lock.o settingsmenu.o clock.o recoverymenu.o imports.o menu.o common.o multi.o power_menu.o \
	   apollo.o fm.o game.o mp3player.o messenger.o include/utils.o include/pgeZip.o include/ram.o screenshot.o id3.o gallery.o
	   
BUILD_PRX = 1
PSP_FW_VERSION=371

CFLAGS = -O2 -g -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =

MYLIBS=
STDLIBS= -losl -lpng -lz \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg \
		 -lpspvram -lpsputility -lpspkubridge -lpspsystemctrl_user  -lpspreg \
		 -lpspusb -lpspusbstor -lpspusbdevice -lpspmp3 -lmad -lpspaudiocodec -lpspsystemctrl_kernel -lpspvshctrl -lpsprtc_driver -lpspreg_driver
		 
LIBS=$(STDLIBS) $(MYLIBS)

LDFLAGS =
PSP_LARGE_MEMORY = 1
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = CyanoPSP
PSP_EBOOT_ICON = ICON0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak