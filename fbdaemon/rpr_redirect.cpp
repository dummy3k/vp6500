#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "rpr_redirect.hpp"
#include "camera.hpp"
#include "log.hpp"


void *mem_ptr;

u_int32_t backup_rgb1 = 0;
u_int32_t backup_rgb2 = 0;

// set the PRP registers, values taken from the product test script
void set_prp(u_int32_t dest1, u_int32_t dest2)
{
	logDebug("set_prp(0x%08x, 0x%08x)", dest1, dest2);
	prp_regs->PRP_CNTL = 0x3234;
	prp_regs->PRP_SRC_PIXEL_FORMAT_CNTL = 0x2CA00565;
	prp_regs->PRP_CH1_PIXEL_FORMAT_CNTL = 0x2CA00565;
	prp_regs->PRP_CSC_COEF_012 = 0x09A2583A;
	prp_regs->PRP_CSC_COEF_345 = 0x0AE2A840;
	prp_regs->PRP_CSC_COEF_678 = 0x08035829;
	prp_regs->PRP_SOURCE_LINE_STRIDE = 0x0;
	prp_regs->PRP_DEST_RGB1_PTR = dest1;
	prp_regs->PRP_DEST_RGB2_PTR = dest2;
	prp_regs->PRP_SOURCE_FRAME_SIZE = 0x01600120;	// 352x288=80256=0x13980  and 0x1600120=23068960
//	prp_regs->PRP_SOURCE_FRAME_SIZE = 287 * camera_width * camera_height; // 287=0x11F

	prp_regs->PRP_CH1_OUT_IMAGE_SIZE = 0x00B000B0;
	prp_regs->PRP_CH1_LINE_STRIDE = 0x1E0;
	prp_regs->PRP_CH1_RZ_HORI_COEF1 = 0x24;
	prp_regs->PRP_CH1_RZ_HORI_COEF2 = 0x0;
	prp_regs->PRP_CH1_RZ_HORI_VALID = 0x02000002;
	prp_regs->PRP_CH1_RZ_VERT_COEF1 = 0x24;
	prp_regs->PRP_CH1_RZ_VERT_COEF2 = 0x0;
	prp_regs->PRP_CH1_RZ_VERT_VALID = 0x02000002;

	prp_regs->PRP_CNTL = 0x2235;
}

void _redirect_camera_to_fb(int stop);

void camera_redirect_start() {
    _redirect_camera_to_fb(0);
}

void camera_redirect_stop() {
    _redirect_camera_to_fb(0);
}

void _redirect_camera_to_fb(int stop) {
        int fd, psize, offset;
	u_int32_t fb_base;
	u_int32_t *fb_base_ptr;
	u_int32_t camera_buffer;
        const int camera_offset = Camera::CAMERA_HEIGHT * 30;
	const size_t buffer_size = 2 * Camera::CAMERA_WIDTH * Camera::CAMERA_HEIGHT;

	// open the memory interface
	fd = open("/dev/mem", O_RDWR);
	if(fd == -1)
	{
		logError("can't open /dev/mem");
		goto EXIT;
	}

	// get the pagesize
	psize = getpagesize();

	logDebug("pagesize = %i", psize);

	// calculate the offset from mapable memory address (multiple of pagesize) to the register holding the frame buffer address
	offset = FB_START_REG % psize;
	logDebug("fbreg offset = %X", offset);

	// map that register to a local pointer
	mem_ptr = mmap((void*) 0x00, offset+4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FB_START_REG-offset);
	if(mem_ptr == (void*)-1)
	{
		logError("can't map FB START registers");
		goto EXIT;
	}

	// retrieve the framebuffer memory address from that register
	fb_base_ptr = (u_int32_t*)(((unsigned char*)mem_ptr) + offset);
	fb_base = fb_base_ptr[0];

	camera_buffer = fb_base;


	camera_buffer += camera_offset;

	logDebug("camera offset: %8X", camera_offset);
	logDebug("camera resolution %dx%d", Camera::CAMERA_HEIGHT, Camera::CAMERA_HEIGHT);

	//camera_buffer -= camera_buffer % psize;

	logDebug("fbreg base = %8X", fb_base);
	logDebug("camera base = %8X", camera_buffer);

	// unmap the memory
	if(munmap((void*)mem_ptr, offset+4) == -1)
	{
		logError("can't unmap FB START registers");
		goto EXIT;
	}

	offset = PRP_BASE % psize;
	logDebug("prpreg offset = %8X", offset);

	// map local pointer to the calculated base address
	mem_ptr = mmap((void*) 0x00, offset+132, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRP_BASE-offset);

	// add the calculated offset to the mapped pointer and set the PRP struct pointer address to the result
	prp_regs = (_PRP_STRUCT*)(((unsigned char*)mem_ptr) + offset);

	if(mem_ptr == (void*)-1)
	{
		logError("can't map PRP registers");
		goto EXIT;
	}

	logDebug("prp regs base %8X", (u_int32_t)prp_regs);


    if(stop == 0) {
        logInfo("enabling camera redirect");

        backup_rgb1 = prp_regs->PRP_DEST_RGB1_PTR;
        backup_rgb2 = prp_regs->PRP_DEST_RGB2_PTR;
        set_prp(camera_buffer, camera_buffer);
    } else {
        logInfo("disabling camera redirect");

        set_prp(backup_rgb1, backup_rgb2);
        backup_rgb1 = 0;
        backup_rgb2 = 0;
    }

	msync((void*)mem_ptr, offset+132, MS_SYNC | MS_INVALIDATE);

	// unmap the memory
	if(munmap((void*)mem_ptr, offset+4) == -1)
	{
		logError("can't unmap FB START registers");
		goto EXIT;
	}

EXIT:
	// close the memory device
	if(close(fd) == -1)
	{
		logError("can't close /dev/mem");
	}
}

