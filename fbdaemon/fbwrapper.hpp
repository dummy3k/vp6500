class FramebufferWrapper {
    public:
    const static size_t FRAMEBUFFER_WIDTH = 240;
    const static size_t FRAMEBUFFER_HEIGHT = 220;

    /**
        opens the framebuffer. afterwards you can use the returned pointer to read and write the framebuffer.
        returns NULL on errors
    */
    unsigned short* open();

    /**
        closes the framebuffer and invalidas any previous pointers on it.

    */

    void close();

    /**
        returns the pointer to the framebuffer.
        returns NULL when framebuffer is not open
        @see open()
    */
    unsigned short* getPointer();

    /**
        convinience function for saving the full content of the framebuffer.
        can be restored by calling restore()

        @see restore()
    */
    void backup();

    /**
        restores the previously backed up framebuffer contents
        @see backup()
    */

    void restore();
};
