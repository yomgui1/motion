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

dxl = []
dyl = []

frames = 0

with open(argv[2], 'r') as fp:
	for line in fp.readlines():
		dx, dy, _ = line.split()
	
		dx = float(dx)
		dy = float(dy)
		
		dxl.append(dx)
		dyl.append(dy)
		
dxl.append(0.0)
dyl.append(0.0)

for i in xrange(len(files)):
	surface = cairo.ImageSurface.create_from_png(files[i])
	
	cr.set_source_surface(surface, dxl[i], dyl[i])
	cr.paint()

	print "Writing frame #%u" % i
	target.write_to_png("ram:image_%u.png" % i)


