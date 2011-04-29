/* Original author: Edward Rosten
 * Modified by: Guillaume Roguez
 */

#include "fast.h"

#include <stdlib.h>

xy* fast9_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
{
	xy* corners;
	int num_corners;
	int* scores;
	xy* nonmax = NULL;

	corners = fast9_detect(im, xsize, ysize, stride, b, &num_corners);
    scores = fast9_score(im, stride, corners, num_corners, b);
    nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);
    
    free(corners);
	free(scores);
  
	return nonmax;
}
