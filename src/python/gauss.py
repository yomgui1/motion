def gauss3x4(mat):
    # Pivot #1
    p = mat[0][0]
    if p == 0.0: return
    if p != 1.0:
        #mat[0][0] = 1.0
        mat[0][1] /= p 
        mat[0][2] /= p
        mat[0][3] /= p
    p = mat[1][0]
    if p != 0.0:
        #mat[1][0] = 0.0
        mat[1][1] -= p*mat[0][1]
        mat[1][2] -= p*mat[0][2]
        mat[1][3] -= p*mat[0][3]
    p = mat[2][0]
    if p != 0.0:
        #mat[2][0] = 0.0
        mat[2][1] -= p*mat[0][1]
        mat[2][2] -= p*mat[0][2]
        mat[2][3] -= p*mat[0][3]
        
    # Pivot #2
    p = mat[1][1]
    if p == 0.0: return
    if p != 1.0:
        #mat[1][0] /= p
        #mat[1][1] = 1.0 
        mat[1][2] /= p
        mat[1][3] /= p
    p = mat[0][1]
    if p != 0.0:
        #mat[0][0] -= p*mat[1][0]
        #mat[0][1] = 0.0
        mat[0][2] -= p*mat[1][2]
        mat[0][3] -= p*mat[1][3]
    p = mat[2][1]
    if p != 0.0:
        #mat[2][0] -= p*mat[1][0]
        #mat[2][1] = 0.0
        mat[2][2] -= p*mat[1][2]
        mat[2][3] -= p*mat[1][3]
        
    # Pivot #3
    p = mat[2][2]
    if p == 0.0: return
    if p != 1.0:
        #mat[2][0] /= p
        #mat[2][1] /= p
        #mat[2][2] = 1.0
        mat[2][3] /= p
    p = mat[0][2]
    if p != 0.0:
        #mat[0][0] -= p*mat[2][0]
        #mat[0][1] -= p*mat[2][1]
        #mat[0][2] = 0.0
        mat[0][3] -= p*mat[2][3]
    p = mat[1][2]
    if p != 0.0:
        #mat[1][0] -= p*mat[2][0]
        #mat[1][1] -= p*mat[2][1]
        #mat[1][2] = 0.0
        mat[1][3] -= p*mat[2][3]
        
    return mat
    
mat = [[1., -1.,  2.,   5.],
       [3.,  2.,  1.,  10.],
       [2., -3., -2., -10.] ]

mat = [[10., 10.,  1.,   20.],
       [30., 25.,  1.,   40.],
       [40.,  2.,  1.,   50.] ]
       
assert gauss3x4(mat)
print mat[0]
print mat[1]
print mat[2]
