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

int main(void) {

    //~ extern void v4l2_version(int *major, int *minor);
    //~ int major = v4l2_major_number;
    //~ int minor = -1;
    //~ v4l2_version(major, minor);
    //~ printf("%d.%d", major, minor);
    printf("%s\n", "Start");
    FILE* myfile = fopen("/dev/sensor", "rw");
    printf("%s\n", "Dev open!");

    int dest_size = 640 * 480;
    //~ int buffer_size = 1024;
    int buffer_size = dest_size;
    char buffer[buffer_size];
    int counter = 0;
    int retval;
    while (!feof(myfile) && counter < dest_size)
    {
        retval = fread(buffer, buffer_size, 1, myfile);
        printf("retval %i\n", retval);

        counter += buffer_size;
        printf("Read %i, %i\n", counter, buffer);
        //~ fwrite(buffer, buffer_size, buffer_size, stdout);

        //~ if (counter > 10240) break;
        //~ printf("%s",buffer);
    }

    
    retval = ioctl(myfile, V4L2_IDENT_OV7660, buffer);
    printf("retval %i\n", retval);
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
