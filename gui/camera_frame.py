import logging
import os, tempfile
import wx

import convert_rgb565

log = logging.getLogger(__name__)

class CameraFrame(wx.Frame):
    def __init__(self, parent=None, id=wx.ID_ANY,
                 pos=wx.DefaultPosition):

        wx.Frame.__init__(self, parent, id,
                          title="CameraFrame", pos=pos,
                          size=(240,220), style=wx.DEFAULT_FRAME_STYLE)
        log.debug("PovPictureWindow Init")
        self.image = None

        self.OnSize(None)
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_MOUSE_EVENTS, self.OnMouse)

    def OnPaint(self, event):
        log.debug("OnPaint()")
        dc = wx.BufferedPaintDC(self, self._Buffer)

    def OnSize(self, event):
        log.debug("OnSize!")
        self.Width, self.Height = self.GetClientSizeTuple()
        self._Buffer = wx.EmptyBitmap(self.Width, self.Height)
        self.Draw()

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
