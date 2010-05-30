struct _PRP_STRUCT *prp_regs;
#define PRP_BASE	0x10026400
#define PRP_SIZE	132
#define FB_START_REG	0x10021000

struct _PRP_STRUCT
{
        u_int32_t       PRP_CNTL;                       // 10026400
        u_int32_t       PRP_INTRCNTL;                   // 10026404
        u_int32_t       PRP_INTRSTATUS;                 // 10026408
        u_int32_t       PRP_SOURCE_Y_PTR;               // 1002640C
        u_int32_t       PRP_SOURCE_CB_PTR;              // 10026410
        u_int32_t       PRP_SOURCE_CR_PTR;              // 10026414
        u_int32_t       PRP_DEST_RGB1_PTR;              // 10026418
        u_int32_t       PRP_DEST_RGB2_PTR;              // 1002641C
        u_int32_t       PRP_DEST_Y_PTR;                 // 10026420
        u_int32_t       PRP_DEST_CB_PTR;                // 10026424
        u_int32_t       PRP_DEST_CR_PTR;                // 10026428
        u_int32_t       PRP_SOURCE_FRAME_SIZE;          // 1002642C
        u_int32_t       PRP_CH1_LINE_STRIDE;            // 10026430
        u_int32_t       PRP_SRC_PIXEL_FORMAT_CNTL;      // 10026434
        u_int32_t       PRP_CH1_PIXEL_FORMAT_CNTL;      // 10026438
        u_int32_t       PRP_CH1_OUT_IMAGE_SIZE;         // 1002643C
        u_int32_t       PRP_CH2_OUT_IMAGE_SIZE;         // 10026440
        u_int32_t       PRP_SOURCE_LINE_STRIDE;         // 10026444
        u_int32_t       PRP_CSC_COEF_012;               // 10026448
        u_int32_t       PRP_CSC_COEF_345;               // 1002644C
        u_int32_t       PRP_CSC_COEF_678;               // 10026450
        u_int32_t       PRP_CH1_RZ_HORI_COEF1;          // 10026454
        u_int32_t       PRP_CH1_RZ_HORI_COEF2;          // 10026458
        u_int32_t       PRP_CH1_RZ_HORI_VALID;          // 1002645C
        u_int32_t       PRP_CH1_RZ_VERT_COEF1;          // 10026460
        u_int32_t       PRP_CH1_RZ_VERT_COEF2;          // 10026464
        u_int32_t       PRP_CH1_RZ_VERT_VALID;          // 10026468
        u_int32_t       PRP_CH2_RZ_HORI_COEF1;          // 1002646C
        u_int32_t       PRP_CH2_RZ_HORI_COEF2;          // 10026470
        u_int32_t       PRP_CH2_RZ_HORI_VALID;          // 10026474
        u_int32_t       PRP_CH2_RZ_VERT_COEF1;          // 10026478
        u_int32_t       PRP_CH2_RZ_VERT_COEF2;          // 1002647C
        u_int32_t       PRP_CH2_RZ_VERT_VALID;          // 10026480
} pregs;


void camera_redirect_start();
void camera_redirect_stop();
