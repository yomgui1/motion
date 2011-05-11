from __future__ import with_statement

from sys import argv
from glob import glob
from math import pi, ceil, floor, atan
import cairo

files = sorted(glob(argv[1]))

surface = cairo.ImageSurface.create_from_png(files[0])
fmt = surface.get_format()
width = surface.get_width()
height = surface.get_height()

target = cairo.ImageSurface(fmt, width, height)
cr = cairo.Context(target)

frames = 0
frame_mat = [ [0.0]*6 ]

with open(argv[2], 'r') as fp:
    for line in fp.readlines():
        a, b, c, d, tx, ty, _ = map(float, line.split())
        frame_mat.append([a, b, c, d, tx, ty])
        
def get_rc(fc):
    return 1. / (2 * pi * fc)

def get_alpha(dt, fc):
    return dt / (dt + get_rc(fc))

alpha = get_alpha(len(files)/25., 2.)
print "alpha:", alpha

min_x = 0
min_y = 0
max_x = width-1
max_y = height-1

glob_mat = cairo.Matrix()
for i in xrange(1, len(files)):
    a, b, c, d, tx, ty = [ frame_mat[i-1][j] + alpha * (frame_mat[i][j] - frame_mat[i-1][j]) for j in xrange(6) ]
    motion_mat = cairo.Matrix(1, 0, 0, 1, tx, ty)
    glob_mat *= motion_mat

    cr.save()
    cr.rectangle(0, 0, width, height)
    cr.clip()
    cr.set_matrix(glob_mat)
    
    x1,y1,x2,y2 = cr.clip_extents()
    cr.restore()

    min_x = max(min_x, x1)
    min_y = max(min_y, y1)
    max_x = min(max_x, x2)
    max_y = min(max_y, y2)

min_x = int(ceil(min_x))
min_y = int(ceil(min_y))
max_x = int(floor(max_x))
max_y = int(floor(max_y))

w = max_x - min_x + 1
h = max_y - min_y + 1
sw = width / float(w)
sh = height / float(h)

print "Cropping: (%u, %u) %u x %u, scale: %g x %g" % (min_x, min_y, w, h, sw, sh)

cr.save()
mat = cairo.Matrix()
crop_mat = cairo.Matrix()

crop_mat.translate(-min_x, -min_y)
crop_mat.scale(sw,sh)

mat.translate(width/2., height/2.)
mat = mat.multiply(crop_mat)
mat.translate(-width/2., -height/2.)

cr.set_matrix(crop_mat)
cr.set_source_surface(surface, 0, 0)
cr.paint()
cr.restore()

print "Writing frame #%u" % 0
target.write_to_png("out/frame_%04u.png" % 0)

glob_mat = cairo.Matrix()
for i in xrange(1, len(files)):
    surface = cairo.ImageSurface.create_from_png(files[i])

    cr.set_source_rgb(0, 0, 0)
    cr.paint()

    a, b, c, d, tx, ty = [ frame_mat[i-1][j] + alpha * (frame_mat[i][j] - frame_mat[i-1][j]) for j in xrange(6) ]
    motion_mat = cairo.Matrix()
    motion_mat.translate(-tx, -ty)

    rot_mat = cairo.Matrix()
    rot_mat.init_rotate(atan(c/a))
    rot_mat.invert()

    glob_mat *= motion_mat

    mat = cairo.Matrix()
    
    mat.translate(width/2., height/2.)
    mat *= rot_mat
    mat.translate(-width/2., -height/2.)
    
    mat *= glob_mat

    mat = mat.multiply(crop_mat)
    cr.set_matrix(mat)

    cr.set_source_surface(surface, 0, 0)
    cr.paint()

    print "Writing frame #%u" % i
    target.write_to_png("out/frame_%04u.png" % i)
