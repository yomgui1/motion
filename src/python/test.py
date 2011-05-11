from motion import *
from sys import argv
from glob import glob
from random import sample
import sys

outliers_prob = 0.5

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

def getSamples(P, n):
    r = range(len(P))
    inliers = list(sample(r, n))
    return inliers, [ j for j in r if j not in inliers ]
        
def computeAffineMatrix(idx, P, E):
    """See notes below.
    """

    P = [P[j] for j in idx]
    E = [E[j] for j in idx]

    # Use first 3-points to compute basic matrix coeffiscients
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
    den = Dx02*P1 - Dx01*P2
    
    ty = ((ey2*px0 - px2*ey0)*P1 - E2*P2) / den
    tx = ((ex2*px0 - px2*ex0)*P1 - E1*P2) / den
    d = (E2 - ty*Dx01) / P1
    c = (ey0 - d*py0 - ty) / px0
    b = (E1 - tx*Dx01) / P1
    a = (ex0 - b*py0 - tx) / px0
    
    mat = [a,b,c,d,tx,ty]

    # Then compute and sum matrix coef. for remaining points
    for p, e in zip(P[3:], E[3:]):
        pxi = p[0]
        pyi = p[1]
        exi = e[0]
        eyi = e[1]
        
        Pi = pyi*px0 - pxi*py0
        Dx0i = px0 - pxi
        den = Dx0i*P1 - Dx01*Pi
        
        ty = ((eyi*px0 - pxi*ey0)*P1 - E2*Pi) / den
        tx = ((exi*px0 - pxi*ex0)*P1 - E1*Pi) / den
        d = (E2 - ty*Dx01) / P1
        c = (ey0 - d*py0 - ty) / px0
        b = (E1 - tx*Dx01) / P1
        a = (ex0 - b*py0 - tx) / px0

        mat[0] += a
        mat[1] += b
        mat[2] += c
        mat[3] += d
        mat[4] += tx
        mat[5] += ty
        
    n = len(idx)
    return tuple(v/n for v in mat)
        
def pointError(p, e, mat):
    ex = (mat[0]*p[0] - mat[1]*p[1] + mat[4]) - e[0]
    ey = (mat[2]*p[0] + mat[3]*p[1] + mat[5]) - e[1]
    return ex*ex + ey*ey
    
def computeError(idx, P, E, mat):
        err = 0.0
        for j in idx:
            err += pointError(P[j], E[j], mat)
        return err/len(idx)

with open(argv[2], 'w') as fp:
    for i in range(1, len(files)):
        im2 = load_image(files[i])

        ctx.track(im1, im2, ftset)

        P = [(x-cx, y-cy) for x,y in ftset.tracked]
        E = [(x-cx, y-cy) for x,y in ftset.estimations]
        max_local = max(3, int(len(P) * (1-outliers_prob)))

        print("Frame %u: remains %u tracker(s), max inliers: %u" % (i, len(ftset.tracked), max_local))
        
        # Start using a good samples
        if len(P) > 3:
            inliers, outliers = getSamples(P, max_local)
        else:
            inliers = [0,1,2]
            outliers = []
        
        allindx = range(len(P))
        frame_mat = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0]
        frame_err = 1e+52
        retry = 0
        
        while 1:
            mat = [0.0]*6

            # Compute frame matrix with inliners
            mat = computeAffineMatrix(inliers, P, E)
            
            # Compute the average error on this matrix for inliers
            loc_err = computeError(inliers, P, E, mat)
            #print("Frame %u: Err=%le (%u inliers)" % (i, loc_err, len(inliers)))
            
            if loc_err > frame_err:
                #print("Frame %u: err inc (frame err=%g)" % (i, frame_err))
                print("Frame %u: Err=%le, a=%g, b=%g, c=%g, d=%g, tx=%g, ty=%g" % ((i,frame_err)+frame_mat))
                fp.write("%le %le %le %le %le %le %u\n" % (frame_mat+(i,)))
                break
                
            frame_err = loc_err
            frame_mat = mat
            del mat, loc_err
            
            # Remove all inliers with bigger error than this average
            for j in tuple(inliers):
                err = pointError(P[j], E[j], frame_mat)
                if err > frame_err:
                    inliers.remove(j)
                    outliers.append(j)
                    
            frame_err = computeError(inliers, P, E, frame_mat)
            #print("Frame %u: refined Err=%g (%u inliers, %u outliers)" % (i, frame_err, len(inliers), len(outliers)))
            
            # Tests all outliers to find new inliers
            for j in tuple(outliers):
                if len(inliers) == max_local: break
                err = pointError(P[j], E[j], frame_mat)
                if err <= frame_err:
                    outliers.remove(j)
                    inliers.append(j)

            # Not enough inliers?
            if len(inliers) < max_local:
                if frame_err > 1.0:
                    retry += 1
                    if retry <= 200:
                        frame_mat = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0]
                        frame_err = 1e+52
                        inliers, outliers = getSamples(P, max_local)
                        sys.stdout.write("  retry %u\r" % retry)
                        continue
                    else:
                        print("Frame %u: too much error, use identity motion matrix!" % i)
                        fp.write("1.0 0.0 0.0 1.0 0.0 0.0 %u\n" % i)
                        break
                
                print("Frame %u: Err=%le, a=%g, b=%g, c=%g, d=%g, tx=%g, ty=%g" % ((i,frame_err)+frame_mat))
                fp.write("%le %le %le %le %le %le %u\n" % (frame_mat+(i,)))
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

[ px(0)   py(0)     0      0     1 0 | ex(0) ]
[   0       0     px(0)   py(0)  0 1 | ey(0) ]
[ px(1)   py(1)     0      0     1 0 | ex(1) ]
[   0       0     px(1)   py(1)  0 1 | ey(1) ]
[ px(2)   py(2)     0      0     1 0 | ex(2) ]
[   0       0     px(2)   py(2)  0 1 | ey(2) ]

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
c = (ey0 - d.py0 - ty) / px0
b = (E1 - tx.Dx01) / P1
a = (ex0 - b.py0 - tx) / px0

=== Generalisation to more points ===

if we add more 2D points to the last matrix:

[ px(n-1) py(n-1)        0        0  1 0 | ex(n-1) ]
[       0       0  px(n-1)  py(n-1)  0 1 | ey(n-1) ]

These lines are solved using combination of the upper ones by doing substraction to obtain zeros:

[       1  py0/px0        0        0  1/px0 0 | ex0/px0 ] (line 1)
...
[ px(n-1)  py(n-1)        0        0      1 0 | ex(n-1) ]
[       0        0  px(n-1)  py(n-1)      0 1 | ey(n-1) ]

[ 0                              1        0        0  (px0-px1)/(py1.px0-px1.py0) 0 | (ex1.px0-px1.ex0)/(py1.px0-px1.py0) ] (line2)
...
[ 0  (py(n-1).px0-px(n-1).py0)/px0        0        0            (px0-px(n-1))/px0 0 |      (ex(n-1).px0-px(n-1).ex0)/px0 ]
[ 0                              0  px(n-1)  py(n-1)                            0 1 |                            ey(n-1) ]

[ 0  0  1                        py0/px0                                                                                              0              1/px0 |                                                                                                           ey0/px0 ] (line3)
...
[ 0  0  0                              0  ((px0-px(n-1)).(py1.px0-px1.py0)-(py(n-1).px0-px(n-1).py0).(px0-px1))/(px0.(py1.px0-px1.py0))                  0 | ((ex(n-1).px0-px(n-1).ex0).(py1.px0-px1.py0)-(py(n-1).px0-px(n-1).py0).(ex1.px0-px1.ex0))/(px0.(py1.px0-px1.py0)) ]
[ 0  0  0  (py(n-1).px0-px(n-1).py0)/px0                                                                                              0  (px0-px(n-1))/px0 |                                                                                     (ey(n-1).px0-px(n-1).ey0)/px0 ]


[ 0  0  0  1                                                                                              0                                                                    (px0-px1)/(py1.px0-px1.py0) |                                                                               (ey1.px0-px1.ey0)/(py1.px0-px1.py0) ] (line4)
...
[ 0  0  0  0  ((px0-px(n-1)).(py1.px0-px1.py0)-(py(n-1).px0-px(n-1).py0).(px0-px1))/(px0.(py1.px0-px1.py0))                                                                                              0 | ((ex(n-1).px0-px(n-1).ex0).(py1.px0-px1.py0)-(ex1.px0-px1.ex0).(py(n-1).px0-px(n-1).py0))/(px0.(py1.px0-px1.py0)) ]
[ 0  0  0  0                                                                                              0  ((px0-px(n-1)).(py1.px0-px1.py0)-(px0-px1).(py(n-1).px0-px(n-1).py0))/(px0.(py1.px0-px1.py0)) | ((ey(n-1).px0-px(n-1).ey0).(py1.px0-px1.py0)-(ey1.px0-px1.ey0).(py(n-1).px0-px(n-1).py0))/(px0.(py1.px0-px1.py0)) ]

[ 0  0  0  0                                                           1                                                           0 |                                                                   tx ] (line5)
...
[ 0  0  0  0  ((px0-px(n-1)).P1-(py(n-1).px0-px(n-1).py0).Dx01)/(px0.P1)                                                           0 | ((ex(n-1).px0-px(n-1).ex0).P1-E1.(py(n-1).px0-px(n-1).py0))/(px0.P1) ]
[ 0  0  0  0                                                           0  ((px0-px(n-1)).P1-Dx01.(py(n-1).px0-px(n-1).py0))/(px0.P1) | ((ey(n-1).px0-px(n-1).ey0).P1-E1.(py(n-1).px0-px(n-1).py0))/(px0.P1) ]

[ 0  0  0  0  1  0 |                                                                   tx ] (line5)
[ 0  0  0  0  0  1 |                                                                   ty ] (line6)
...
[ 0  0  0  0  0  0 | (((ex(n-1).px0 - px(n-1).ex0 - tx.(px0-px(n-1))).P1 - (E1 - tx.Dx01).(py(n-1).px0 - px(n-1).py0)) / (px0.P1) ]
[ 0  0  0  0  0  0 | (((ey(n-1).px0 - px(n-1).ey0 - ty.(px0-px(n-1))).P1 - (E1 - ty.Dx01).(py(n-1).px0 - px(n-1).py0)) / (px0.P1) ]

This gives following equations:

(((ex(n-1).px0 - px(n-1).ex0 - tx.(px0-px(n-1))).P1 - (E1 - tx.Dx01).(py(n-1).px0 - px(n-1).py0)) / (px0.P1) = 0
(((ey(n-1).px0 - px(n-1).ey0 - ty.(px0-px(n-1))).P1 - (E1 - ty.Dx01).(py(n-1).px0 - px(n-1).py0)) / (px0.P1) = 0

((ex(n-1).px0 - px(n-1).ex0 - tx.(px0-px(n-1))).P1 - (E1 - tx.Dx01).(py(n-1).px0 - px(n-1).py0) = 0
((ey(n-1).px0 - px(n-1).ey0 - ty.(px0-px(n-1))).P1 - (E1 - ty.Dx01).(py(n-1).px0 - px(n-1).py0) = 0

((ex(n-1).px0 - px(n-1).ex0 - tx.(px0-px(n-1))).P1 = (E1 - tx.Dx01).(py(n-1).px0 - px(n-1).py0)
((ey(n-1).px0 - px(n-1).ey0 - ty.(px0-px(n-1))).P1 = (E1 - ty.Dx01).(py(n-1).px0 - px(n-1).py0)

(ex(n-1).px0 - px(n-1).ex0).P1 - tx.(px0-px(n-1)).P1 = E1.(py(n-1).px0 - px(n-1).py0) - tx.Dx01.(py(n-1).px0 - px(n-1).py0)
(ey(n-1).px0 - px(n-1).ey0).P1 - ty.(px0-px(n-1)).P1 = E1.(py(n-1).px0 - px(n-1).py0) - ty.Dx01.(py(n-1).px0 - px(n-1).py0)

tx.Dx01.(py(n-1).px0 - px(n-1).py0) - tx.(px0-px(n-1)).P1 = E1.(py(n-1).px0 - px(n-1).py0) - P1.(ex(n-1).px0 - px(n-1).ex0)
ty.Dx01.(py(n-1).px0 - px(n-1).py0) - ty.(px0-px(n-1)).P1 = E1.(py(n-1).px0 - px(n-1).py0) - P1.(ey(n-1).px0 - px(n-1).ey0)

tx.(Dx01.(py(n-1).px0 - px(n-1).py0) - P1.(px0-px(n-1))) = E1.(py(n-1).px0 - px(n-1).py0) - P1.(ex(n-1).px0 - px(n-1).ex0)
ty.(Dx01.(py(n-1).px0 - px(n-1).py0) - P1.(px0-px(n-1))) = E1.(py(n-1).px0 - px(n-1).py0) - P1.(ey(n-1).px0 - px(n-1).ey0)

tx = (E1.(py(n-1).px0 - px(n-1).py0) - P1.(ex(n-1).px0 - px(n-1).ex0)) / (Dx01.(py(n-1).px0 - px(n-1).py0) - P1.(px0-px(n-1)))
ty = (E1.(py(n-1).px0 - px(n-1).py0) - P1.(ey(n-1).px0 - px(n-1).ey0)) / (Dx01.(py(n-1).px0 - px(n-1).py0) - P1.(px0-px(n-1)))

well... then a,b,c and d can be found using upper equations and this both results.
"""
