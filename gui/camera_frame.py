import logging
import Image, sys, struct
import os, tempfile
import socket
import wx
from threading import *
from datetime import datetime
from StringIO import StringIO

import convert_rgb565

log = logging.getLogger(__name__)

# Define notification event for thread completion
EVT_RESULT_ID = wx.NewId()

def EVT_RESULT(win, func):
    """Define Result Event."""
    win.Connect(-1, -1, EVT_RESULT_ID, func)

class ResultEvent(wx.PyEvent):
    """Simple event to carry arbitrary result data."""
    def __init__(self, data):
        """Init Result Event."""
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_RESULT_ID)
        self.data = data

class WorkerThread(Thread):
    """Worker Thread Class."""
    def __init__(self, notify_window):
        """Init Worker Thread Class."""
        Thread.__init__(self)
        self._notify_window = notify_window
        self.image = None

    def run(self):
        while True:
            log.debug("Thread downloading...")
            img = convert_rgb565.__download__("phone.leo")
            tmp_file = tempfile.mkstemp()[1]
            output = open(tmp_file, 'wb')
            img.save(output, 'bmp')
            output.close()

            log.debug("Creating image")
            #~ wx_image = wx.Image(tmp_file, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
            #~ os.remove(tmp_file)
            log.debug("Creating image. Done.")
            #~ wx.PostEvent(self._notify_window, wx.EVT_PAINT)
            wx.PostEvent(self._notify_window, ResultEvent(tmp_file))


class CameraFrame(wx.Frame):
    def __init__(self, parent=None, id=wx.ID_ANY,
                 pos=wx.DefaultPosition):

        wx.Frame.__init__(self, parent, id,
                          title="CameraFrame", pos=pos,
                          size=(240,220), style=wx.DEFAULT_FRAME_STYLE)
        log.debug("PovPictureWindow Init")
        self.image = None
        self.worker = WorkerThread(self)
        #~ self.worker.start()

        self.OnSize(None)
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_MOUSE_EVENTS, self.OnMouse)
        EVT_RESULT(self, self.OnResult)


    def OnPaint(self, event):
        log.debug("OnPaint()")
        dc = wx.BufferedPaintDC(self, self._Buffer)

    def OnSize(self, event):
        log.debug("OnSize!")
        self.Width, self.Height = self.GetClientSizeTuple()
        self._Buffer = wx.EmptyBitmap(self.Width, self.Height)
        self.Draw()

    def OnResult(self, event):
        log.debug("OnResult(%s)" % event.data)
        tmp_file = event.data
        
        self.image = wx.Image(tmp_file, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        os.remove(tmp_file)
        self.Update()
        
    def Draw(self):
        #~ log.debug("Draw()")
        dc = wx.BufferedPaintDC(self, self._Buffer)
        dc.Clear()

        if self.image:
            png_dc = wx.MemoryDC()
            png_dc.SelectObject(self.image)
            dc.Blit(0, 0, self.image.GetWidth(), self.image.GetHeight(),
                    png_dc, 0, 0)
        
    def OnMouse(self, event):
        if not event.LeftDown():
            return

        log.debug("OnMouse()")
        host = "192.168.120.105"
        port = 31337
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))

        BUFFER_SIZE = 1024
        img = Image.new("RGB", (240, 220))
        putpixel = img.putpixel
        y = 0
        x = 0
        last_ts = datetime.now()
        #~ data = s.recv(BUFFER_SIZE)
        while True:
            raw_buffer = s.recv(BUFFER_SIZE)
            #~ log.debug("raw_buffer: %s" % len(raw_buffer))

            raw_ushort = None
            #~ mybuffer = StringIO(raw_buffer)
            my_fmt = "%dH" % (len(raw_buffer) / 2, )
            raw_ushort_array = struct.unpack_from(my_fmt, raw_buffer)
            #~ log.debug("raw_ushort_array: %s" % len(raw_ushort_array))
            #~ continue

            for raw_ushort in raw_ushort_array:
                ##~ raw = raw_buffer[index:index + 2]
                ##~ raw_ushort = struct.unpack('H', raw)[0]

                color = convert_rgb565.convert_one(raw_ushort)
                putpixel( (y, x), color)
                y += 1
                if y >= 240:
                    y = 0
                    x += 1

                if x >= 220:
                    y = 0
                    x = 0

                    log.debug("*** NEXT COLUMN ***")
                    tmp_file = tempfile.mkstemp()[1]
                    output = open(tmp_file, 'wb')
                    img.save(output, 'bmp')
                    output.close()
    
                    self.image = wx.Image(tmp_file, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
                    os.remove(tmp_file)
    
                    self.Update()
#~ 
                    delta = datetime.now() - last_ts
                    last_ts = datetime.now()
                    log.debug("delta: %s" % delta)
                    #~ return
                    
                
                #~ log.debug("data: %s" % data)
        s.close()

        return
        
        #~ infile = open('/tmp/blah4', 'rb')
        #~ img = convert_rgb565.__convert_rgb565__(infile, False)
        #~ infile.close()

        img = convert_rgb565.__download__("phone.leo")

        tmp_file = tempfile.mkstemp()[1]
        output = open(tmp_file, 'wb')
        img.save(output, 'bmp')
        output.close()

        self.image = wx.Image(tmp_file, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        os.remove(tmp_file)

        self.Update()

    def Update(self):
        self.Draw()
        wx.Frame.Update(self)

def main():
    app = wx.App()
    window = CameraFrame()
    window.Update()
    window.Show(True)

    log.info("entering main loop")
    app.MainLoop()
