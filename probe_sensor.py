
size = (320, 240)
#~ size = (640, 480)
#~ size = (6, 4)

for x in range(1, size[0] + 1):
    for y in range(x, size[0] + 1):
        for bytes_per_pixel in range(1, 4):
            count = x * y * bytes_per_pixel
            #~ print "count = %d * %d * %d = %d" % (x, y, bytes_per_pixel,
                                                 #~ x * y * bytes_per_pixel)

            print "do it %d" % count
            
    
