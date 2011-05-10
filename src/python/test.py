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

max_local = 10

with open(argv[2], 'w') as fp:
    for i in range(1, len(files)):
        im2 = load_image(files[i])

        ctx.track(im1, im2, ftset)
        print("Frame %u: remains %u tracker(s)" % (i, len(ftset.tracked)))
        
        P = [(x-cx, y-cy) for x,y in ftset.tracked]
        E = [(x-cx, y-cy) for x,y in ftset.estimations]

        """
        dx = []
        dy = []
        for p, e in zip(P, E):
            dx.append(e[0]-p[0])
            dy.append(e[1]-p[1])
        dx=sum(dx)/len(dx)
        dy=sum(dy)/len(dy)
        print("Frame %u: %g,%g" % (i,dx,dy))
        """
        
        r = range(len(P)-3)
        inliners = list(sample(r, max_local))
        outliners = [j for j in r if j not in inliners]
        del r

        GAc = 0.0
        GCc = 0.0
        GTx = 0.0
        GTy = 0.0
        oErr = 1e+52
        bad_ge = 0
        ge_err = 0.01
        
        while 1:
            Ac = []
            Cc = []
            Tx = []
            Ty = []

            # values for inliners
            for j in inliners:
                mat = []
                for k in (0, 1, 2):
                    B = P[j+k][1]
                    C = P[j+k][0]
                    A = C*C + B*B
                    D = E[j+k][1]*C - E[j+k][0]*B
                    mat.append([ A, B, C, D ])
                
                if gauss3x4(mat) is not None:
                    c = mat[0][3]
                    tx = -mat[1][3]
                    ty = mat[2][3]

                    a = (E[j][0] + c*P[j][1] - tx) / P[j][0]

                    Ac.append(a)
                    Cc.append(c)
                    Tx.append(tx)
                    Ty.append(ty)

            del A,B,C,D,a,c

            n = len(Ac)
            Ac=sum(Ac)/n
            Cc=sum(Cc)/n
            Tx=sum(Tx)/n
            Ty=sum(Ty)/n

            Err=[]
            for j in inliners:
                ex = (Ac*P[j][0] - Cc*P[j][1] + Tx) - E[j][0]
                ey = (Cc*P[j][0] + Ac*P[j][1] + Ty) - E[j][1]
                err = ex*ex + ey*ey
                Err.append(err)
            N = len(Err)
            Err=sum(Err)/N

            # Local error increase and don't change?
            if Err >= oErr:
                # compute global error
                Err=[]
                for j in inliners:
                    ex = (Ac*P[j][0] - Cc*P[j][1] + Tx) - E[j][0]
                    ey = (Cc*P[j][0] + Ac*P[j][1] + Ty) - E[j][1]
                    err = ex*ex + ey*ey
                    Err.append(err)
                Err=sum(Err)/len(Err)

                # Keep global error low also (but accept more error)
                if Err > ge_err:
                    #print("GErr too high:", Err)
                    bad_ge += 1

                    # need to accept a bigger global error for this frame?
                    if bad_ge > 3:
                        ge_err *= 2
                        bad_ge = 0
                        
                    r = range(len(P)-3)
                    inliners = list(sample(r, max_local))
                    outliners = [j for j in r if j not in inliners]
                    del r

                    GAc = 0.0
                    GCc = 0.0
                    GTx = 0.0
                    GTy = 0.0
                    oErr = 1e+52
                    continue

                angle = degrees(atan(GCc/GAc))
                print("Frame #%u: angle=%g, Tx=%g, Ty=%g, LocErr(%u)=%g GlobErr=%g" % (i, angle, GTx, GTy, oN, oErr, Err))
                fp.write("%le %le %le %le %u\n" % (GAc, GCc, GTx, GTy, i))
                break

            # Keep local error low
            if Err > 0.1:
                #print("LErr too high:", Err)
                r = range(len(P)-3)
                inliners = list(sample(r, max_local))
                outliners = [j for j in r if j not in inliners]
                del r

                GAc = 0.0
                GCc = 0.0
                GTx = 0.0
                GTy = 0.0
                oErr = 1e+52
                continue
                
            oN = N
            oErr = Err
            GAc = Ac
            GCc = Cc
            GTx = Tx
            GTy = Ty

            # Refine inliners
            for j in inliners:
                ex = (Ac*P[j][0] - Cc*P[j][1] + Tx) - E[j][0]
                ey = (Cc*P[j][0] + Ac*P[j][1] + Ty) - E[j][1]
                err = ex*ex + ey*ey
                if err > Err:
                    outliners.append(j)
            inliners = [j for j in inliners if j not in outliners]

            # Check outliners
            for j in outliners:                
                ex = (Ac*P[j][0] - Cc*P[j][1] + Tx) - E[j][0]
                ey = (Cc*P[j][0] + Ac*P[j][1] + Ty) - E[j][1]
                err = ex*ex + ey*ey
                if err < Err:
                    inliners.append(j)

            outliners = [j for j in outliners if j not in inliners]
        
        ftset.trackers = ftset.estimations
        im1.flush()
        im1 = im2

    del im1, im2
del ctx, ftset

"""
[a b tx] . [px py 1]T = [ex]
[c d ty]                [ey]

rot matrix:
a = d = cos(teta)
b = -c = -sin(teta)

ex[i] = a.px[i]-c.py[i]+tx
ey[i] = c.px[i]+a.py[i]+ty

[px[0] -py[0] 1 | ex[0]]
[px[1] -py[1] 1 | ex[1]]
[px[2] -py[2] 1 | ex[2]]

[py[0] px[0] 1 | ey[0]]
[py[1] px[1] 1 | ey[1]]
[py[2] px[2] 1 | ey[2]]

(1) a = (ex[i] + c.py[i] - tx) / px[i]
(2) a = (ey[i] - c.px[i] - ty) / py[i]

(1) = (2) => (ex[i] + c.py[i] - tx) / px[i] = (ey[i] - c.px[i] - ty) / py[i]
<=> (ex[i] + c.py[i] - tx).py[i] = (ey[i] - c.px[i] - ty).px[i]
<=> ex[i].py[i] + c.py[i].py[i] - tx.py[i] = ey[i].px[i] - c.px[i].px[i] - ty.px[i]
<=> c.py[i].py[i] - tx.py[i] + c.px[i].px[i] + ty.px[i] = -ex[i].py[i] + ey[i].px[i]
<=> c.py[i]^2 - tx.py[i] + c.px[i]^2 + ty.px[i] = ey[i].px[i] - ex[i].py[i]
<=> c.(px[i]^2 + py[i]^2) - tx.py[i] + ty.px[i] = ey[i].px[i] - ex[i].py[i]

A[i]=px[i]^2 + py[i]^2
B[i]=py[i]
C[i]=px[i]
D[i]=ey[i].px[i] - ex[i].py[i]

[A[0] B[0] C[0] | D[0]]    [c=sin(teta)]
[A[1] B[1] C[1] | D[1]] => [-tx]
[A[2] B[2] C[2] | D[2]]    [ty]

"""
