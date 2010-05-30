#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "camera.hpp"
#include "log.hpp"

#define		INIT			0x6C01
#define		SET_CONTRAST		0x6C02
#define		SET_REGISTER		0x6C03
#define		START_CAPTURE		0x6C04
#define		STOP_CAPTURE		0x6C05
#define		SET_FORMAT		0x6C06
#define		POWER_ON		0x6C07

void Camera::enableCamera() {
	logInfo("enabling camera");

	int fd, result;

	u_int16_t contrast;
	struct _fmt_struct format;
	u_int32_t initreg;

	contrast = 3;

	format.width = CAMERA_WIDTH;
	format.height = CAMERA_HEIGHT;
	format.unknown1 = 0;
	format.unknown2 = 0;

	initreg = 0xFFFFFFFF;

	fd = open("/dev/sensor", O_RDWR);
	if(fd == -1)
	{
		logError("can't open /dev/sensor");
		goto EXIT;
	}

	result = ioctl(fd, POWER_ON);			// power on IOCTL
	if(result != 0)
		logError("error in power on IOCTL, got %i",result);


	result = ioctl(fd, INIT, &initreg);		// init IOCTL
	if(result != 0)
		logError("error in init IOCTL, got %i",result);

	result = ioctl(fd, SET_FORMAT, &format);	// set format IOCTL
	if(result != 0)
		logError("error in set format IOCTL, got %i",result);

	result = ioctl(fd, START_CAPTURE);		// cpature start IOCTL
	if(result != 0)
		logError("error in start capture IOCTL, got %i",result);

	result = ioctl(fd, SET_CONTRAST, contrast);	// contrast IOCTL
	if(result != 0)
		logError("error in set contrast IOCTL, got %i",result);

/*
	result = ioctl(fd, STOP_CAPTURE);		// capture stop IOCTL
	if(result != 0)
		logError("error in stop capture IOCTL, got %i",result);
*/

EXIT:
	if(close(fd) == -1)
	{
		logError("can't close /dev/sensor");
	}
}


void Camera::disableCamera() {
	logDebug("disabling camera...");
    int fd, result;

    fd = open("/dev/sensor", O_RDWR);
    if(fd == -1)
    {
            logError("can't open /dev/sensor");
            goto EXIT;
    }

    result = ioctl(fd, STOP_CAPTURE);               // capture stop IOCTL
    if(result != 0)
        logError("error in stop capture IOCTL, got %i",result);
	else
		logDebug("successfully disabled camera device with result: %d ", result);

EXIT:
        if(close(fd) == -1)
        {
            logError("can't close /dev/sensor");
        }
}
