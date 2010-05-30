#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

//~ #define __KERNEL__
//~ #include <linux>
//~ #include <videodevx/videodev.h>
//~ #include <linux/videodev2.h>
//~ #include <media/v4l2-chip-ident.h>

//enum {
	///* general idents: reserved range 0-49 */
	//V4L2_IDENT_NONE      = 0,       /* No chip matched */
    //V4L2_IDENT_OV7660 = 253,
//};

//~ fromhttp://www.mikrocontroller.net/topic/172616#1657696
#define IOTCL_INIT      0x6C01   //init the sensor
#define IOTCL_CONTRAST  0x6C02   // set the contrast, values range from 0 to 4 inclusive
#define IOTCL_CAPREG    0x6C03   // set the cameras register. no idea what that does
#define IOTCL_START     0x6C04   // start capture
#define IOTCL_STOP      0x6C05   // stop capture
#define IOTCL_FORMAT    0x6C06   // set format, want's a struct
                                 // consisting of uint16's for width,
                                 // height, and two other, yet unknown
                                 // parameters
#define IOTCL_POWER_ON  0x6C07   // power on the camera

void echo(char* buffer) {
    printf("%s\n", buffer);
}

int main(void) {
    echo("Start");
    FILE* myfile = fopen("/dev/sensor", "rw");
    printf("Dev open, addr: %i\n", myfile);
    int retval = ioctl(myfile, 0x6C01);
    printf("retval %i\n", retval);

    //int dest_size = 640 * 480;
    ////~ int buffer_size = 1024;
    //int buffer_size = dest_size;
    //char buffer[buffer_size];
    //int counter = 0;
    //int retval;
    //while (!feof(myfile) && counter < dest_size)
    //{
        //retval = fread(buffer, buffer_size, 1, myfile);
        //printf("retval %i\n", retval);

        //counter += buffer_size;
        //printf("Read %i, %i\n", counter, buffer);
        ////~ fwrite(buffer, buffer_size, buffer_size, stdout);

        ////~ if (counter > 10240) break;
        ////~ printf("%s",buffer);
    //}

    
    //~ retval = ioctl(myfile, V4L2_IDENT_OV7660, buffer);
    //~ printf("retval %i\n", retval);
    //~ int retval = ioctl(file, 253, 0);
    fclose(myfile);
    
	return 0;
}


//int main2(void)
//{
    //int i=1;
    ////~ 75670
    ////~ 30600
    //for (i=30600;i<2147483648;i++) {
        //if (i % 100 == 0) {
            //printf("hello %i\r\n", i);
        //}
        ////~ system("echo Hi");
        //char cmd[1024];
        //sprintf(cmd, "/bin/dd if=/dev/sensor of=/tmp/blah bs=%d count=1 2>/dev/null", i);
        ////~ if (i % 100 == 0) {
            ////~ printf("cmd: %s\n", cmd);
        ////~ }
        //int retval = system(cmd);
        ////~ if (i % 100 == 0) {
            ////~ printf("retval: %d\n", retval);
        ////~ }
        //if (retval == 0) {
            //printf("WINNING i: %d\n", i);
            //break;
        //}
        ////~ printf("%s", cmd);
        ////~ system("/bin/dd if=/dev/sensor of=/tmp/blah bs=%d count=1");
    //}
	//return 0;
//}
