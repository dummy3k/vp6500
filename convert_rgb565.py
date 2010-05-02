import Image, sys, struct
import socket
from StringIO import StringIO
from leopytools.helpers import ipython
from optfunc import optfunc

def convert_rgb565(filename):
    infile = open(filename, 'rb')
    __convert_rgb565__(infile)
    infile.close()

def __convert_rgb565__(buffer):
    img = Image.new("RGB", (240, 220))
    definitely = [('green', (156, 11)),
                  ('blue',  (47, 46)),
                  ('red',  (25, 165))]

    unique = []
    for y in range(220):
        for x in range(240):
            definitely_color = None
            for color, coords in definitely:
                if (x, y) == coords:
                    definitely_color = color
                    break
                    
            raw = buffer.read(2)
            if not raw:
                print "Unexpected no data"
                return

            raw_ushort = struct.unpack('H', raw)[0]

            #~ r = raw >> (6+5)
            #~ if x == 10 and y > 25 and y < 35:
                #~ print "%04x (%s)" % (raw, r)
            #~ continue
            
            raw_touple = ord(raw[0]), ord(raw[1])
            if definitely_color:
                print "%s\traw_ushort: %04x" % (definitely_color, raw_ushort)
                print "%s\traw_ushort: % 20s" % (definitely_color, bin(raw_ushort))
                print "%s\traw_touple: %02x, %02x" % (definitely_color, raw_touple[0], raw_touple[1])

            #~ r = raw_touple[0] >> 3
            #~ g = raw_touple[0] & 0b111
            #~ g = g << 3
            #~ g += (raw_touple[1] & 0b111000) >> 3
            #~ b = raw_touple[1] & 0b11111
            r = raw_ushort >> (6 + 5)
            g = raw_ushort >> 5 & 0b111111
            b = raw_ushort & 0b11111
            

            r *= 8
            g *= 4
            b *= 8
            if r > 255 or g > 255 or b > 255:
                print "bad values (%s, %s, %s)" % (r, g, b)
                return

            #~ tmp = r
            #~ r = b
            #~ b = tmp
            
            color_tuple = (r, g, b)

            #~ r = 0
            #~ g = 0
            #~ b = 0
            color = r
            color += g << 8
            color += b << 16
            log_msg = "<(%02x%02x) -> (%s, %s, %s) %X>" % (raw_touple[0], raw_touple[1], r, g, b, color)
            if not log_msg in unique:
                unique.append(log_msg)

            if definitely_color:
                print definitely_color + "\t" + log_msg

            img.putpixel( (x, y), color)

    for raw_touple in unique:
        #~ print raw_touple
        pass

    print "Hello"
    img.show()

def download(host, port=31337):
    print "%s:%s" % (host, port)

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
    print "Ok: %s" % len(data)
    __convert_rgb565__(StringIO(data))

if __name__ == '__main__':
    #~ optfunc.run(convert_rgb565)
    optfunc.main([convert_rgb565,download])
