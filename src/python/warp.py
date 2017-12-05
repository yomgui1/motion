import cairo
from motion import *

class WarpTool:
    def init_reference(self, image):
        if image.format in (IMT_PIXFMT_RGB24, IMT_PIXFMT_ARGB32):
            self.fmt = cairo.FORMAT_ARGB32
        elif image.format == IMT_PIXFMT_GRAY8:
            self.fmt = cairo.FORMAT_A8

        self.width = image.width
        self.height = image.height

        self.target = cairo.ImageSurface(self.fmt, self.width, self.height)
        self.cr = cairo.Context(self.target)
        self.global_mat = cairo.Matrix()

        self.min_x = 0
        self.min_y = 0
        self.max_x = self.width-1
        self.max_y = self.height-1

    def warp(self, filename, image, motion_mat=[1,0,0,1,0,0], center=(0,0), clear=False, debug=False):
        mat = cairo.Matrix(*motion_mat)
        mat.invert()
        self.global_mat = mat * self.global_mat

        cx, cy = center

        crmat = cairo.Matrix()
        crmat.translate(cx, cy)
        crmat = self.global_mat * crmat
        crmat.translate(-cx, -cy)

        cx += motion_mat[-2]
        cy += motion_mat[-1]

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

        if debug:
            self.cr.set_line_width(2)
            self.cr.set_source_rgb(1,0,0)
            self.cr.move_to(cx-2, cy-2)
            self.cr.line_to(cx+2, cy+2)
            self.cr.move_to(cx+2, cy-2)
            self.cr.line_to(cx-2, cy+2)
            self.cr.stroke()

        print("Writing frame '%s'" % filename)
        self.target.write_to_png(filename)

        # Compute the area
        p = []
        x = self.width-1
        y = self.height-1
        p.append(crmat.transform_point(0,0))
        p.append(crmat.transform_point(x,0))
        p.append(crmat.transform_point(x,y))
        p.append(crmat.transform_point(0,y))
        X = sorted(x for x,_ in p)
        Y = sorted(y for _,y in p)
        del p

        self.min_x = min(x, max(self.min_x, X[1]))
        self.max_x = max(0, min(self.max_x, X[2]))
        self.min_y = min(y, max(self.min_y, Y[1]))
        self.max_y = max(0, min(self.max_y, Y[2]))

    def print_crop(self):
        print("crop=%d:%d:%d:%d" % (self.max_x-self.min_x+1, self.max_y-self.min_y+1, self.min_x, self.min_y))
