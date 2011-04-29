/* Original author: Edward Rosten
 * Modified by: Guillaume Roguez
 */

/* This is mechanically generated code */

#include "fast.h"
#include "fast_utils.h"

#include <stdlib.h>

/* Return NULL if malloc failed or a valid pointer on xy values array.
 * The size of this array is given at function return by the value of ret_num_corners,
 * this value could be zero.
 * Image 'im' must be a grayscale pixel array.
 */
xy* fast9_detect(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
{
	int num_corners=0;
	xy* ret_corners;
	int rsize=512;
	int pixel[16];
	int x, y;

	ret_corners = (xy*)malloc(sizeof(xy)*rsize);
    if (!ret_corners)
        goto out;

	make_offsets(pixel, stride);

	for (y=3; y < ysize - 3; y++)
    {
		for (x=3; x < xsize - 3; x++)
		{
			const byte* p = im + y*stride + x;
			int cb = *p + b;
			int c_b= *p - b;

            if(p[pixel[0]] > cb)
                if(p[pixel[1]] > cb)
                    if(p[pixel[2]] > cb)
                        if(p[pixel[3]] > cb)
                            if(p[pixel[4]] > cb)
                                if(p[pixel[5]] > cb)
                                    if(p[pixel[6]] > cb)
                                        if(p[pixel[7]] > cb)
                                            if(p[pixel[8]] > cb)
                                            {}
                                            else
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    continue;
                                        else if(p[pixel[7]] < c_b)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    continue;
                                            else if(p[pixel[14]] < c_b)
                                                if(p[pixel[8]] < c_b)
                                                    if(p[pixel[9]] < c_b)
                                                        if(p[pixel[10]] < c_b)
                                                            if(p[pixel[11]] < c_b)
                                                                if(p[pixel[12]] < c_b)
                                                                    if(p[pixel[13]] < c_b)
                                                                        if(p[pixel[15]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else if(p[pixel[6]] < c_b)
                                        if(p[pixel[15]] > cb)
                                            if(p[pixel[13]] > cb)
                                                if(p[pixel[14]] > cb)
                                                {}
                                                else
                                                    continue;
                                            else if(p[pixel[13]] < c_b)
                                                if(p[pixel[7]] < c_b)
                                                    if(p[pixel[8]] < c_b)
                                                        if(p[pixel[9]] < c_b)
                                                            if(p[pixel[10]] < c_b)
                                                                if(p[pixel[11]] < c_b)
                                                                    if(p[pixel[12]] < c_b)
                                                                        if(p[pixel[14]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            if(p[pixel[7]] < c_b)
                                                if(p[pixel[8]] < c_b)
                                                    if(p[pixel[9]] < c_b)
                                                        if(p[pixel[10]] < c_b)
                                                            if(p[pixel[11]] < c_b)
                                                                if(p[pixel[12]] < c_b)
                                                                    if(p[pixel[13]] < c_b)
                                                                        if(p[pixel[14]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if(p[pixel[13]] < c_b)
                                            if(p[pixel[7]] < c_b)
                                                if(p[pixel[8]] < c_b)
                                                    if(p[pixel[9]] < c_b)
                                                        if(p[pixel[10]] < c_b)
                                                            if(p[pixel[11]] < c_b)
                                                                if(p[pixel[12]] < c_b)
                                                                    if(p[pixel[14]] < c_b)
                                                                        if(p[pixel[15]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else if(p[pixel[5]] < c_b)
                                    if(p[pixel[14]] > cb)
                                        if(p[pixel[12]] > cb)
                                            if(p[pixel[13]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                    if(p[pixel[10]] > cb)
                                                                        if(p[pixel[11]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                continue;
                                        else if(p[pixel[12]] < c_b)
                                            if(p[pixel[6]] < c_b)
                                                if(p[pixel[7]] < c_b)
                                                    if(p[pixel[8]] < c_b)
                                                        if(p[pixel[9]] < c_b)
                                                            if(p[pixel[10]] < c_b)
                                                                if(p[pixel[11]] < c_b)
                                                                    if(p[pixel[13]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if(p[pixel[14]] < c_b)
                                        if(p[pixel[7]] < c_b)
                                            if(p[pixel[8]] < c_b)
                                                if(p[pixel[9]] < c_b)
                                                    if(p[pixel[10]] < c_b)
                                                        if(p[pixel[11]] < c_b)
                                                            if(p[pixel[12]] < c_b)
                                                                if(p[pixel[13]] < c_b)
                                                                    if(p[pixel[6]] < c_b)
                                                                    {}
                                                                    else
                                                                        if(p[pixel[15]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        if(p[pixel[6]] < c_b)
                                            if(p[pixel[7]] < c_b)
                                                if(p[pixel[8]] < c_b)
                                                    if(p[pixel[9]] < c_b)
                                                        if(p[pixel[10]] < c_b)
                                                            if(p[pixel[11]] < c_b)
                                                                if(p[pixel[12]] < c_b)
                                                                    if(p[pixel[13]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                    if(p[pixel[10]] > cb)
                                                                        if(p[pixel[11]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if(p[pixel[12]] < c_b)
                                        if(p[pixel[7]] < c_b)
                                            if(p[pixel[8]] < c_b)
                                                if(p[pixel[9]] < c_b)
                                                    if(p[pixel[10]] < c_b)
                                                        if(p[pixel[11]] < c_b)
                                                            if(p[pixel[13]] < c_b)
                                                                if(p[pixel[14]] < c_b)
                                                                    if(p[pixel[6]] < c_b)
                                                                    {}
                                                                    else
                                                                        if(p[pixel[15]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else if(p[pixel[4]] < c_b)
                                if(p[pixel[13]] > cb)
                                    if(p[pixel[11]] > cb)
                                        if(p[pixel[12]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                    if(p[pixel[10]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                    if(p[pixel[10]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            continue;
                                    else if(p[pixel[11]] < c_b)
                                        if(p[pixel[5]] < c_b)
                                            if(p[pixel[6]] < c_b)
                                                if(p[pixel[7]] < c_b)
                                                    if(p[pixel[8]] < c_b)
                                                        if(p[pixel[9]] < c_b)
                                                            if(p[pixel[10]] < c_b)
                                                                if(p[pixel[12]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if(p[pixel[13]] < c_b)
                                    if(p[pixel[7]] < c_b)
                                        if(p[pixel[8]] < c_b)
                                            if(p[pixel[9]] < c_b)
                                                if(p[pixel[10]] < c_b)
                                                    if(p[pixel[11]] < c_b)
                                                        if(p[pixel[12]] < c_b)
                                                            if(p[pixel[6]] < c_b)
                                                                if(p[pixel[5]] < c_b)
                                                                {}
                                                                else
                                                                    if(p[pixel[14]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                            else
                                                                if(p[pixel[14]] < c_b)
                                                                    if(p[pixel[15]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    if(p[pixel[5]] < c_b)
                                        if(p[pixel[6]] < c_b)
                                            if(p[pixel[7]] < c_b)
                                                if(p[pixel[8]] < c_b)
                                                    if(p[pixel[9]] < c_b)
                                                        if(p[pixel[10]] < c_b)
                                                            if(p[pixel[11]] < c_b)
                                                                if(p[pixel[12]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                    if(p[pixel[10]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                    if(p[pixel[10]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if(p[pixel[11]] < c_b)
                                    if(p[pixel[7]] < c_b)
                                        if(p[pixel[8]] < c_b)
                                            if(p[pixel[9]] < c_b)
                                                if(p[pixel[10]] < c_b)
                                                    if(p[pixel[12]] < c_b)
                                                        if(p[pixel[13]] < c_b)
                                                            if(p[pixel[6]] < c_b)
                                                                if(p[pixel[5]] < c_b)
                                                                {}
                                                                else
                                                                    if(p[pixel[14]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                            else
                                                                if(p[pixel[14]] < c_b)
                                                                    if(p[pixel[15]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                        else if(p[pixel[3]] < c_b)
                            if(p[pixel[10]] > cb)
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if(p[pixel[10]] < c_b)
                                if(p[pixel[7]] < c_b)
                                    if(p[pixel[8]] < c_b)
                                        if(p[pixel[9]] < c_b)
                                            if(p[pixel[11]] < c_b)
                                                if(p[pixel[6]] < c_b)
                                                    if(p[pixel[5]] < c_b)
                                                        if(p[pixel[4]] < c_b)
                                                        {}
                                                        else
                                                            if(p[pixel[12]] < c_b)
                                                                if(p[pixel[13]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                    else
                                                        if(p[pixel[12]] < c_b)
                                                            if(p[pixel[13]] < c_b)
                                                                if(p[pixel[14]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[12]] < c_b)
                                                        if(p[pixel[13]] < c_b)
                                                            if(p[pixel[14]] < c_b)
                                                                if(p[pixel[15]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            if(p[pixel[10]] > cb)
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                                if(p[pixel[9]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if(p[pixel[10]] < c_b)
                                if(p[pixel[7]] < c_b)
                                    if(p[pixel[8]] < c_b)
                                        if(p[pixel[9]] < c_b)
                                            if(p[pixel[11]] < c_b)
                                                if(p[pixel[12]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[5]] < c_b)
                                                            if(p[pixel[4]] < c_b)
                                                            {}
                                                            else
                                                                if(p[pixel[13]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                        else
                                                            if(p[pixel[13]] < c_b)
                                                                if(p[pixel[14]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                    else
                                                        if(p[pixel[13]] < c_b)
                                                            if(p[pixel[14]] < c_b)
                                                                if(p[pixel[15]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                    else if(p[pixel[2]] < c_b)
                        if(p[pixel[9]] > cb)
                            if(p[pixel[10]] > cb)
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] > cb)
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    continue;
                            else
                                continue;
                        else if(p[pixel[9]] < c_b)
                            if(p[pixel[7]] < c_b)
                                if(p[pixel[8]] < c_b)
                                    if(p[pixel[10]] < c_b)
                                        if(p[pixel[6]] < c_b)
                                            if(p[pixel[5]] < c_b)
                                                if(p[pixel[4]] < c_b)
                                                    if(p[pixel[3]] < c_b)
                                                    {}
                                                    else
                                                        if(p[pixel[11]] < c_b)
                                                            if(p[pixel[12]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[11]] < c_b)
                                                        if(p[pixel[12]] < c_b)
                                                            if(p[pixel[13]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[11]] < c_b)
                                                    if(p[pixel[12]] < c_b)
                                                        if(p[pixel[13]] < c_b)
                                                            if(p[pixel[14]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[11]] < c_b)
                                                if(p[pixel[12]] < c_b)
                                                    if(p[pixel[13]] < c_b)
                                                        if(p[pixel[14]] < c_b)
                                                            if(p[pixel[15]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        if(p[pixel[9]] > cb)
                            if(p[pixel[10]] > cb)
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] > cb)
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                            if(p[pixel[8]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    continue;
                            else
                                continue;
                        else if(p[pixel[9]] < c_b)
                            if(p[pixel[7]] < c_b)
                                if(p[pixel[8]] < c_b)
                                    if(p[pixel[10]] < c_b)
                                        if(p[pixel[11]] < c_b)
                                            if(p[pixel[6]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[4]] < c_b)
                                                        if(p[pixel[3]] < c_b)
                                                        {}
                                                        else
                                                            if(p[pixel[12]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                    else
                                                        if(p[pixel[12]] < c_b)
                                                            if(p[pixel[13]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[12]] < c_b)
                                                        if(p[pixel[13]] < c_b)
                                                            if(p[pixel[14]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[12]] < c_b)
                                                    if(p[pixel[13]] < c_b)
                                                        if(p[pixel[14]] < c_b)
                                                            if(p[pixel[15]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                else if(p[pixel[1]] < c_b)
                    if(p[pixel[8]] > cb)
                        if(p[pixel[9]] > cb)
                            if(p[pixel[10]] > cb)
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] > cb)
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[2]] > cb)
                                        if(p[pixel[3]] > cb)
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                continue;
                        else
                            continue;
                    else if(p[pixel[8]] < c_b)
                        if(p[pixel[7]] < c_b)
                            if(p[pixel[9]] < c_b)
                                if(p[pixel[6]] < c_b)
                                    if(p[pixel[5]] < c_b)
                                        if(p[pixel[4]] < c_b)
                                            if(p[pixel[3]] < c_b)
                                                if(p[pixel[2]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[10]] < c_b)
                                                        if(p[pixel[11]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[10]] < c_b)
                                                    if(p[pixel[11]] < c_b)
                                                        if(p[pixel[12]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[10]] < c_b)
                                                if(p[pixel[11]] < c_b)
                                                    if(p[pixel[12]] < c_b)
                                                        if(p[pixel[13]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[10]] < c_b)
                                            if(p[pixel[11]] < c_b)
                                                if(p[pixel[12]] < c_b)
                                                    if(p[pixel[13]] < c_b)
                                                        if(p[pixel[14]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[10]] < c_b)
                                        if(p[pixel[11]] < c_b)
                                            if(p[pixel[12]] < c_b)
                                                if(p[pixel[13]] < c_b)
                                                    if(p[pixel[14]] < c_b)
                                                        if(p[pixel[15]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else
                    if(p[pixel[8]] > cb)
                        if(p[pixel[9]] > cb)
                            if(p[pixel[10]] > cb)
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[15]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] > cb)
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[2]] > cb)
                                        if(p[pixel[3]] > cb)
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[7]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                continue;
                        else
                            continue;
                    else if(p[pixel[8]] < c_b)
                        if(p[pixel[7]] < c_b)
                            if(p[pixel[9]] < c_b)
                                if(p[pixel[10]] < c_b)
                                    if(p[pixel[6]] < c_b)
                                        if(p[pixel[5]] < c_b)
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[3]] < c_b)
                                                    if(p[pixel[2]] < c_b)
                                                    {}
                                                    else
                                                        if(p[pixel[11]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[11]] < c_b)
                                                        if(p[pixel[12]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[11]] < c_b)
                                                    if(p[pixel[12]] < c_b)
                                                        if(p[pixel[13]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[11]] < c_b)
                                                if(p[pixel[12]] < c_b)
                                                    if(p[pixel[13]] < c_b)
                                                        if(p[pixel[14]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[11]] < c_b)
                                            if(p[pixel[12]] < c_b)
                                                if(p[pixel[13]] < c_b)
                                                    if(p[pixel[14]] < c_b)
                                                        if(p[pixel[15]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
            else if(p[pixel[0]] < c_b)
                if(p[pixel[1]] > cb)
                    if(p[pixel[8]] > cb)
                        if(p[pixel[7]] > cb)
                            if(p[pixel[9]] > cb)
                                if(p[pixel[6]] > cb)
                                    if(p[pixel[5]] > cb)
                                        if(p[pixel[4]] > cb)
                                            if(p[pixel[3]] > cb)
                                                if(p[pixel[2]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[10]] > cb)
                                                        if(p[pixel[11]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[10]] > cb)
                                                    if(p[pixel[11]] > cb)
                                                        if(p[pixel[12]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[10]] > cb)
                                                if(p[pixel[11]] > cb)
                                                    if(p[pixel[12]] > cb)
                                                        if(p[pixel[13]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[10]] > cb)
                                            if(p[pixel[11]] > cb)
                                                if(p[pixel[12]] > cb)
                                                    if(p[pixel[13]] > cb)
                                                        if(p[pixel[14]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[10]] > cb)
                                        if(p[pixel[11]] > cb)
                                            if(p[pixel[12]] > cb)
                                                if(p[pixel[13]] > cb)
                                                    if(p[pixel[14]] > cb)
                                                        if(p[pixel[15]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                continue;
                        else
                            continue;
                    else if(p[pixel[8]] < c_b)
                        if(p[pixel[9]] < c_b)
                            if(p[pixel[10]] < c_b)
                                if(p[pixel[11]] < c_b)
                                    if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] < c_b)
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[2]] < c_b)
                                        if(p[pixel[3]] < c_b)
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
                else if(p[pixel[1]] < c_b)
                    if(p[pixel[2]] > cb)
                        if(p[pixel[9]] > cb)
                            if(p[pixel[7]] > cb)
                                if(p[pixel[8]] > cb)
                                    if(p[pixel[10]] > cb)
                                        if(p[pixel[6]] > cb)
                                            if(p[pixel[5]] > cb)
                                                if(p[pixel[4]] > cb)
                                                    if(p[pixel[3]] > cb)
                                                    {}
                                                    else
                                                        if(p[pixel[11]] > cb)
                                                            if(p[pixel[12]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[11]] > cb)
                                                        if(p[pixel[12]] > cb)
                                                            if(p[pixel[13]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[11]] > cb)
                                                    if(p[pixel[12]] > cb)
                                                        if(p[pixel[13]] > cb)
                                                            if(p[pixel[14]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[11]] > cb)
                                                if(p[pixel[12]] > cb)
                                                    if(p[pixel[13]] > cb)
                                                        if(p[pixel[14]] > cb)
                                                            if(p[pixel[15]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if(p[pixel[9]] < c_b)
                            if(p[pixel[10]] < c_b)
                                if(p[pixel[11]] < c_b)
                                    if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] < c_b)
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if(p[pixel[2]] < c_b)
                        if(p[pixel[3]] > cb)
                            if(p[pixel[10]] > cb)
                                if(p[pixel[7]] > cb)
                                    if(p[pixel[8]] > cb)
                                        if(p[pixel[9]] > cb)
                                            if(p[pixel[11]] > cb)
                                                if(p[pixel[6]] > cb)
                                                    if(p[pixel[5]] > cb)
                                                        if(p[pixel[4]] > cb)
                                                        {}
                                                        else
                                                            if(p[pixel[12]] > cb)
                                                                if(p[pixel[13]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                    else
                                                        if(p[pixel[12]] > cb)
                                                            if(p[pixel[13]] > cb)
                                                                if(p[pixel[14]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[12]] > cb)
                                                        if(p[pixel[13]] > cb)
                                                            if(p[pixel[14]] > cb)
                                                                if(p[pixel[15]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if(p[pixel[10]] < c_b)
                                if(p[pixel[11]] < c_b)
                                    if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if(p[pixel[3]] < c_b)
                            if(p[pixel[4]] > cb)
                                if(p[pixel[13]] > cb)
                                    if(p[pixel[7]] > cb)
                                        if(p[pixel[8]] > cb)
                                            if(p[pixel[9]] > cb)
                                                if(p[pixel[10]] > cb)
                                                    if(p[pixel[11]] > cb)
                                                        if(p[pixel[12]] > cb)
                                                            if(p[pixel[6]] > cb)
                                                                if(p[pixel[5]] > cb)
                                                                {}
                                                                else
                                                                    if(p[pixel[14]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                            else
                                                                if(p[pixel[14]] > cb)
                                                                    if(p[pixel[15]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if(p[pixel[13]] < c_b)
                                    if(p[pixel[11]] > cb)
                                        if(p[pixel[5]] > cb)
                                            if(p[pixel[6]] > cb)
                                                if(p[pixel[7]] > cb)
                                                    if(p[pixel[8]] > cb)
                                                        if(p[pixel[9]] > cb)
                                                            if(p[pixel[10]] > cb)
                                                                if(p[pixel[12]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if(p[pixel[11]] < c_b)
                                        if(p[pixel[12]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                    if(p[pixel[10]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                    if(p[pixel[10]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    if(p[pixel[5]] > cb)
                                        if(p[pixel[6]] > cb)
                                            if(p[pixel[7]] > cb)
                                                if(p[pixel[8]] > cb)
                                                    if(p[pixel[9]] > cb)
                                                        if(p[pixel[10]] > cb)
                                                            if(p[pixel[11]] > cb)
                                                                if(p[pixel[12]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else if(p[pixel[4]] < c_b)
                                if(p[pixel[5]] > cb)
                                    if(p[pixel[14]] > cb)
                                        if(p[pixel[7]] > cb)
                                            if(p[pixel[8]] > cb)
                                                if(p[pixel[9]] > cb)
                                                    if(p[pixel[10]] > cb)
                                                        if(p[pixel[11]] > cb)
                                                            if(p[pixel[12]] > cb)
                                                                if(p[pixel[13]] > cb)
                                                                    if(p[pixel[6]] > cb)
                                                                    {}
                                                                    else
                                                                        if(p[pixel[15]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if(p[pixel[14]] < c_b)
                                        if(p[pixel[12]] > cb)
                                            if(p[pixel[6]] > cb)
                                                if(p[pixel[7]] > cb)
                                                    if(p[pixel[8]] > cb)
                                                        if(p[pixel[9]] > cb)
                                                            if(p[pixel[10]] > cb)
                                                                if(p[pixel[11]] > cb)
                                                                    if(p[pixel[13]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if(p[pixel[12]] < c_b)
                                            if(p[pixel[13]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                    if(p[pixel[10]] < c_b)
                                                                        if(p[pixel[11]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        if(p[pixel[6]] > cb)
                                            if(p[pixel[7]] > cb)
                                                if(p[pixel[8]] > cb)
                                                    if(p[pixel[9]] > cb)
                                                        if(p[pixel[10]] > cb)
                                                            if(p[pixel[11]] > cb)
                                                                if(p[pixel[12]] > cb)
                                                                    if(p[pixel[13]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else if(p[pixel[5]] < c_b)
                                    if(p[pixel[6]] > cb)
                                        if(p[pixel[15]] < c_b)
                                            if(p[pixel[13]] > cb)
                                                if(p[pixel[7]] > cb)
                                                    if(p[pixel[8]] > cb)
                                                        if(p[pixel[9]] > cb)
                                                            if(p[pixel[10]] > cb)
                                                                if(p[pixel[11]] > cb)
                                                                    if(p[pixel[12]] > cb)
                                                                        if(p[pixel[14]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if(p[pixel[13]] < c_b)
                                                if(p[pixel[14]] < c_b)
                                                {}
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            if(p[pixel[7]] > cb)
                                                if(p[pixel[8]] > cb)
                                                    if(p[pixel[9]] > cb)
                                                        if(p[pixel[10]] > cb)
                                                            if(p[pixel[11]] > cb)
                                                                if(p[pixel[12]] > cb)
                                                                    if(p[pixel[13]] > cb)
                                                                        if(p[pixel[14]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else if(p[pixel[6]] < c_b)
                                        if(p[pixel[7]] > cb)
                                            if(p[pixel[14]] > cb)
                                                if(p[pixel[8]] > cb)
                                                    if(p[pixel[9]] > cb)
                                                        if(p[pixel[10]] > cb)
                                                            if(p[pixel[11]] > cb)
                                                                if(p[pixel[12]] > cb)
                                                                    if(p[pixel[13]] > cb)
                                                                        if(p[pixel[15]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if(p[pixel[7]] < c_b)
                                            if(p[pixel[8]] < c_b)
                                            {}
                                            else
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[13]] > cb)
                                            if(p[pixel[7]] > cb)
                                                if(p[pixel[8]] > cb)
                                                    if(p[pixel[9]] > cb)
                                                        if(p[pixel[10]] > cb)
                                                            if(p[pixel[11]] > cb)
                                                                if(p[pixel[12]] > cb)
                                                                    if(p[pixel[14]] > cb)
                                                                        if(p[pixel[15]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[12]] > cb)
                                        if(p[pixel[7]] > cb)
                                            if(p[pixel[8]] > cb)
                                                if(p[pixel[9]] > cb)
                                                    if(p[pixel[10]] > cb)
                                                        if(p[pixel[11]] > cb)
                                                            if(p[pixel[13]] > cb)
                                                                if(p[pixel[14]] > cb)
                                                                    if(p[pixel[6]] > cb)
                                                                    {}
                                                                    else
                                                                        if(p[pixel[15]] > cb)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                    if(p[pixel[10]] < c_b)
                                                                        if(p[pixel[11]] < c_b)
                                                                        {}
                                                                        else
                                                                            continue;
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                if(p[pixel[11]] > cb)
                                    if(p[pixel[7]] > cb)
                                        if(p[pixel[8]] > cb)
                                            if(p[pixel[9]] > cb)
                                                if(p[pixel[10]] > cb)
                                                    if(p[pixel[12]] > cb)
                                                        if(p[pixel[13]] > cb)
                                                            if(p[pixel[6]] > cb)
                                                                if(p[pixel[5]] > cb)
                                                                {}
                                                                else
                                                                    if(p[pixel[14]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                            else
                                                                if(p[pixel[14]] > cb)
                                                                    if(p[pixel[15]] > cb)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else if(p[pixel[11]] < c_b)
                                    if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                    if(p[pixel[10]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                    if(p[pixel[10]] < c_b)
                                                                    {}
                                                                    else
                                                                        continue;
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                        else
                            if(p[pixel[10]] > cb)
                                if(p[pixel[7]] > cb)
                                    if(p[pixel[8]] > cb)
                                        if(p[pixel[9]] > cb)
                                            if(p[pixel[11]] > cb)
                                                if(p[pixel[12]] > cb)
                                                    if(p[pixel[6]] > cb)
                                                        if(p[pixel[5]] > cb)
                                                            if(p[pixel[4]] > cb)
                                                            {}
                                                            else
                                                                if(p[pixel[13]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                        else
                                                            if(p[pixel[13]] > cb)
                                                                if(p[pixel[14]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                    else
                                                        if(p[pixel[13]] > cb)
                                                            if(p[pixel[14]] > cb)
                                                                if(p[pixel[15]] > cb)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else if(p[pixel[10]] < c_b)
                                if(p[pixel[11]] < c_b)
                                    if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                                if(p[pixel[9]] < c_b)
                                                                {}
                                                                else
                                                                    continue;
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                    else
                        if(p[pixel[9]] > cb)
                            if(p[pixel[7]] > cb)
                                if(p[pixel[8]] > cb)
                                    if(p[pixel[10]] > cb)
                                        if(p[pixel[11]] > cb)
                                            if(p[pixel[6]] > cb)
                                                if(p[pixel[5]] > cb)
                                                    if(p[pixel[4]] > cb)
                                                        if(p[pixel[3]] > cb)
                                                        {}
                                                        else
                                                            if(p[pixel[12]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                    else
                                                        if(p[pixel[12]] > cb)
                                                            if(p[pixel[13]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[12]] > cb)
                                                        if(p[pixel[13]] > cb)
                                                            if(p[pixel[14]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[12]] > cb)
                                                    if(p[pixel[13]] > cb)
                                                        if(p[pixel[14]] > cb)
                                                            if(p[pixel[15]] > cb)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                            else
                                continue;
                        else if(p[pixel[9]] < c_b)
                            if(p[pixel[10]] < c_b)
                                if(p[pixel[11]] < c_b)
                                    if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] < c_b)
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                            if(p[pixel[8]] < c_b)
                                                            {}
                                                            else
                                                                continue;
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                else
                    if(p[pixel[8]] > cb)
                        if(p[pixel[7]] > cb)
                            if(p[pixel[9]] > cb)
                                if(p[pixel[10]] > cb)
                                    if(p[pixel[6]] > cb)
                                        if(p[pixel[5]] > cb)
                                            if(p[pixel[4]] > cb)
                                                if(p[pixel[3]] > cb)
                                                    if(p[pixel[2]] > cb)
                                                    {}
                                                    else
                                                        if(p[pixel[11]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                else
                                                    if(p[pixel[11]] > cb)
                                                        if(p[pixel[12]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[11]] > cb)
                                                    if(p[pixel[12]] > cb)
                                                        if(p[pixel[13]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[11]] > cb)
                                                if(p[pixel[12]] > cb)
                                                    if(p[pixel[13]] > cb)
                                                        if(p[pixel[14]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[11]] > cb)
                                            if(p[pixel[12]] > cb)
                                                if(p[pixel[13]] > cb)
                                                    if(p[pixel[14]] > cb)
                                                        if(p[pixel[15]] > cb)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    continue;
                            else
                                continue;
                        else
                            continue;
                    else if(p[pixel[8]] < c_b)
                        if(p[pixel[9]] < c_b)
                            if(p[pixel[10]] < c_b)
                                if(p[pixel[11]] < c_b)
                                    if(p[pixel[12]] < c_b)
                                        if(p[pixel[13]] < c_b)
                                            if(p[pixel[14]] < c_b)
                                                if(p[pixel[15]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[3]] < c_b)
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[2]] < c_b)
                                        if(p[pixel[3]] < c_b)
                                            if(p[pixel[4]] < c_b)
                                                if(p[pixel[5]] < c_b)
                                                    if(p[pixel[6]] < c_b)
                                                        if(p[pixel[7]] < c_b)
                                                        {}
                                                        else
                                                            continue;
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                continue;
                        else
                            continue;
                    else
                        continue;
            else
                if(p[pixel[7]] > cb)
                    if(p[pixel[8]] > cb)
                        if(p[pixel[9]] > cb)
                            if(p[pixel[6]] > cb)
                                if(p[pixel[5]] > cb)
                                    if(p[pixel[4]] > cb)
                                        if(p[pixel[3]] > cb)
                                            if(p[pixel[2]] > cb)
                                                if(p[pixel[1]] > cb)
                                                {}
                                                else
                                                    if(p[pixel[10]] > cb)
                                                    {}
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[10]] > cb)
                                                    if(p[pixel[11]] > cb)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[10]] > cb)
                                                if(p[pixel[11]] > cb)
                                                    if(p[pixel[12]] > cb)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[10]] > cb)
                                            if(p[pixel[11]] > cb)
                                                if(p[pixel[12]] > cb)
                                                    if(p[pixel[13]] > cb)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[10]] > cb)
                                        if(p[pixel[11]] > cb)
                                            if(p[pixel[12]] > cb)
                                                if(p[pixel[13]] > cb)
                                                    if(p[pixel[14]] > cb)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                if(p[pixel[10]] > cb)
                                    if(p[pixel[11]] > cb)
                                        if(p[pixel[12]] > cb)
                                            if(p[pixel[13]] > cb)
                                                if(p[pixel[14]] > cb)
                                                    if(p[pixel[15]] > cb)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                        else
                            continue;
                    else
                        continue;
                else if(p[pixel[7]] < c_b)
                    if(p[pixel[8]] < c_b)
                        if(p[pixel[9]] < c_b)
                            if(p[pixel[6]] < c_b)
                                if(p[pixel[5]] < c_b)
                                    if(p[pixel[4]] < c_b)
                                        if(p[pixel[3]] < c_b)
                                            if(p[pixel[2]] < c_b)
                                                if(p[pixel[1]] < c_b)
                                                {}
                                                else
                                                    if(p[pixel[10]] < c_b)
                                                    {}
                                                    else
                                                        continue;
                                            else
                                                if(p[pixel[10]] < c_b)
                                                    if(p[pixel[11]] < c_b)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                        else
                                            if(p[pixel[10]] < c_b)
                                                if(p[pixel[11]] < c_b)
                                                    if(p[pixel[12]] < c_b)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                    else
                                        if(p[pixel[10]] < c_b)
                                            if(p[pixel[11]] < c_b)
                                                if(p[pixel[12]] < c_b)
                                                    if(p[pixel[13]] < c_b)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                else
                                    if(p[pixel[10]] < c_b)
                                        if(p[pixel[11]] < c_b)
                                            if(p[pixel[12]] < c_b)
                                                if(p[pixel[13]] < c_b)
                                                    if(p[pixel[14]] < c_b)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                            else
                                if(p[pixel[10]] < c_b)
                                    if(p[pixel[11]] < c_b)
                                        if(p[pixel[12]] < c_b)
                                            if(p[pixel[13]] < c_b)
                                                if(p[pixel[14]] < c_b)
                                                    if(p[pixel[15]] < c_b)
                                                    {}
                                                    else
                                                        continue;
                                                else
                                                    continue;
                                            else
                                                continue;
                                        else
                                            continue;
                                    else
                                        continue;
                                else
                                    continue;
                        else
                            continue;
                    else
                        continue;
                else
                    continue;

			if(num_corners == rsize)
			{
                void *ptr;

				rsize *= 2;
			    ptr = (xy*)realloc(ret_corners, sizeof(xy)*rsize);
                if (!ptr) /* in case of error, return the current dataset */
                    goto out;
                ret_corners = ptr;
			}

			ret_corners[num_corners].x = x;
			ret_corners[num_corners].y = y;
			num_corners++;
        }
    }

out:
	*ret_num_corners = num_corners;
	return ret_corners;
}
