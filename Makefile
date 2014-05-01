TARGET = CyanogenMod
OBJS = main.o appdrawer.o

INCDIR = 
CFLAGS = -G4 -Wall -O2 -g -G0
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
STDLIBS= -losl -lpng -lz \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg
LIBS=$(STDLIBS)$(YOURLIBS)


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = CyanogenMod


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak