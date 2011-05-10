from motion import *
from sys import argv
from glob import glob

files = sorted(glob(argv[1]))

im1 = load_image(files[0])
corners = detect_corners(im1.grayscale, 0.3, 500)

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

from math import acos, asin, degrees

with open(argv[2], 'w') as fp:
    for i in range(1, len(files)):
        im2 = load_image(files[i])

        ctx.track(im1, im2, ftset)
        print("Frame %u: remains %u tracker(s)" % (i, len(ftset.tracked)))
        
        P = ftset.tracked
        E = ftset.estimations
        
        A = []
        B = []
        C = []
        D = []
        Tx = []
        Ty = []
        Ex = []
        Ey = []
        
        for j in range(len(P)//3):
            mat1 = [ [ P[j  ][0], P[j  ][1], 1.0, E[j  ][0] ],
                     [ P[j+1][0], P[j+1][1], 1.0, E[j+1][0] ],
                     [ P[j+2][0], P[j+2][1], 1.0, E[j+2][0] ] ]
            if gauss3x4(mat1) is not None:
                mat2 = [ [ P[j  ][0], P[j  ][1], 1.0, E[j  ][1] ],
                         [ P[j+1][0], P[j+1][1], 1.0, E[j+1][1] ],
                         [ P[j+2][0], P[j+2][1], 1.0, E[j+2][1] ] ]
                if gauss3x4(mat2) is not None:
                    s = 1. / max(mat1[0][3], mat2[1][3])
                    A.append(mat1[0][3]*s)
                    B.append(mat1[1][3]*s)
                    C.append(mat2[0][3]*s)
                    D.append(mat2[1][3]*s)
                    Tx.append(mat1[2][3]*s)
                    Ty.append(mat2[2][3]*s)
                    
                    Ex.append(abs((mat1[0][3]*P[j][0]*s + mat1[1][3]*P[j][1]*s + mat1[2][3]*s) - E[j][0]*s))
                    Ey.append(abs((mat2[0][3]*P[j][0]*s + mat2[1][3]*P[j][1]*s + mat2[2][3]*s) - E[j][1]*s))
        
        n = len(A)
        A=sum(A)/n
        B=sum(B)/n
        C=sum(C)/n
        D=sum(D)/n
        Tx=sum(Tx)/n
        Ty=sum(Ty)/n
        Ex=sum(Ex)/n
        Ey=sum(Ey)/n
        
        print("Frame #%u: got %u sample(s), A=%g, B=%g, Tx=%le, Ty=%le, Ex=%le, Ey=%le" % (i, n, degrees(acos(A)), degrees(asin(-B)), Tx, Ty, Ex, Ey))
        fp.write("%le %le %le %le %u\n" % (A, -B, Tx, Ty, i))

        ftset.trackers = ftset.estimations
        im1.flush()
        im1 = im2

del im1, im2, ctx, ftset

"""
[a b tx] . [px py 1]T = [ex]
[c d ty]                [ey]

rot matrix:
a = d = cos(teta)
b = -c = -sin(teta)

ex[i] = a.px[i]+b.py[i]+tx
ey[i] = b.py[i]-b.px[i]+ty

[px[0] py[0] 1 | ex[0]]
[px[1] py[1] 1 | ex[1]]
[px[2] py[2] 1 | ex[2]]

[px[0] py[0] 1 | ey[0]]
[px[1] py[1] 1 | ey[1]]
[px[2] py[2] 1 | ey[2]]



"""