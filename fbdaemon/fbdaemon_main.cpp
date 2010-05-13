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

const size_t camera_width = 352;
const size_t camera_height = 288;

void ex_program(int sig);

void ex_program(int sig) {
	printf("Wake up call ... !!! - Catched signal: %d ... !!\n", sig);
	disable_camera();
	(void) signal(SIGINT, SIG_DFL);
	exit(0);
}

int main(int argc, const char* argv[])
{
    printf("hello world\n");
	return 0;
}
