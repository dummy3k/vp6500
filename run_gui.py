if __name__ == '__main__':
    import logging.config
    #~ logging.config.fileConfig("logging.conf")
    #~ logging.basicConfig(level=logging.DEBUG)
    logging.basicConfig(level=logging.DEBUG)

import gui.camera_frame

if __name__ == '__main__':
    gui.camera_frame.main()
