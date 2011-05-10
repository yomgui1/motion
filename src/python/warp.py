from __future__ import with_statement

from sys import argv
from glob import glob
import cairo

files = sorted(glob(argv[1]))

surface = cairo.ImageSurface.create_from_png(files[0])
fmt = surface.get_format()
width = surface.get_width()
height = surface.get_height()

target = cairo.ImageSurface(fmt, width, height)
cr = cairo.Context(target)

frames = 0

from math import atan, pi

dxl = [0.]
dyl = [0.]
drl = [0.]

with open(argv[2], 'r') as fp:
    for line in fp.readlines():
        cs, sn, dx, dy, _ = line.split()
        
        cs = float(cs)
        sn = float(sn)
        dx = float(dx)
        dy = float(dy)
        
        dxl.append(dx)
        dyl.append(dy)
        drl.append(atan(sn/cs))

matid = cairo.Matrix()

def get_rc(fc):
    return 1. / (2 * pi * fc)

def get_alpha(dt, fc):
    return dt / (dt + get_rc(fc))

alpha = get_alpha(len(files)/25., 1.0)
#print "alpha:", alpha

drp = dxp = dyp = 0.

cr.set_source_surface(surface, 0, 0)
cr.paint()
print "Writing frame #%u" % 0
target.write_to_png("out/frame_%04u.png" % 0)

alpha = 1.0

for i in xrange(1, len(files)):
    surface = cairo.ImageSurface.create_from_png(files[i])

    mat = cairo.Matrix()

    drp = drp + alpha * (drl[i]-drp)
    dxp = dxp + alpha * (dxl[i]-dxp)
    dyp = dyp + alpha * (dyl[i]-dyp)

    mat.translate(-dxp, -dyp)
    mat.translate(width/2, height/2)
    mat.rotate(-drp)
    mat.translate(-width/2, -height/2)
    
    
    cr.set_matrix(mat)
    cr.set_source_surface(surface, 0, 0)
    cr.paint()

    print "Writing frame #%u" % i
    target.write_to_png("out/frame_%04u.png" % i)


