#include <stdio.h>
#include <stdlib.h>
//~ #define __KERNEL__
//~ #include <linux>
//~ #include <videodevx/videodev.h>
//~ #include <linux/videodev2.h>
//~ #include <media/v4l2-chip-ident.h>

enum {
	/* general idents: reserved range 0-49 */
	V4L2_IDENT_NONE      = 0,       /* No chip matched */
    V4L2_IDENT_OV7660 = 253,
};

int main2(void) {

    //~ extern void v4l2_version(int *major, int *minor);
    //~ int major = v4l2_major_number;
    //~ int minor = -1;
    //~ v4l2_version(major, minor);
    //~ printf("%d.%d", major, minor);
    int file = fopen('/dev/sensor', 'r');
    //~ int retval = ioctl(file, V4L2_IDENT_OV7660, 0);
    int retval = ioctl(file, 253, 0);
    fclose(file);
    
	return 0;
}


int main(void)
{
    int i=1;
    //~ 75670
    //~ 30600
    for (i=30600;i<2147483648;i++) {
        if (i % 100 == 0) {
            printf("hello %i\r\n", i);
        }
        //~ system("echo Hi");
        char cmd[1024];
        sprintf(cmd, "/bin/dd if=/dev/sensor of=/tmp/blah bs=%d count=1 2>/dev/null", i);
        //~ if (i % 100 == 0) {
            //~ printf("cmd: %s\n", cmd);
        //~ }
        int retval = system(cmd);
        //~ if (i % 100 == 0) {
            //~ printf("retval: %d\n", retval);
        //~ }
        if (retval == 0) {
            printf("WINNING i: %d\n", i);
            break;
        }
        //~ printf("%s", cmd);
        //~ system("/bin/dd if=/dev/sensor of=/tmp/blah bs=%d count=1");
    }
	return 0;
}
