import logging
import Image, sys, struct
import socket
from StringIO import StringIO
from leopytools.helpers import ipython
from optfunc import optfunc

log = logging.getLogger(__name__)

def convert_rgb565(filename, showImage=False, out_file=None):
    infile = open(filename, 'rb')
    img = __convert_rgb565__(infile, showImage)
    infile.close()

    if out_file:
        #~ ipython()()
        img.save(out_file)
        print "saved image: %s" % out_file

def convert_one(raw_ushort):
    r = raw_ushort >> (6 + 5)
    g = raw_ushort >> 5 & 0b111111
    b = raw_ushort & 0b11111

    r *= 8
    g *= 4
    b *= 8
    if r > 255 or g > 255 or b > 255:
        print "bad values (%s, %s, %s)" % (r, g, b)
        return

    color = r
    color += g << 8
    color += b << 16

    return color

def __convert_rgb565__(buffer, showImage=False):
    img = Image.new("RGB", (240, 220))
    putpixel = img.putpixel
    unique = []
    for y in range(220):
        for x in range(240):
            raw = buffer.read(2)
            if not raw:
                print "Unexpected no data"
                return

            raw_ushort = struct.unpack('H', raw)[0]
            #~ raw_touple = ord(raw[0]), ord(raw[1])

            r = raw_ushort >> (6 + 5)
            g = raw_ushort >> 5 & 0b111111
            b = raw_ushort & 0b11111

            r *= 8
            g *= 4
            b *= 8
            if r > 255 or g > 255 or b > 255:
                print "bad values (%s, %s, %s)" % (r, g, b)
                return

            color_tuple = (r, g, b)

            #~ r = 0
            #~ g = 0
            #~ b = 0
            color = r
            color += g << 8
            color += b << 16
            #~ log_msg = "<(%02x%02x) -> (%s, %s, %s) %X>" % (raw_touple[0], raw_touple[1], r, g, b, color)
            #~ if not log_msg in unique:
                #~ unique.append(log_msg)
#~ 
            #~ if definitely_color:
                #~ print definitely_color + "\t" + log_msg

            putpixel( (x, y), color)

    for raw_touple in unique:
        #~ print raw_touple
        pass

    if showImage:
        img.show()

    return img

def download(host, port=31337, showImage=False, out_file=None):
    img = __download__(host, port, showImage)
    if out_file:
        #~ ipython()()
        img.save(out_file)
        print "saved image: %s" % out_file
        
def __download__(host, port=31337, showImage=False):
    log.debug("downloading from: %s:%s" % (host, port))

    # 2*240*220 = 105600
    WHOLE_SIZE = 105600
    BUFFER_SIZE = 128

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    #~ s.send(MESSAGE)

    data = s.recv(BUFFER_SIZE)
    while (len(data) < WHOLE_SIZE):
        data += s.recv(BUFFER_SIZE)
    s.close()

    data = data[:WHOLE_SIZE]
    log.info("downloaded %d bytes" % len(data))
    return __convert_rgb565__(StringIO(data), showImage)


def main():
    #~ optfunc.run(convert_rgb565)
    optfunc.main([convert_rgb565,download])

if __name__ == '__main__':
    main()

    #~ import cProfile
    #~ profile_filename = __file__ + '.profile'
    #~ cProfile.run('main()', profile_filename)
#~ 
    #~ import pstats
    #~ p = pstats.Stats(profile_filename)
    #~ p.strip_dirs().sort_stats('cumulative').print_stats(10)
    #~ p.strip_dirs().sort_stats('time').print_stats(10)
    #~ p.strip_dirs().sort_stats('time', 'cum').print_stats(10)
    #~ p.strip_dirs().sort_stats(-1).print_stats(10)
