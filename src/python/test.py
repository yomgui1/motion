from motion import *
from sys import argv
from glob import glob

files = sorted(glob(argv[1]))

im1 = load_image(files[0])
corners = detect_corners(im1.grayscale, 0.3, 500)

cx = im1.width/2
cy = im1.height/2

ftset = FeatureSet()
ftset.trackers = corners
del corners

print("%u tracker(s) found." % len(ftset.trackers))

ctx = KLTContext()

def gauss3x4(mat):
    # Pivot #1
    p = mat[0][0]
    if p == 0.0: return
    if p != 1.0:
        mat[0][1] /= p 
        mat[0][2] /= p
        mat[0][3] /= p
    p = mat[1][0]
    if p != 0.0:
        mat[1][1] -= p*mat[0][1]
        mat[1][2] -= p*mat[0][2]
        mat[1][3] -= p*mat[0][3]
    p = mat[2][0]
    if p != 0.0:
        mat[2][1] -= p*mat[0][1]
        mat[2][2] -= p*mat[0][2]
        mat[2][3] -= p*mat[0][3]
        
    # Pivot #2
    p = mat[1][1]
    if p == 0.0: return
    if p != 1.0:
        mat[1][2] /= p
        mat[1][3] /= p
    p = mat[0][1]
    if p != 0.0:
        mat[0][2] -= p*mat[1][2]
        mat[0][3] -= p*mat[1][3]
    p = mat[2][1]
    if p != 0.0:
        mat[2][2] -= p*mat[1][2]
        mat[2][3] -= p*mat[1][3]
        
    # Pivot #3
    p = mat[2][2]
    if p == 0.0: return
    if p != 1.0:
        mat[2][3] /= p
    p = mat[0][2]
    if p != 0.0:
        mat[0][3] -= p*mat[2][3]
    p = mat[1][2]
    if p != 0.0:
        mat[1][3] -= p*mat[2][3]
        
    return mat

from math import acos, asin, atan, degrees
from random import sample

max_local = 25

def getSamples(P, n):
    r = range(len(P))
    inliers = list(sample(r, n))
    return inliers, [ j for j in r if j not in inliers ]
        
def computeMatrix(j, P, E):
    """See notes below.
    """
    
    px0 = P[0][0]
    px1 = P[1][0]
    px2 = P[2][0]
    py0 = P[0][1]
    py1 = P[1][1]
    py2 = P[2][1]
    ex0 = E[0][0]
    ex1 = E[1][0]
    ex2 = E[2][0]
    ey0 = E[0][1]
    ey1 = E[1][1]
    ey2 = E[2][1]
    P1 = py1*px0 - px1*py0
    P2 = py2*px0 - px2*py0
    E1 = ex1*px0 - px1*ex0
    E2 = ey1*px0 - px1*ey0
    Dx02 = px0-px2
    Dx01 = px0-px1
    ty = ((ey2*px0-px2*ey0)*P1 - E2*P2) / (Dx02*P1 - Dx01*P2)
    tx = ((ex2*px0-px2*ex0)*P1 - E1*P2) / (Dx02*P1 - Dx01*P2)
    d = (E2 - ty*Dx01) / P1
    c = (ey0 - d*py0 - ty)  / px0
    b = (E1 - tx*Dx01) / P1
    a = (ex0 - b*py0 - tx)  / px0
    return [a,b,c,d,tx,ty]
        
def errorForIdx(j, P, E, mat):
    ex = (mat[0]*P[j][0] - mat[1]*P[j][1] + mat[4]) - E[j][0]
    ey = (mat[2]*P[j][0] + mat[3]*P[j][1] + mat[5]) - E[j][1]
    return ex*ex + ey*ey
    
def computeError(idx, P, E, mat):
        err = 0.0
        for j in idx:
            err += errorForIdx(j, P, E, mat)
        return err/len(idx)

with open(argv[2], 'w') as fp:
    for i in range(1, len(files)):
        im2 = load_image(files[i])

        ctx.track(im1, im2, ftset)
        print("Frame %u: remains %u tracker(s)" % (i, len(ftset.tracked)))
        
        P = [(x-cx, y-cy) for x,y in ftset.tracked]
        E = [(x-cx, y-cy) for x,y in ftset.estimations]
        
        # Start using a good samples
        inliers, outliers = getSamples(P, max_local)
        
        allindx = range(len(P))
        frame_mat = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0]
        frame_err = 1e+52
        
        while 1:
            mat = [0.0]*6

            # Compute frame matrix with inliners
            for j in inliers[:-3]:
                for k, v in enumerate(computeMatrix(j, P[j:j+3], E[j:j+3])):
                    mat[k] += v
            mat = [ v/len(inliers) for v in mat ]
            print("Frame %u: " % i, mat)
            
            # Compute the average error on this matrix for inliers
            loc_err = computeError(inliers, P, E, mat)
            print("Frame %u: Err=%g (%u inliers)" % (i, loc_err, len(inliers)))
            
            if loc_err > frame_err:
                print("Frame %u: err inc (frame err=%g)" % (i, frame_err))
                for j in tuple(inliers):
                    err = errorForIdx(j, P, E, mat)
                    if err > frame_err:
                        inliers.remove(j)
                        outliers.append(j)
                continue
                
            frame_err = loc_err
            frame_mat = mat
            del mat, loc_err
            
            # Remove all inliers with bigger error than this average
            for j in tuple(inliers):
                err = errorForIdx(j, P, E, frame_mat)
                if err > frame_err:
                    inliers.remove(j)
                    outliers.append(j)
                    
            frame_err = computeError(inliers, P, E, frame_mat)
            print("Frame %u: refined Err=%g (%u inliers, %u outliers)" % (i, frame_err, len(inliers), len(outliers)))
            
            # Tests all outliers to find new inliers
            for j in tuple(outliers):
                if len(inliers) == max_local: break
                err = errorForIdx(j, P, E, frame_mat)
                if err <= frame_err:
                    outliers.remove(j)
                    inliers.append(j)
                    
            if len(inliers) < max_local:
                for j in outliers:
                    print(frame_err, errorForIdx(j, P, E, frame_mat))
                break
                    
            # New inliers ?
            if 0:
                print("Frame #%u: Tx=%g, Ty=%g, LocErr(%u)=%g GlobErr=%g" % (i, angle, GTx, GTy, oN, oErr, Err))
                fp.write("%le %le %le %le %u\n" % (GAc, GCc, GTx, GTy, i))
                break

        ftset.trackers = ftset.estimations
        im1.flush()
        im1 = im2

    del im1, im2
del ctx, ftset

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

[ px(0)   py(0)     0      0     1 0 | ex(0) ] | a|
[   0       0     px(0)   py(0)  0 1 | ey(0) ] | b|
[ px(1)   py(1)     0      0     1 0 | ex(1) ] | c|
[   0       0     px(1)   py(1)  0 1 | ey(1) ] | d|
[ px(2)   py(2)     0      0     1 0 | ex(2) ] |tx|
[   0       0     px(2)   py(2)  0 1 | ey(2) ] |ty|

[ px0     py0       0       0  1 0 | ex0 ]
[   0       0     px0     py0  0 1 | ey0 ]
[ px1     py1       0       0  1 0 | ex1 ]
[   0       0     px1     py1  0 1 | ey1 ]
[ px2     py2       0       0  1 0 | ex2 ]
[   0       0     px2     py2  0 1 | ey2 ]

[ px0     py0       0       0  1 0 | ex0 ]
[ px1     py1       0       0  1 0 | ex1 ]
[   0       0     px0     py0  0 1 | ey0 ]
[   0       0     px1     py1  0 1 | ey1 ]
[ px2     py2       0       0  1 0 | ex2 ]
[   0       0     px2     py2  0 1 | ey2 ]

[   1   py0/px0     0       0  1/px0 0 | ex0/px0 ]
...

[ 1          py0/px0    0    0      1/px0  0 |         ex0/px0 ]
[ 0  py1-px1.py0/px0    0    0  1-px1/px0  0 | ex1-px1.ex0/px0 ]
[ 0                0  px0  py0          0  1 |             ey0 ]
[ 0                0  px1  py1          0  1 |             ey1 ]
[ 0  py2-px2.py0/px0    0    0  1-px2/px0  0 | ex2-px2.ex0/px0 ]
[ 0                0  px2  py2          0  1 |             ey2 ]

[ 1          py0/px0    0    0                        1/px0  0 |                             ex0/px0 ]
[ 0                1    0    0  (px0-px1)/(py1.px0-px1.py0)  0 | (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0                0  px0  py0                            0  1 |                                 ey0 ]
[ 0                0  px1  py1                            0  1 |                                 ey1 ]
[ 0  py2-px2.py0/px0    0    0                    1-px2/px0  0 |                     ex2-px2.ex0/px0 ]
[ 0                0  px2  py2                            0  1 |                                 ey2 ]

[ 1  py0/px0    0    0                                                                              1/px0  0 |                                                                                           ex0/px0 ]
[ 0        1    0    0                                                        (px0-px1)/(py1.px0-px1.py0)  0 |                                                               (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0        0  px0  py0                                                                                  0  1 |                                                                                               ey0 ]
[ 0        0  px1  py1                                                                                  0  1 |                                                                                               ey1 ]
[ 0        0    0    0  ((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0))  0 | ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) ]
[ 0        0  px2  py2                                                                                  0  1 |                                                                                               ey2 ]

[ 1  py0/px0    0        0                                                                              1/px0      0 |                                                                                           ex0/px0 ]
[ 0        1    0        0                                                        (px0-px1)/(py1.px0-px1.py0)      0 |                                                               (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0        0    1  py0/px0                                                                                  0  1/px0 |                                                                                           ey0/px0 ]
[ 0        0  px1      py1                                                                                  0      1 |                                                                                               ey1 ]
[ 0        0    0        0  ((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0))      0 | ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) ]
[ 0        0  px2      py2                                                                                  0      1 |                                                                                               ey2 ]

[ 1  py0/px0    0                      0                                                                              1/px0              0 |                                                                                           ex0/px0 ]
[ 0        1    0                      0                                                        (px0-px1)/(py1.px0-px1.py0)              0 |                                                               (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0        0    1                py0/px0                                                                                  0          1/px0 |                                                                                           ey0/px0 ]
[ 0        0    0  (py1.px0-px1.py0)/px0                                                                                  0  (px0-px1)/px0 |                                                                             (ey1.px0-px1.ey0)/px0 ]
[ 0        0    0                      0  ((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0))              0 | ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) ]
[ 0        0    0  (py2.px0-px2.py0)/px0                                                                                  0  (px0-px2)/px0 |                                                                             (ey2.px0-px2.ey0)/px0 ]

[ 1  py0/px0    0                      0                                                                              1/px0                            0 |                                                                                           ex0/px0 ]
[ 0        1    0                      0                                                        (px0-px1)/(py1.px0-px1.py0)                            0 |                                                               (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0        0    1                py0/px0                                                                                  0                        1/px0 |                                                                                           ey0/px0 ]
[ 0        0    0                      1                                                                                  0  (px0-px1)/(py1.px0-px1.py0) |                                                               (ey1.px0-px1.ey0)/(py1.px0-px1.py0) ]
[ 0        0    0                      0  ((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0))                            0 | ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) ]
[ 0        0    0  (py2.px0-px2.py0)/px0                                                                                  0                (px0-px2)/px0 |                                                                             (ey2.px0-px2.ey0)/px0 ]

[ 1  py0/px0    0        0                                                                              1/px0                                                                                  0 |                                                                                           ex0/px0 ]
[ 0        1    0        0                                                        (px0-px1)/(py1.px0-px1.py0)                                                                                  0 |                                                               (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0        0    1  py0/px0                                                                                  0                                                                              1/px0 |                                                                                           ey0/px0 ]
[ 0        0    0        1                                                                                  0                                                        (px0-px1)/(py1.px0-px1.py0) |                                                               (ey1.px0-px1.ey0)/(py1.px0-px1.py0) ]
[ 0        0    0        0  ((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0))                                                                                  0 | ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) ]
[ 0        0    0        0                                                                                  0  ((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) | ((ey2.px0-px2.ey0).(py1.px0-px1.py0)-(ey1.px0-px1.ey0).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) ]

[ 1  py0/px0    0        0                        1/px0                                                                                  0 |                                                                                                                             ex0/px0 ]
[ 0        1    0        0  (px0-px1)/(py1.px0-px1.py0)                                                                                  0 |                                                                                                 (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0        0    1  py0/px0                            0                                                                              1/px0 |                                                                                                                             ey0/px0 ]
[ 0        0    0        1                            0                                                        (px0-px1)/(py1.px0-px1.py0) |                                                                                                 (ey1.px0-px1.ey0)/(py1.px0-px1.py0) ]
[ 0        0    0        0                            1                                                                                  0 | ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0)) ]
[ 0        0    0        0                            0  ((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) |                                   ((ey2.px0-px2.ey0).(py1.px0-px1.py0)-(ey1.px0-px1.ey0).(py2.px0-px2.py0))/(px0.(py1.px0-px1.py0)) ]

[ 1  py0/px0    0        0                        1/px0                            0 |                                                                                                                             ex0/px0 ]
[ 0        1    0        0  (px0-px1)/(py1.px0-px1.py0)                            0 |                                                                                                 (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ]
[ 0        0    1  py0/px0                            0                        1/px0 |                                                                                                                             ey0/px0 ]
[ 0        0    0        1                            0  (px0-px1)/(py1.px0-px1.py0) |                                                                                                 (ey1.px0-px1.ey0)/(py1.px0-px1.py0) ]
[ 0        0    0        0                            1                            0 | ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0)) ]
[ 0        0    0        0                            0                            1 | ((ey2.px0-px2.ey0).(py1.px0-px1.py0)-(ey1.px0-px1.ey0).(py2.px0-px2.py0))/((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0)) ]

ty = ((ey2.px0-px2.ey0).(py1.px0-px1.py0)-(ey1.px0-px1.ey0).(py2.px0-px2.py0))/((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))
tx = ((ex2.px0-px2.ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py2.px0-px2.py0))/((px0-px2).(py1.px0-px1.py0)-(px0-px1).(py2.px0-px2.py0))
d = ((ey1.px0-px1.ey0) - ty.(px0-px1))/(py1.px0-px1.py0)
c = (ey0 - d.py0 - ty)/px0
b = ((ex1.px0-px1.ex0) - tx.(px0-px1))/(py1.px0-px1.py0)
a = (ex0 - b.py0 - tx)/px0

P1 = py1.px0 - px1.py0
P2 = py2.px0 - px2.py0
E1 = ex1.px0 - px1.ex0
E2 = ey1.px0 - px1.ey0
Dx02 = px0-px2
Dx01 = px0-px1
ty = ((ey2.px0-px2.ey0).P1 - E2.P2) / (Dx02.P1 - Dx01.P2)
tx = ((ex2.px0-px2.ex0).P1 - E1.P2) / (Dx02.P1 - Dx01.P2)
d = (E2 - ty.Dx01) / P1
c = (ey0 - d.py0 - ty)  / px0
b = (E1 - tx.Dx01) / P1
a = (ex0 - b.py0 - tx)  / px0
"""
