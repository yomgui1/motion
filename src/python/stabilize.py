#
# Keep 3.x/2.x compatibility (except external modules)
#

# Python 2.5 compatibility
from __future__ import with_statement

from motion import *
from sys import argv
from glob import glob
from random import sample, shuffle
from math import log
from warp import WarpTool

import sys, os

# Value used to compute number of RANSAC iterations
# Must be in [0, 1] range.
# Lower is, higher iterations will be.
outliers_prob = 0.05

# maximal model average error on all trackers per frame
max_error = .5

good_fac = 0.5

max_max_iterations = 1000

files = sorted(glob(argv[1]))

im1 = load_image(files[0])
corners = detect_corners(im1.grayscale, 0.3, 500)

ctx = KLTContext()
ftset = FeatureSet()

ftset.trackers = corners
del corners
print("%u tracker(s) found." % len(ftset.trackers))


def solve2DAffineMatrix(samples):
    """solve2DAffineMatrix(samples) -> err
    Solve the equation M.p = e for three (p, e) points in samples.

    M : is a 3x2 matrix, describing an affine transformation (model).
    p : is the input 2D point
    e : is an estimation of p.

    See notes below to know how this equations have been found.
    Note also due to divisions during solving, p0 should not be at
    coordinates (0,0) and points should not be colinears to not
    divide by zero.
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

    den = (Vx02*YP01 - Vx01*YP02) # 0 if px0 == 0
    if not (den and YP01):
        return

    ty = (YE02*YP01 - YE01*YP02) / den
    tx = (XE02*YP01 - XE01*YP02) / den

    d  = (YE01 - ty*Vx01) / YP01
    c  = (ey0 - d*py0 - ty) / px0
    b  = (XE01 - tx*Vx01) / YP01
    a  = (ex0 - b*py0 - tx) / px0

    return (a,c,b,d,tx,ty)

def getError(model, p, e):
    ex = model[0]*p[0] + model[2]*p[1] + model[4] - e[0]
    ey = model[1]*p[0] + model[3]*p[1] + model[5] - e[1]
    return ex*ex + ey*ey

def getScore(samples, model, threshold):
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
min_samples = 3

warp = WarpTool()

# record first image as it
warp.init_reference(im1)
warp.warp(os.path.join(argv[3], "frame_%04u.png" % 0), im1)

with open(argv[2], 'w') as fp:
    for i in range(1, len(files)):
        # reset frame state variables
        best_score = 1e400 # inf for 64bits floating point
        best_model = None

        # load second image and track
        im2 = load_image(files[i])
        ctx.track(im1, im2, ftset)

        # prepare samples
        estimations = tuple(t[-2:] for t in ftset.estimations)
        allsamples = tuple(zip((t[-2:] for t in ftset.tracked), estimations))
        min_inliers = len(allsamples)*good_fac

        print("Frame %u: remains %u tracker(s), min_inliers=%u" % (i, len(allsamples), min_inliers))

        # enough trackers for solving?
        if min_samples > len(allsamples):
            fp.write("1 0 0 1 0 0 %u\n" % i)
            continue

        # Applying RANSAC algorithme to find the best model
        min_inliers_frame = min_inliers
        while not best_model:
                        k = 0
                        max_iterations = max_max_iterations
                        while k < max_iterations:

                                # Compute a model for a random set of points
                                model = None
                                while not model:
                                        model = solve2DAffineMatrix(sample(allsamples, min_samples))

                                # Compute the summed square error over all points for the model
                                # and find the model giving the minimal value.
                                score, inliers = getScore(allsamples, model, threshold)
                                if len(inliers) > min_inliers_frame and score < best_score:
                                        best_score = score
                                        best_model = model
                                        best_inliers = inliers
                                        w = len(best_inliers) / float(len(allsamples))
                                        if w < 1.0:
                                                max_iterations = int(log(outliers_prob) / log(1.0 - pow(w, min_samples)))
                                                max_iterations = min(max_iterations, max_max_iterations)

                                k += 1

                        # when no model fit, try to decrease the number of requiered inliers per model
                        if not best_model:
                                if min_inliers_frame == min_samples:
                                        best_model = (1,0,0,1,0,0)
                                        best_score = threshold*len(allsamples)
                                        best_inliers = min_samples
                                        break
                                print("Frame %u: no suitable model found with a minimal of %u inliers, retrying with 10%% less..." % (i, min_inliers_frame))
                                min_inliers_frame *= 0.9 # 10% less

        print("Frame %u: best err of %le over %u inliers (%u iterations)" % (i, best_score/len(allsamples), len(best_inliers), k))
        print("Frame %u: model=(%le, %le, %le, %le, %le, %le)" % ((i,)+best_model))
        fp.write("%le %le %le %le %le %le %u\n" % (best_model + (i,)))

        filename = os.path.join(argv[3], "frame_%04u.png" % i)
        warp.warp(filename, im2, best_model, clear=True)

        ftset.trackers = estimations
        im1.flush()
        im1 = im2

warp.print_crop()

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

The resolution of this matrix is detailed in doc/2d_gauss.org document
"""
