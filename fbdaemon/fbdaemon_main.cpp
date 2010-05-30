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
#include "log.hpp"

void ex_program(int sig);
void cleanup();

FramebufferWrapper* fbWrapper = NULL;

int main(int argc, const char* argv[])
{
    signal(SIGINT, ex_program); // register clean shutdown function
    logInfo("starting fbdaemon");

    fbWrapper = new FramebufferWrapper();

    short* fb = fbWrapper->open();

    fbWrapper->backup();

    unsigned int x,y;

    for(x=0; x<FramebufferWrapper::FRAMEBUFFER_WIDTH; x++) {
        for(y=0; y<FramebufferWrapper::FRAMEBUFFER_HEIGHT; y++) {
            short pixel = fb[y*FramebufferWrapper::FRAMEBUFFER_HEIGHT+x];
            logInfo("pixel at [%d, %d] is %04X", x,y,pixel);
        }
    }

    cleanup();
    return 0;
}

void cleanup() {
    fbWrapper->restore();
    fbWrapper->close();

    delete fbWrapper;
    fbWrapper = NULL;

    logInfo("exiting fbdaemon");
    exit(0);
}

void ex_program(int sig) {
    logWarn("Catched signal: %d. Cleaning up ...", sig);
    signal(SIGINT, SIG_DFL);
    cleanup();
}

