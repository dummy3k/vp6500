
#define         INIT                    0x6C01
#define         SET_CONTRAST            0x6C02
#define         SET_REGISTER            0x6C03
#define         START_CAPTURE           0x6C04
#define         STOP_CAPTURE            0x6C05
#define         SET_FORMAT              0x6C06
#define         POWER_ON                0x6C07

struct _fmt_struct
{
        u_int16_t       width;
        u_int16_t       height;
        u_int16_t       unknown1;
        u_int16_t       unknown2;
};

class Camera {
    public: 
    const static size_t CAMERA_WIDTH = 352;
    const static size_t CAMERA_HEIGHT = 288;

    void enableCamera();
    void disableCamera();
};

