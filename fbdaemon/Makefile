ARMBASE=/opt/VP5500/toolchain/usr/local/arm/3.3.2
INCLUDEPATH=$(ARMBASE)/include
LIBPATH=$(ARMBASE)/arm-linux/lib
ARMPATH=$(ARMBASE)/bin
TOOLPREFIX=/arm-linux-

###############################################################
#####
##### Compiler, Linker and Tools
#####
###############################################################

CC=$(ARMPATH)$(TOOLPREFIX)gcc
CPP=$(ARMPATH)$(TOOLPREFIX)g++
AS=$(ARMPATH)$(TOOLPREFIX)as
LD=$(ARMPATH)$(TOOLPREFIX)gcc
OC=$(ARMPATH)$(TOOLPREFIX)objcopy
OD=$(ARMPATH)$(TOOLPREFIX)objdump

CPUFLAGS=-mcpu=arm9
OPTFLAGS=-Os
#OPTFLAGS=

CFLAGS=$(CPUFLAGS) -c -Wall -I$(INCLUDEPATH)
ASFLAGS=$(CPUFLAGS) -D --gstabs
ROMLDFLAGS=-lc -s -Wl,-warn-common
THUMBFLAGS=

PROJECT=fbdaemon

-include Makefile.local

all: $(PROJECT)

$(PROJECT): fbdaemon_main.o camera.o rpr_redirect.o log.o fbwrapper.o
	$(CPP) $(ROMLDFLAGS) -o $(PROJECT) fbdaemon_main.o camera.o rpr_redirect.o log.o fbwrapper.o

fbdaemon_main.o:
	$(CPP) $(CFLAGS) $(THUMBFLAGS) -L $(LIBPATH) -o fbdaemon_main.o fbdaemon_main.cpp

camera.o:
	$(CPP) $(CFLAGS) $(THUMBFLAGS) -L $(LIBPATH) -o camera.o camera.cpp

rpr_redirect.o:
	$(CPP) $(CFLAGS) $(THUMBFLAGS) -L $(LIBPATH) -o rpr_redirect.o rpr_redirect.cpp

fbwrapper.o:
	$(CPP) $(CFLAGS) $(THUMBFLAGS) -L $(LIBPATH) -o fbwrapper.o fbwrapper.cpp

log.o:
	$(CPP) $(CFLAGS) $(THUMBFLAGS) -L $(LIBPATH) -o log.o log.cpp

clean:
	$(RM) -v $(PROJECT) *.o *.elf *.bin *.hex *~

### EOF
