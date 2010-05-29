#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "fbwrapper.hpp"
#include "camera.hpp"

#define _MODE_565_RGB16BIT(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))

     int fb_file = 0;
     short* fb_pointer = NULL;
     short* fb_backup_buffer = NULL;

    short* fb_open() {
        if(fb_file != 0) {
            printf("fb_open() was called, but fb already open\n");
            return null;
        }

        fb_file = open("/dev/fb", O_RDWR);

        if(fb_file == NULL) {
            printf("fb_open() failed: could not open /dev/fb for rw\n");
            return null;
        }

        fb_pointer = mmap(0, 2*fb_width*fb_height, PROT_WRITE, MAP_SHARED , fb_nr, 0);

        if(fb_pointer == NULL) {
            printf("fb_open() failed: could not mmap fb\n");
            return null;
        }

        fb_backup_buffer = new short[396000];

        return fb_pointer;
    }

    void fb_close() {
        if(fb_pointer != NULL) {
            munmap((void*)fb_pointer, 2*fb_width*fb_height) == -1)
        }

        if(fb_file != 0 && close(fd) == -1)
        {
            printf("can't close /dev/sensor\r\n");
        } else {
            delete fb_backup_buffer;
            fb_backup_buffer = NULL;
        }
    }

    short* fb_getpointer() {
        return fb_pointer;
    }

    void fb_backup() {
        if(fb_pointer == NULL) {
            printf("fb_backup(): please call fb_open() first!\n");
            return ;
        }

        for(int i = 0; i < 180; i++)
            for(int j = 0; j < 220; j++)
                fb_backup_buffer[240*j + i] = fb_pointer[240*j + i];

    }

    void fb_restore() {
        if(fb_pointer == NULL) {
            printf("fb_backup(): please call fb_open() first!\n");
            return ;
        }

        for(int i = 0; i < 180; i++)
            for(int j = 0; j < 220; j++)
                fb_pointer[240*j + i] = fb_backup_buffer[240*j + i];
    }


};



