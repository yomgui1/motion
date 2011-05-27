#
# Kepp 3.x/2.x compatibility (except external modules)
#

# Python 2.5 compatibility
from __future__ import with_statement

from motion import *
from sys import argv
from glob import glob
from random import sample
from math import log
from warp import WarpTool

import sys, os

# Value used to compute number of RANSAC iterations
# Must be in [0, 1] range.
# Lower is, higher iterations will be.
outliers_prob = 0.01

# maximal model average error on all trackers per frame
max_error = 0.1

files = sorted(glob(argv[1]))

im1 = load_image(files[0])
corners = detect_corners(im1.grayscale, 0.3, 600)

ctx = KLTContext()
ftset = FeatureSet()

ftset.trackers = corners
del corners
print("%u tracker(s) found." % len(ftset.trackers))


def solveAffineMatrix(samples):
    """solveAffineMatrix(samples) -> err
    Solve the equation M.p = e for three (p, e) points in samples.

    M : is a 3x2 matrix, describing an affine transformation (model).
    p : is the input 2D point
    e : is an estimation of p.
    
    See notes below to know how this equations have been found.
    """

    # Use first 3-points to compute basic matrix coeffiscients
    p0, e0 = samples[0]
    p1, e1 = samples[1]
    p2, e2 = samples[2]
    
    px0, py0 = p0
    ex0, ey0 = e0
    px1, py1 = p1
    ex1, ey1 = e1
    px2, py2 = p2
    ex2, ey2 = e2

    Vx01 =     px0 -     px1
    YP01 = py1*px0 - py0*px1
    XE01 = ex1*px0 - ex0*px1
    YE01 = ey1*px0 - ey0*px1
    
    Vx02 =     px0 -     px2
    YP02 = py2*px0 - py0*px2
    XE02 = ex2*px0 - ex0*px2
    YE02 = ey2*px0 - ey0*px2
        
    den = (Vx02*YP01 - Vx01*YP02)
    if not (den and YP01 and px0):
        return
    
    ty = (YE02*YP01 - YE01*YP02) / den
    tx = (XE02*YP01 - XE01*YP02) / den
    
    d  = (YE01 - ty*Vx01) / YP01
    c  = (ey0 - d*py0 - ty) / px0
    b  = (XE01 - tx*Vx01) / YP01
    a  = (ex0 - b*py0 - tx) / px0

    return (a,b,c,d,tx,ty)
        
def getError(model, p, e):
    ex = (model[0]*p[0] + model[1]*p[1] + model[4]) - e[0]
    ey = (model[2]*p[0] + model[3]*p[1] + model[5]) - e[1]
    return ex*ex + ey*ey

def getInliners(samples, model, threshold):
    score = 0
    inliers = []
    for s in samples:
        err = getError(model, *s)
        if err < threshold:
            score += err
            inliers.append(s)
        else:
            score += threshold
    return score, inliers

threshold = 2 * (max_error**2)
max_max_iterations = 1000
min_samples = 3

warp = WarpTool()

# record first image as it
warp.init_reference(im1)
warp.warp(os.path.join(argv[3], "frame_%04u.png" % 0), im1)

with open(argv[2], 'w') as fp:
    for i in range(1, len(files)):
        # reset frame state variables
        best_score = 1e400 # inf for 64bits floating point
        best_model = [1, 0, 0, 1, 0, 0]

        # load second image and track
        im2 = load_image(files[i])
        ctx.track(im1, im2, ftset)

        # prepare samples
        allsamples = tuple(zip(ftset.tracked, ftset.estimations))
        print("Frame %u: remains %u tracker(s)" % (i, len(allsamples)))

        # enough trackers for solving?
        if min_samples > len(allsamples):
            fp.write("1 0 0 1 0 0 %u\n" % i)
            continue
        
        # Applying RANSAC algorithme to find the best model
        k = 0
        max_iterations = max_max_iterations
        while k < max_iterations:
            k += 1

            # Compute a model list for current inliers
            model = None
            while not model:
                model = solveAffineMatrix(sample(allsamples, min_samples))

            # Compute the summed square error over all points for the model
            # and find the model giving the minimal value.
            score, inliers = getInliners(allsamples, model, threshold)
            if score < best_score:
                best_score = score
                best_model = model
                best_inliers = inliers
                w = len(best_inliers) / float(len(allsamples))
                if w < 1.0:
                    max_iterations = int(log(outliers_prob) / log(1.0 - pow(w, min_samples)))
                    max_iterations = min(max_iterations, max_max_iterations)
        
        print("Frame %u: best err of %le over %u inliers (%u iterations)" % (i, best_score/len(allsamples), len(best_inliers), k))
        print("Frame %u: model=(%le, %le, %le, %le, %le, %le)" % ((i,)+best_model))
        fp.write("%le %le %le %le %le %le %u\n" % (best_model + (i,)))

        filename = os.path.join(argv[3], "frame_%04u.png" % i)
        warp.warp(filename, im2, best_model, clear=True)
        
        ftset.trackers = ftset.estimations
        im1.flush()
        im1 = im2

"""
Notes:

Need to compute the 2D affine transformation matrix M:
    e = M.p

where:

    |a b tx|      
M = |c d ty|; e = |ex ey 1|T; p = |px py 1|T
    |0 0  1|

Note that a,b,c,d constitues coef. of a rotation matrix:
    a = d = cos(teta) = cs
    c = -b = sin(teta) = sn

=> 6 dof, so we need at least 3 points (3x2 values) to resolve the M matrix.

a.px(i) + b.py(i) + tx = ex(i)
c.px(i) + d.py(i) + ty = ey(i)

This gives following equation to solve:

Pn.N = En

Where:

| px(0)   py(0)     0      0     1 0|   | a|   | ex(0) |
|   0       0     px(0)   py(0)  0 1|   | b|   | ey(0) |
                 ...                  . | c| =    ...
|px(n-1) py(n-1)    0       0    1 0|   | d|   |ex(n-1)|
|   0       0    px(n-1) py(n-1) 0 1|   |tx|   |ey(n-1)|
                                        |ty|
and n >= 3

This is give the augmented matrix:

[ px0  py0    0    0  1  0 | ex0 ]
[   0    0  px0  py0  0  1 | ey0 ]
[ px1  py1    0    0  1  0 | ex1 ]
[   0    0  px1  py1  0  1 | ey1 ]
[ px2  py2    0    0  1  0 | ex2 ]
[   0    0  px2  py2  0  1 | ey2 ]


1) swap some lines

[ px0  py0    0    0  1  0 | ex0 ]
[ px1  py1    0    0  1  0 | ex1 ]
[ px2  py2    0    0  1  0 | ex2 ]
[   0    0  px0  py0  0  1 | ey0 ]
[   0    0  px1  py1  0  1 | ey1 ]
[   0    0  px2  py2  0  1 | ey2 ]


3) divide L1 per px0

[   1  py0/px0    0    0  1/px0  0 | ex0/px0 ]
[ px1      py1    0    0      1  0 |     ex1 ]
[ px2      py2    0    0      1  0 |     ex2 ]
[   0        0  px0  py0      0  1 |     ey0 ]
[   0        0  px1  py1      0  1 |     ey1 ]
[   0        0  px2  py2      0  1 |     ey2 ]


4) remove a21 and a31 by substracting ai1*a11, and replacing some values for visibility

[ 1   py0/px0    0    0     1/px0  0 |  ex0/px0 ]
[ 0  CP01/px0    0    0  Vx01/px0  0 | XE01/px0 ]
[ 0  CP02/px0    0    0  Vx01/px0  0 | XE02/px0 ]
[ 0         0  px0  py0         0  1 |      ey0 ]
[ 0         0  px1  py1         0  1 |      ey1 ]
[ 0         0  px2  py2         0  1 |      ey2 ]

where:
CP01 = py1.px0-px1.py0
CP02 = py2.px0-px2.py0
XE01 = ex1.px0-px1.ex0
XE02 = ex2.px0-px2.ex0
Vx01 = px0-px1
Vx02 = px0-px2


5) divide L2 per a22

[ 1   py0/px0    0    0      1/px0  0 |   ex0/px0 ]
[ 0         1    0    0  Vx01/CP01  0 | XE01/CP01 ]
[ 0  CP02/px0    0    0   Vx02/px0  0 |  XE02/px0 ]
[ 0         0  px0  py0          0  1 |       ey0 ]
[ 0         0  px1  py1          0  1 |       ey1 ]
[ 0         0  px2  py2          0  1 |       ey2 ]


6) remove a32 by substracting a32*a22

[ 1  py0/px0    0    0              1/px0  0 |               ex0/px0 ]
[ 0        1    0    0          Vx01/CP01  0 |             XE01/CP01 ]
[ 0        0    0    0  (Vx02 - CP02)/px0  0 | (XE02 - CP02.ex0)/px0 ]
[ 0        0  px0  py0                  0  1 |                   ey0 ]
[ 0        0  px1  py1                  0  1 |                   ey1 ]
[ 0        0  px2  py2                  0  1 |                   ey2 ]


7) swap some lines

[ 1  py0/px0    0    0              1/px0  0 |               ex0/px0 ]
[ 0        1    0    0          Vx01/CP01  0 |             XE01/CP01 ]
[ 0        0  px0  py0                  0  1 |                   ey0 ]
[ 0        0  px1  py1                  0  1 |                   ey1 ]
[ 0        0  px2  py2                  0  1 |                   ey2 ]
[ 0        0    0    0  (Vx02 - CP02)/px0  0 | (XE02 - CP02.ex0)/px0 ]


8) divide L3 per a33

[ 1  py0/px0    0        0              1/px0      0 |                ex0/px0 ]
[ 0        1    0        0          Vx01/CP01      0 |              XE01/CP01 ]
[ 0        0    1  py0/px0                  0  1/px0 |                ey0/px0 ]
[ 0        0  px1      py1                  0      1 |                    ey1 ]
[ 0        0  px2      py2                  0      1 |                    ey2 ]
[ 0        0    0        0  (Vx02 - CP02)/px0      0 |  (XE02 - CP02.ex0)/px0 ]


9) remove a43 and a53 by substracting ai3*a33, and replacing some values for visibility

[ 1  py0/px0  0         0              1/px0         0 |               ex0/px0 ]
[ 0        1  0         0          Vx01/CP01         0 |             XE01/CP01 ]
[ 0        0  1   py0/px0                  0     1/px0 |               ey0/px0 ]
[ 0        0  0  CP01/px0                  0  Vx01/px0 |              YE01/px0 ]
[ 0        0  0  CP02/px0                  0  Vx02/px0 |              YE02/px0 ]
[ 0        0  0         0  (Vx02 - CP02)/px0         0 | (XE02 - CP02.ex0)/px0 ]

YE01 = ey1.px0-px1.ey0
YE02 = ey2.px0-px2.ey0


10) divide L4 per a44

[ 1  py0/px0  0         0              1/px0          0 |               ex0/px0 ]
[ 0        1  0         0          Vx01/CP01          0 |             XE01/CP01 ]
[ 0        0  1   py0/px0                  0      1/px0 |               ey0/px0 ]
[ 0        0  0         1                  0  Vx01/CP01 |             YE01/CP01 ]
[ 0        0  0  CP02/px0                  0   Vx02/px0 |              YE02/px0 ]
[ 0        0  0         0  (Vx02 - CP02)/px0          0 | (XE02 - CP02.ex0)/px0 ]


11) remove a54 by substracting ai4*a44

[ 1  py0/px0  0         0              1/px0                                 0 |                          ex0/px0 ]
[ 0        1  0         0          Vx01/CP01                                 0 |                        XE01/CP01 ]
[ 0        0  1   py0/px0                  0                             1/px0 |                          ey0/px0 ]
[ 0        0  0         1                  0                         Vx01/CP01 |                        YE01/CP01 ]
[ 0        0  0         0                  0  (Vx02.CP01-CP02.Vx01)/(px0.CP01) | (YE02.CP01-CP02.YE01)/(px0.CP01) ]
[ 0        0  0         0  (Vx02 - CP02)/px0                                 0 |            (XE02 - CP02.ex0)/px0 ]


12) swap some lines

[ 1  py0/px0  0         0              1/px0                                 0 |                          ex0/px0 ]
[ 0        1  0         0          Vx01/CP01                                 0 |                        XE01/CP01 ]
[ 0        0  1   py0/px0                  0                             1/px0 |                          ey0/px0 ]
[ 0        0  0         1                  0                         Vx01/CP01 |                        YE01/CP01 ]
[ 0        0  0         0  (Vx02 - CP02)/px0                                 0 |            (XE02 - CP02.ex0)/px0 ]
[ 0        0  0         0                  0  (Vx02.CP01-CP02.Vx01)/(px0.CP01) | (YE02.CP01-CP02.YE01)/(px0.CP01) ]


13) divide L5 per a55

[ 1  py0/px0  0         0      1/px0                                 0 |                          ex0/px0 ]
[ 0        1  0         0  Vx01/CP01                                 0 |                        XE01/CP01 ]
[ 0        0  1   py0/px0          0                             1/px0 |                          ey0/px0 ]
[ 0        0  0         1          0                         Vx01/CP01 |                        YE01/CP01 ]
[ 0        0  0         0          1                                 0 |  (XE02 - CP02.ex0)/(Vx02 - CP02) ]
[ 0        0  0         0          0  (Vx02.CP01-CP02.Vx01)/(px0.CP01) | (YE02.CP01-CP02.YE01)/(px0.CP01) ]


14) divide L6 per a66

[ 1  py0/px0  0         0      1/px0          0 |                                         ex0/px0 ]
[ 0        1  0         0  Vx01/CP01          0 |                                       XE01/CP01 ]
[ 0        0  1   py0/px0          0      1/px0 |                                         ey0/px0 ]
[ 0        0  0         1          0  Vx01/CP01 |                                       YE01/CP01 ]
[ 0        0  0         0          1          0 |                 (XE02 - CP02.ex0)/(Vx02 - CP02) ]
[ 0        0  0         0          0          1 | (YE02.CP01 - CP02.YE01)/(Vx02.CP01 - CP02.Vx01) ]


15) beautify pass

[ 1  py0/px0  0         0      1/px0          0 |                                         ex0/px0 ]
[ 0        1  0         0  Vx01/CP01          0 |                                       XE01/CP01 ]
[ 0        0  1   py0/px0          0      1/px0 |                                         ey0/px0 ]
[ 0        0  0         1          0  Vx01/CP01 |                                       YE01/CP01 ]
[ 0        0  0         0          1          0 |                 (XE02 - CP02.ex0)/(Vx02 - CP02) ]
[ 0        0  0         0          0          1 | (YE02.CP01 - YE01.CP02)/(Vx02.CP01 - Vx01.CP02) ]

16) resolve the model coeffiscients by resolving backward over this equation:

| 1  py0/px0  0         0      1/px0          0 |   |  a |   |                                         ex0/px0 |
| 0        1  0         0  Vx01/CP01          0 |   |  b |   |                                       XE01/CP01 |
| 0        0  1   py0/px0          0      1/px0 | . |  c | = |                                         ey0/px0 |
| 0        0  0         1          0  Vx01/CP01 |   |  d |   |                                       YE01/CP01 |
| 0        0  0         0          1          0 |   | tx |   |                 (XE02 - CP02.ex0)/(Vx02 - CP02) |
| 0        0  0         0          0          1 |   | ty |   | (YE02.CP01 - YE01.CP02)/(Vx02.CP01 - Vx01.CP02) |

This is gives:

ty = (YE02.CP01 - YE01.CP02) / (Vx02.CP01 - Vx01.CP02)
tx = (XE02 - CP02.ex0) / (Vx02 - CP02)
d  = (YE01 - ty.Vx01) / CP01
c  = (ey0 - d.py0 - ty) / px0
b  = (XE01 - tx.Vx01) / CP01
a  = (ex0 - b.py0 - tx) / px0

Note2: I doubt of tx result, I'll use this: (XE02.CP01 - XE01.CP02) / (Vx02.CP01 - Vx01.CP02)
=> gives better results.
"""
