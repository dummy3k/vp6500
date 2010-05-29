#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "camera.hpp"
#include "rpr_redirect.hpp"
#include "fbwrapper.hpp"

void ex_program(int sig);

int main(int argc, const char* argv[])
{
    signal(SIGINT, ex_program); // register clean shutdown function
    printf("starting fbdaemon\n");

    short* fb = fb_open();

    fb_backup();

    int x,y;

    for(x=0; x<fb_width; x++) {
        for(y=0; y<fb_height; y++) {
            short pixel = fb[y*camera_width+x];
            printf("pixel at [%d, %d] is %04X\n", x,y,pixel);
        }
    }

    fb_restore();
    fb_close();
	return 0;
}


void ex_program(int sig) {

	printf("Catched signal: %d\n", sig);
	signal(SIGINT, SIG_DFL);

	fb_close();
	exit(0);
}

