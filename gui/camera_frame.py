import logging
import os, tempfile
import wx
from threading import *

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
    def __init__(self, notify_window, host):
        """Init Worker Thread Class."""
        Thread.__init__(self)
        self._notify_window = notify_window
        self.image = None
        self.host = host

    def run(self):
        while True:
            log.debug("Thread downloading...")
            img = convert_rgb565.__download__(self.host)
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
    def __init__(self, host, parent=None, id=wx.ID_ANY,
                 pos=wx.DefaultPosition):

        wx.Frame.__init__(self, parent, id,
                          title="CameraFrame", pos=pos,
                          size=(240,220), style=wx.DEFAULT_FRAME_STYLE)
        log.debug("PovPictureWindow Init")
        self.image = None
        self._Buffer = None
        self.worker = WorkerThread(self, host)
        self.worker.start()
		
        #self.OnSize(None)
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_MOUSE_EVENTS, self.OnMouse)
        EVT_RESULT(self, self.OnResult)


    def OnPaint(self, event):
        log.debug("OnPaint()")
        #dc = wx.BufferedPaintDC(self, self._Buffer)
        self.Draw()

    def OnSize(self, event):
        log.debug("OnSize!")
        self.Width, self.Height = self.GetClientSizeTuple()
        self._Buffer = wx.EmptyBitmap(self.Width, self.Height)
#        self.Draw()

    def OnResult(self, event):
        log.debug("OnResult(%s)" % event.data)
        tmp_file = event.data
        
        self.image = wx.Image(tmp_file, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        #os.remove(tmp_file)
        self.Update()
        
    def Draw(self):
        #~ log.debug("Draw()")
        
        if not self._Buffer:
            return
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

        self.Update()

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

        
    def Update(self):
        #self.Draw()
        wx.Frame.Update(self)

def main(host):
    app = wx.App()
    window = CameraFrame(host)
    window.Update()
    window.Show(True)

    log.info("entering main loop")
    app.MainLoop()
