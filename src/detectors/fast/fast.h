#ifndef FAST_H
#define FAST_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct { int x, y; } xy; 
typedef unsigned char byte;

extern int  fast9_corner_score(const byte* p, const int pixel[], int bstart);
extern xy*  fast9_detect(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
extern int* fast9_score(const byte* i, int stride, xy* corners, int num_corners, int b);
extern xy*  fast9_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
extern xy*  fast9_detect_limited(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners, int with_nonmax);
extern xy*  nonmax_suppression(const xy* corners, const int* scores, int num_corners, int* ret_num_nonmax);

#ifdef __cplusplus
}
#endif

#endif /* FAST_H */
