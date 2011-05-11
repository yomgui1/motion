from __future__ import with_statement

from sys import argv
from glob import glob
from math import pi, ceil, floor, atan
import cairo, os

files = sorted(glob(argv[1]))

surface = cairo.ImageSurface.create_from_png(files[0])
fmt = surface.get_format()
width = surface.get_width()
height = surface.get_height()

target = cairo.ImageSurface(fmt, width, height)
cr = cairo.Context(target)

frames = 0
frame_mat = [ [0.0]*6 ]

alpha = 0.9

with open(argv[2], 'r') as fp:
    for line in fp.readlines():
        frame_mat.append(map(float, line.split())[:-1])

frames = len(frame_mat)

min_x = 0
min_y = 0
max_x = width-1
max_y = height-1

cum_mat = cairo.Matrix()
smoothed_frame_mat = [ [0.0]*6 ]

for i in xrange(1, frames):
    data = [ smoothed_frame_mat[i-1][j] - alpha * (smoothed_frame_mat[i-1][j] - frame_mat[i-1][j]) for j in xrange(6) ]
    smoothed_frame_mat.append(data)
    
    tx, ty = data[-2:]
    cum_mat.translate(tx, ty)

    cr.save()
    cr.rectangle(0, 0, width, height)
    cr.clip()
    cr.set_matrix(cum_mat)
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

# Compute scaling factor to keep aspect
print width, height, w, h
aspect1 = float(width)/height
aspect2 = float(w)/h
if aspect1 < aspect2:
    scale = height / float(h)
else:
    scale = width / float(w)

print "Cropping: Offset=(%u, %u), Scale=%g" % (min_x, min_y, scale)

cr.save()
mat = cairo.Matrix()
crop_mat = cairo.Matrix()

crop_mat.translate(-min_x, -min_y)
crop_mat.scale(scale, scale)
mat = mat.multiply(crop_mat)

cum_mat = cairo.Matrix()
for i in xrange(frames):
    cr.set_source_rgb(0, 0, 0)
    cr.paint()
    
    coef = smoothed_frame_mat[i]
    
    # Cumulate translation but not rotation
    cum_mat.translate(-coef[-2], -coef[-1])

    # XXX: affine matrix seems wrong (gives a more unstable sequence!)
    #affine_mat = cairo.Matrix(*coef[:4])
    #affine_mat.invert()

    mat = cairo.Matrix()
    mat *= cum_mat
    #mat *= affine_mat
    #mat *= crop_mat
    cr.set_matrix(mat)
    
    surface = cairo.ImageSurface.create_from_png(files[i])
    cr.set_source_surface(surface, 0, 0)
    cr.paint()

    print "Writing frame #%u" % i
    target.write_to_png(os.path.join(argv[3], "frame_%04u.png" % i))
