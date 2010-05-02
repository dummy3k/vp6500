import Image, sys, struct
from leopytools.helpers import ipython

def do_it():
    img = Image.new("RGB", (240, 220))
    #~ infile = open('/tmp/blah3.bmp', 'rb')
    infile = open('/tmp/blah4', 'rb')

    #~ definitely_green = (156, 11)
    #~ definitely_blue = (47, 46)
    #~ definitely_red = (25, 165)
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
                    
            raw = infile.read(2)
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

    infile.close()
    print "Hello"

    img.show()

do_it()
