#
# ECAL LI controller Makefile
#
# C.Andreopoulos <<costas.andreopoulos \at stfc.ac.uk>>, Rutherford Lab.
# 

SHELL    = /bin/sh
NAME     = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
include Make.include

#
LIB_SOURCES := $(wildcard *.c) 
LIB_OBJECTS  = $(addsuffix .$(ObjSuf), $(basename $(LIB_SOURCES)))

#
all: lib \
     libsymlink \
     pulser_emulator \
     pulser_controller

#
lib: $(LIB_OBJECTS)
	$(CC) $(CCFLAGS) $(SOFLAGS) -o $(LIB) $(LIB_OBJECTS)

#
libsymlink:
	$(SYMLINK) $(LIB) $(LIBLNK)
ifeq ($(strip $(ARCH)),macosx)
	$(SYMLINK) $(LIB) $(LIBLNK_mac)
endif

#
pulser_emulator: FORCE
	$(CC) $(CCFLAGS) -c appECalLiPulserEmulator.cc $(INCLUDES)
	$(LD) $(LDFLAGS) appECalLiPulserEmulator.o $(LIBRARIES) -o pulser_emulator

#
pulser_controller: FORCE
	$(CC) $(CCFLAGS) -c appECalLiPulserController.cc $(INCLUDES)
	$(LD) $(LDFLAGS) appECalLiPulserController.o $(LIBRARIES) -o pulser_controller

#
purge: FORCE
	$(RM) *.o *~ core *.log

#
clean: FORCE
	$(RM) *.o *~ core *.log *.so *.dylib pulser_emulator pulser_controller

FORCE:

# DO NOT DELETE
