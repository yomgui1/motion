# Python 2.5 compatibility
from __future__ import with_statement


from motion import *
from sys import argv
from glob import glob
from random import sample
from math import log, atan, acos, cos, sin, degrees, hypot
from warp import WarpTool

import sys, os

files = sorted(glob(argv[1]))
ctx = KLTContext()

ftset = FeatureSet()
ftset.trackers = tuple((float(argv[3+i]), float(argv[4+i])) for i in range(0, len(argv)-3, 2))

im1 = load_image(files[0])

warp = WarpTool()
warp.init_reference(im1)
warp.warp(os.path.join(argv[2], "frame_%04u.png" % 0), im1)

model = [1,0,0,1,0,0]

def get_model(PX, PY, EX, EY):
    px0, px1 = PX
    py0, py1 = PY
    ex0, ex1 = EX
    ey0, ey1 = EY

    dx = ex0 - px0
    dy = ey0 - py0

    px1 -= px0
    py1 -= py0
    ex1 -= ex0
    ey1 -= ey0
    
    dp = hypot(px1, py1)
    de = hypot(ex1, ey1)
    if not (dp*de): return
    cs = (px1*ex1 + py1*ey1) / (dp*de)
    a = acos(cs)
    if ey1 < py1:
        a = -a
    return cos(a),sin(a),dx,dy

for i in range(1, len(files)):
    im2 = load_image(files[i])
    ctx.track(im1, im2, ftset)

    if len(ftset.tracked) < 2:
        print "[STOP] trackers lost"
        break

    PX = [ x for x,_ in ftset.tracked ]
    PY = [ y for _,y in ftset.tracked ]
    EX = [ x for x,_ in ftset.estimations ]
    EY = [ y for _,y in ftset.estimations ]

    fit_model = get_model(PX, PY, EX, EY)
    if fit_model:
        cs,sn,dx,dy = fit_model
        model = [cs,sn,-sn,cs,dx,dy]
    else:
        model = [1,0,0,1,0,0]
        
    filename = os.path.join(argv[2], "frame_%04u.png" % i)
    warp.warp(filename, im2, model, center=ftset.tracked[0], clear=True)

    ftset.trackers = ftset.estimations
    im1.flush()
    im1 = im2

"""
a.px(i) - b.py(i) + tx = ex(i)
b.px(i) + a.py(i) + ty = ey(i)

tx = ex0 - a.px0 + b.py0
tx = ex1 - a.px1 + b.py1

ex0 - a.px0 + b.py0 = ex1 - a.px1 + b.py1
a = (ex1 - ex0 + b.(py1 - py0)) / (px1 - px0)

ty = ey0 - b.px0 - a.py0
ty = ey1 - b.px1 - a.py1

ey0 - b.px0 - a.py0 = ey1 - b.px1 - a.py1
b = (ey1 - ey0 - a.(py1 - py0)) / (px1 - px0)


d = ((px1 - px0)**2 - (py1 - py0)**2)

px1 != px0
d != 0

a = ((ex1 - ex0).(px1 - px0) + (ey1 - ey0).(py1 - py0)) / ((px1 - px0)**2 - (py1 - py0)**2)
b = (ey1 - ey0 - a.(py1 - py0)) / (px1 - px0)
tx = ex0 - a.px0 + b.py0
ty = ey0 - b.px0 - a.py0
"""
