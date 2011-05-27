import cairo
from motion import *

class WarpTool:
    def init_reference(self, image):
        if image.format in (IMT_PIXFMT_RGB24, IMT_PIXFMT_ARGB32):
            self.fmt = cairo.FORMAT_ARGB32
        elif image.format == IMT_PIXFMT_GRAY8:
            self.fmt = cairo.FORMAT_A8

        width = image.width
        height = image.height
        
        self.target = cairo.ImageSurface(self.fmt, width, height)
        self.cr = cairo.Context(self.target)
        self.global_mat = cairo.Matrix()
        
    def warp(self, filename, image, motion_mat=(1,0,0,1,0,0), center=(0,0), clear=False):
        mat = cairo.Matrix(*motion_mat)
        mat.invert()
        self.global_mat = mat * self.global_mat

        cx, cy = center
        cx += motion_mat[-2]
        cy += motion_mat[-1]

        crmat = cairo.Matrix()
        crmat.translate(cx, cy)
        crmat = self.global_mat * crmat
        crmat.translate(-cx, -cy)

        self.cr.set_matrix(crmat)
        
        surface = cairo.ImageSurface.create_for_data(image.data,
                                                     self.fmt,
                                                     image.width,
                                                     image.height,
                                                     image.stride)
        if clear:
            self.cr.set_source_rgb(0,0,0)
            self.cr.paint()
            
        self.cr.set_source_surface(surface, 0, 0)
        self.cr.paint()
        
        print("Writing frame '%s'" % filename)
        self.target.write_to_png(filename)
