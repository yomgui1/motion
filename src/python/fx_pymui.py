import os
import pymui
import cairo
import motion
import math

import traceback as tb

VERSION = 1.0
DATE = "09/06/2011"

_last_filename = None
IDENTITY_MATRIX = cairo.Matrix()

IECODE_LBUTTON   = 0x68
IECODE_RBUTTON   = 0x69
IECODE_MBUTTON   = 0x6A

def select_filename(title, parent=None, read=True, multiple=False):
    global _last_filename
    
    result = pymui.GetFilename(parent,
                               title,
                               _last_filename and os.path.dirname(_last_filename),
                               "#?.png",
                               not read,
                               multiple)
    if result:
        if multiple and read:
            _last_filename = result[0]
            return result
        else:
            _last_filename = result[0]
            return _last_filename

class VideoDisplay(pymui.Rectangle):
    _MCC_ = True
    
    _cr = None
    width = height = None
    
    def __init__(self):
        super(VideoDisplay, self).__init__(InnerSpacing=0, FillArea=False, DoubleBuffer=True)
        
        self._ev = pymui.EventHandler()
        self._view_mat = cairo.Matrix()
        self._cur_mat = cairo.Matrix()
        self._callbacks = {}

        self.clear()
        
    @pymui.muimethod(pymui.MUIM_Setup)
    def MCC_Setup(self, msg):
        self._ev.install(self, pymui.IDCMP_RAWKEY | pymui.IDCMP_MOUSEBUTTONS | pymui.IDCMP_MOUSEOBJECTMUI)
        return msg.DoSuper()

    @pymui.muimethod(pymui.MUIM_Cleanup)
    def MCC_Cleanup(self, msg):
        self._ev.uninstall()
        return msg.DoSuper()

    @pymui.muimethod(pymui.MUIM_HandleEvent)
    def MCC_HandleEvent(self, msg):
        try:
            self._ev.readmsg(msg)
            cl = self._ev.Class
            if cl == pymui.IDCMP_MOUSEMOVE:
                return self.on_mousemove(self._ev)
            elif cl == pymui.IDCMP_MOUSEOBJECTMUI:
                return self.on_mouseobject(self._ev)
            elif cl == pymui.IDCMP_RAWKEY:
                return self.on_key(self._ev)
            elif cl == pymui.IDCMP_MOUSEBUTTONS:
                return self.on_buttons(self._ev)
        except:
            tb.print_exc(limit=20)

    @pymui.muimethod(pymui.MUIM_AskMinMax)
    def _mcc_AskMinMax(self, msg):
        msg.DoSuper()
        mmi = msg.MinMaxInfo.contents
        mmi.MinWidth = mmi.MinWidth.value + 640
        mmi.MinHeight = mmi.MinHeight.value + 360
        mmi.MaxWidth = mmi.MaxWidth.value + pymui.MUI_MAXMAX
        mmi.MaxHeight = mmi.MaxHeight.value + pymui.MUI_MAXMAX

    @pymui.muimethod(pymui.MUIM_Draw)
    def _mcc_Draw(self, msg):
        msg.DoSuper()
        if not (msg.flags.value & pymui.MADF_DRAWOBJECT): return
        
        self.AddClipping()
        try:
            width = self.MWidth
            height = self.MHeight
            
            if self.width != width or self.height != height:
                self.width = width
                self.height = height
            
                self._target = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
                self._pixels = self._target.get_data()
                self._stride = self._target.get_stride()
                self._cr = cairo.Context(self._target)
                self._cr.set_source_rgb(0,0,0)
                self._cr.paint()

            cr = self._cr
            cr.save()
            try:
                if self._image:
                    cr.set_source_surface(self._image, 0, 0)
                    cr.set_matrix(self._cur_mat)
                    cr.paint()
                    
                    x = self._image.get_width() / 2.
                    y = self._image.get_height() / 2.
                    cr.translate(x, y)
                    cr.set_source_rgba(.9, .9, .9, 0.7)
                    cr.set_line_width(1)
                    cr.arc(0, 0, 4, 0, 2*math.pi)
                    cr.move_to(-7, 0)
                    cr.line_to(7, 0)
                    cr.move_to(0, -7)
                    cr.line_to(0, 7)
                    cr.stroke()
                    cr.translate(-x, -y)
                    
                    if self._trackers:
                        cr.set_source_rgb(0,1,0)
                        cr.set_line_width(1)
                        for x, y in self._trackers:
                            cr.translate(x, y)
                            cr.move_to(-2, -2)
                            cr.line_to(2, 2)
                            cr.move_to(2, -2)
                            cr.line_to(-2, 2)
                            cr.translate(-x, -y)
                        cr.stroke()
                    
            finally:
                cr.restore()
                
            self._rp.Blit8(self._pixels, self._stride, self.MLeft, self.MTop, self.width, self.height)
                
        except:
            tb.print_exc(limit=20)
       
        finally:
            self.RemoveClipping()
            
    def on_mousemove(self, evt):
        pass
        
    def on_mouseobject(self, evt):
        pass
        
    def on_key(self, evt):
        pass

    def on_buttons(self, evt):
        if evt.Up:
            cb = self._callbacks.get('buttons_released')
        else:
            cb = self._callbacks.get('buttons_released')
            
        if cb:
            return cb(evt)
        
    def set_callback(self, key, callable):
        self._callbacks[key] = callable

    def get_image_coordinates(self, mx, my):
        return self._cur_mat.transform_point(mx - self.MLeft, my - self.MTop)

    def reset_view(self):
        self._view_mat = cairo.Matrix()
        
    def clear(self):
        self._trackers = None
        self._image = None
        self._im_mat = IDENTITY_MATRIX
        self._cur_mat = IDENTITY_MATRIX
        self._inv_cur_mat = IDENTITY_MATRIX
        if self._cr:
            self._cr.set_source_rgb(0,0,0)
            self._cr.paint()
        self.Redraw()

    def show_image(self, im, im_mat, trackers, force=False):
        self._trackers = trackers
        self._image = im
        self._im_mat = im_mat
        self._cur_mat = im_mat * self._view_mat
        self._inv_cur_mat = self._cur_mat * IDENTITY_MATRIX
        self._inv_cur_mat.invert()
        self.Redraw()

class VideoEditorWindow(pymui.Window):
    _bt_im = { 'play': pymui.MUII_TapePlay,
               'stop': pymui.MUII_TapeStop,
               'first': pymui.MUII_TapePlayBack,
               'last': pymui.MUII_TapePlay,
               }

    _mode = None
    
    def __init__(self):
        super(VideoEditorWindow, self).__init__("Video Fx MUI - Editor", ID="VidEdWin", CloseOnReq=True)

        self._klt_ctx = motion.KLTContext()

        root = pymui.HGroup()
        self.RootObject = root

        # Left Buttons
        btbox = pymui.VGroup(Weight=0)
        root.AddChild(btbox)

        self._trackbt = {}
        for name in ('Add images',
                     'Remove all images',
                     '*Add tracker',
                     'Remove all trackers',
                     'Backward tracking',
                     'Forward tracking',
                     ):
            if name[0] == '*':
                name = name[1:]
                bt = pymui.Text(name,
                                PreParse=pymui.MUIX_C,
                                InputMode='Toggle',
                                Frame='Button',
                                Background='ButtonBack')
            else:
                bt = pymui.SimpleButton(name)
            btbox.AddChild(bt)
            self._trackbt[name] = bt
        btbox.AddChild(pymui.VSpace(0))

        # Video preview and buttons
        box = pymui.VGroup()
        root.AddChild(box)

        self._vd = VideoDisplay()
        box.AddChild(self._vd)

        self._framescale = pymui.Slider(Min=0, Max=0)
        self._framescale.Disabled = True
        box.AddChild(self._framescale)

        btbox = pymui.HGroup()
        box.AddChild(pymui.HCenter(btbox))

        self._playbt = {}
        for label, k in (('first', 'first'),
                         ('play_stop', 'play'),
                         ('last', 'last'),
                         ):
            bt = pymui.Image(Spec=self._bt_im[k],
                             InputMode='RelVerify',
                             Frame='Button',
                             Background='ImageBack')
            btbox.AddChild(bt)
            bt.Disable = True
            self._playbt[label] = bt

        self._trackbt['Add tracker'].Disabled = True

        self._trackbt['Add images'].Notify('Pressed', self._on_add_images, when=False)
        self._trackbt['Remove all images'].Notify('Pressed', self._on_clear_all, when=False)
        self._trackbt['Remove all trackers'].Notify('Pressed', self._on_clear_all_trackers, when=False)
        self._trackbt['Add tracker'].Notify('Selected', self._on_add_tracker_toggle)
        self._trackbt['Forward tracking'].Notify('Pressed', self._on_forward_tracking, when=False)

        self._playbt['first'].Notify('Pressed', lambda *a: self.show_first_image(), when=False)
        self._playbt['last'].Notify('Pressed', lambda *a: self.show_last_image(), when=False)
        self._playbt['play_stop'].Notify('Pressed', self._on_play_stop, when=False)
        
        self._framescale.Notify('Value', self._on_show_frame)

        self._vd.set_callback('buttons_released', self.vd_on_buttons_released)

        self._on_clear_all()

    def add_images(self, files):
        self.images_filenames += files
        
        for bt in self._playbt.itervalues():
            bt.Disabled = False
        self._framescale.Max = self.frame_count - 1
        self._framescale.Disabled = False
        self._trackbt['Add tracker'].Disabled = False
        
        self.show_current()
        
    def load_frame(self, frame):
        im = cairo.ImageSurface.create_from_png(self.images_filenames[frame])
        if self.width is None:
            self.width = im.get_width()
            self.height = im.get_height()
        else:
            assert self.width == im.get_width() and self.height == im.get_height()
        self.images_cache[frame] = im
        return im

    def show_frame(self, frame):
        if frame not in self.images_cache:
            im = self.load_frame(frame)
        else:
            im = self.images_cache[frame]
        if frame in self.images_matrices:
            im_mat = self.images_matrices[frame]
        else:
            im_mat = IDENTITY_MATRIX

        self._vd.show_image(im, im_mat, self.get_trackers(frame), force=True)
        
    def show_current(self):
        self.show_frame(self.current_frame)

    def show_first_image(self):
        self.current_frame = 0
        self.show_current()

    def show_last_image(self):
        self.current_frame = self.frame_count - 1
        self.show_current()

    def get_trackers(self, frame):
        return tuple(d[frame] for d in self._trackers if frame in d)

    def get_motion_image(self, frame):
        if frame in self.images_cache:
            surf = self.images_cache[frame]
            return motion.Image(motion.IMT_PIXFMT_ARGB32, surf.get_width(), surf.get_height(), 0, surf.get_data())
        else:
            return motion.load_image(self.images_filenames[frame])

    def forward_tracking(self, start, end):
        ftset = motion.FeatureSet()
        ftset.trackers = self.get_trackers(start)
        im1 = self.get_motion_image(start)            
        for frame in range(start, end):
            im2 = self.get_motion_image(frame)
            self._klt_ctx.track(im1, im2, ftset)

            for tracker in ftset.estimations:
                self._trackers[tracker[0]][frame] = tracker[-2:]
            
            ftset.trackers = tuple((x,y) for _, x, y in ftset.estimations)
            im1.flush()
            im1 = im2

            self.show_frame(frame)
        del im1, im2, ftset
        self.show_current()
            
    def _on_play_stop(self, evt):
        pass
        
    def _on_add_images(self, evt):
        names = select_filename(title='Select images to add', parent=self,
                                read=True, multiple=True)
        if names:
            self.add_images(names)

    def _on_clear_all(self, evt=None):
        self.images_filenames = []
        self.images_cache = {}
        self.images_matrices = {}
        self.current_frame = 0
        self._trackers = []
        self.width = self.height = None
        
        self._framescale.Max = 0
        self._framescale.Value = 0
        self.play = False

        for bt in self._playbt.itervalues():
            bt.Disabled = True
            
        self._vd.clear()
            
    def _on_clear_all_trackers(self, evt):
        self._trackers = []
        self.show_current()

    def _on_show_frame(self, evt):
        frame = evt.value.value
        if self.current_frame != frame:
            self.current_frame = frame
            self.show_current()

    def _on_add_tracker_toggle(self, evt):
        if evt.Source.Selected.value:
            self._mode = 'add tracker'
        else:
            self._mode = None

    def _on_forward_tracking(self, evt):
        parent = evt.Source.Parent.contents
        parent.Disabled = True
        self.Sleep = True
        try:
            self.forward_tracking(self.current_frame, self.frame_count)
        finally:
            parent.Disabled = False
            self.Sleep = False

    def vd_on_buttons_released(self, evt):
        if not evt.InObject and evt.Up: return
        if self._mode == 'add tracker':
            if evt.InObject and evt.Code == IECODE_LBUTTON:
                pt = self._vd.get_image_coordinates(evt.MouseX, evt.MouseY)
                self._trackers.append( { self.current_frame: pt } )
                self.show_current()
                return pymui.MUI_EventHandlerRC_Eat

    @property
    def frame_count(self):
        return len(self.images_filenames)
    
class Application(pymui.Application):
    def __init__(self, files=None):
        keys = {
            'Title'       : "PostFxMui",
            'Version'     : "$VER: PostFxMui %f (%s)" % (VERSION, DATE),
            'Copyright'   : "\xa92011, Guillaume ROGUEZ",
            'Author'      : "Guillaume ROGUEZ",
            'Description' : "Video Post Fx application",
            'Base'        : "PostFxMui",
        }
        super(Application, self).__init__(**keys)
        
        self._vedwin = VideoEditorWindow()
        self.AddChild(self._vedwin)
        self._vedwin.OpenWindow()
        
        if files:
            self._vedwin.add_images(files)

if __name__ == '__main__':
    import sys
    import glob
    
    if len(sys.argv) > 1:
        files = glob.glob(sys.argv[1])
    else:
        files = None
    app = Application(files)
    app.Run()
