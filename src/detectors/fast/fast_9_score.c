/* Original author: Edward Rosten
 * Modified by: Guillaume Roguez
 */

/* This is mechanically generated code */

#include "fast.h"
#include "fast_utils.h"

#include <stdlib.h> /* malloc/free */

int fast9_corner_score(const byte* p, const int pixel[], int bstart)
{    
    int bmin = bstart;
    int bmax = 255;
    int b = (bmax + bmin)/2;
    
    /* Compute the score using binary search */
	for(;;)
    {
		int cb = *p + b;
		int c_b= *p - b;

        if( p[pixel[0]] > cb)
            if( p[pixel[1]] > cb)
                if( p[pixel[2]] > cb)
                    if( p[pixel[3]] > cb)
                        if( p[pixel[4]] > cb)
                            if( p[pixel[5]] > cb)
                                if( p[pixel[6]] > cb)
                                    if( p[pixel[7]] > cb)
                                        if( p[pixel[8]] > cb)
                                            goto is_a_corner;
                                        else
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else if( p[pixel[7]] < c_b)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if( p[pixel[14]] < c_b)
                                            if( p[pixel[8]] < c_b)
                                                if( p[pixel[9]] < c_b)
                                                    if( p[pixel[10]] < c_b)
                                                        if( p[pixel[11]] < c_b)
                                                            if( p[pixel[12]] < c_b)
                                                                if( p[pixel[13]] < c_b)
                                                                    if( p[pixel[15]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else if( p[pixel[6]] < c_b)
                                    if( p[pixel[15]] > cb)
                                        if( p[pixel[13]] > cb)
                                            if( p[pixel[14]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if( p[pixel[13]] < c_b)
                                            if( p[pixel[7]] < c_b)
                                                if( p[pixel[8]] < c_b)
                                                    if( p[pixel[9]] < c_b)
                                                        if( p[pixel[10]] < c_b)
                                                            if( p[pixel[11]] < c_b)
                                                                if( p[pixel[12]] < c_b)
                                                                    if( p[pixel[14]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        if( p[pixel[7]] < c_b)
                                            if( p[pixel[8]] < c_b)
                                                if( p[pixel[9]] < c_b)
                                                    if( p[pixel[10]] < c_b)
                                                        if( p[pixel[11]] < c_b)
                                                            if( p[pixel[12]] < c_b)
                                                                if( p[pixel[13]] < c_b)
                                                                    if( p[pixel[14]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if( p[pixel[13]] < c_b)
                                        if( p[pixel[7]] < c_b)
                                            if( p[pixel[8]] < c_b)
                                                if( p[pixel[9]] < c_b)
                                                    if( p[pixel[10]] < c_b)
                                                        if( p[pixel[11]] < c_b)
                                                            if( p[pixel[12]] < c_b)
                                                                if( p[pixel[14]] < c_b)
                                                                    if( p[pixel[15]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else if( p[pixel[5]] < c_b)
                                if( p[pixel[14]] > cb)
                                    if( p[pixel[12]] > cb)
                                        if( p[pixel[13]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                if( p[pixel[10]] > cb)
                                                                    if( p[pixel[11]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if( p[pixel[12]] < c_b)
                                        if( p[pixel[6]] < c_b)
                                            if( p[pixel[7]] < c_b)
                                                if( p[pixel[8]] < c_b)
                                                    if( p[pixel[9]] < c_b)
                                                        if( p[pixel[10]] < c_b)
                                                            if( p[pixel[11]] < c_b)
                                                                if( p[pixel[13]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if( p[pixel[14]] < c_b)
                                    if( p[pixel[7]] < c_b)
                                        if( p[pixel[8]] < c_b)
                                            if( p[pixel[9]] < c_b)
                                                if( p[pixel[10]] < c_b)
                                                    if( p[pixel[11]] < c_b)
                                                        if( p[pixel[12]] < c_b)
                                                            if( p[pixel[13]] < c_b)
                                                                if( p[pixel[6]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    if( p[pixel[15]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    if( p[pixel[6]] < c_b)
                                        if( p[pixel[7]] < c_b)
                                            if( p[pixel[8]] < c_b)
                                                if( p[pixel[9]] < c_b)
                                                    if( p[pixel[10]] < c_b)
                                                        if( p[pixel[11]] < c_b)
                                                            if( p[pixel[12]] < c_b)
                                                                if( p[pixel[13]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                if( p[pixel[10]] > cb)
                                                                    if( p[pixel[11]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if( p[pixel[12]] < c_b)
                                    if( p[pixel[7]] < c_b)
                                        if( p[pixel[8]] < c_b)
                                            if( p[pixel[9]] < c_b)
                                                if( p[pixel[10]] < c_b)
                                                    if( p[pixel[11]] < c_b)
                                                        if( p[pixel[13]] < c_b)
                                                            if( p[pixel[14]] < c_b)
                                                                if( p[pixel[6]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    if( p[pixel[15]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else if( p[pixel[4]] < c_b)
                            if( p[pixel[13]] > cb)
                                if( p[pixel[11]] > cb)
                                    if( p[pixel[12]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                if( p[pixel[10]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                if( p[pixel[10]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if( p[pixel[11]] < c_b)
                                    if( p[pixel[5]] < c_b)
                                        if( p[pixel[6]] < c_b)
                                            if( p[pixel[7]] < c_b)
                                                if( p[pixel[8]] < c_b)
                                                    if( p[pixel[9]] < c_b)
                                                        if( p[pixel[10]] < c_b)
                                                            if( p[pixel[12]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if( p[pixel[13]] < c_b)
                                if( p[pixel[7]] < c_b)
                                    if( p[pixel[8]] < c_b)
                                        if( p[pixel[9]] < c_b)
                                            if( p[pixel[10]] < c_b)
                                                if( p[pixel[11]] < c_b)
                                                    if( p[pixel[12]] < c_b)
                                                        if( p[pixel[6]] < c_b)
                                                            if( p[pixel[5]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                if( p[pixel[14]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                        else
                                                            if( p[pixel[14]] < c_b)
                                                                if( p[pixel[15]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                if( p[pixel[5]] < c_b)
                                    if( p[pixel[6]] < c_b)
                                        if( p[pixel[7]] < c_b)
                                            if( p[pixel[8]] < c_b)
                                                if( p[pixel[9]] < c_b)
                                                    if( p[pixel[10]] < c_b)
                                                        if( p[pixel[11]] < c_b)
                                                            if( p[pixel[12]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            if( p[pixel[11]] > cb)
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                if( p[pixel[10]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                if( p[pixel[10]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if( p[pixel[11]] < c_b)
                                if( p[pixel[7]] < c_b)
                                    if( p[pixel[8]] < c_b)
                                        if( p[pixel[9]] < c_b)
                                            if( p[pixel[10]] < c_b)
                                                if( p[pixel[12]] < c_b)
                                                    if( p[pixel[13]] < c_b)
                                                        if( p[pixel[6]] < c_b)
                                                            if( p[pixel[5]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                if( p[pixel[14]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                        else
                                                            if( p[pixel[14]] < c_b)
                                                                if( p[pixel[15]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                    else if( p[pixel[3]] < c_b)
                        if( p[pixel[10]] > cb)
                            if( p[pixel[11]] > cb)
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if( p[pixel[10]] < c_b)
                            if( p[pixel[7]] < c_b)
                                if( p[pixel[8]] < c_b)
                                    if( p[pixel[9]] < c_b)
                                        if( p[pixel[11]] < c_b)
                                            if( p[pixel[6]] < c_b)
                                                if( p[pixel[5]] < c_b)
                                                    if( p[pixel[4]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        if( p[pixel[12]] < c_b)
                                                            if( p[pixel[13]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                else
                                                    if( p[pixel[12]] < c_b)
                                                        if( p[pixel[13]] < c_b)
                                                            if( p[pixel[14]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[12]] < c_b)
                                                    if( p[pixel[13]] < c_b)
                                                        if( p[pixel[14]] < c_b)
                                                            if( p[pixel[15]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        if( p[pixel[10]] > cb)
                            if( p[pixel[11]] > cb)
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            if( p[pixel[9]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if( p[pixel[10]] < c_b)
                            if( p[pixel[7]] < c_b)
                                if( p[pixel[8]] < c_b)
                                    if( p[pixel[9]] < c_b)
                                        if( p[pixel[11]] < c_b)
                                            if( p[pixel[12]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[5]] < c_b)
                                                        if( p[pixel[4]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            if( p[pixel[13]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                    else
                                                        if( p[pixel[13]] < c_b)
                                                            if( p[pixel[14]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                else
                                                    if( p[pixel[13]] < c_b)
                                                        if( p[pixel[14]] < c_b)
                                                            if( p[pixel[15]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                else if( p[pixel[2]] < c_b)
                    if( p[pixel[9]] > cb)
                        if( p[pixel[10]] > cb)
                            if( p[pixel[11]] > cb)
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] > cb)
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if( p[pixel[9]] < c_b)
                        if( p[pixel[7]] < c_b)
                            if( p[pixel[8]] < c_b)
                                if( p[pixel[10]] < c_b)
                                    if( p[pixel[6]] < c_b)
                                        if( p[pixel[5]] < c_b)
                                            if( p[pixel[4]] < c_b)
                                                if( p[pixel[3]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    if( p[pixel[11]] < c_b)
                                                        if( p[pixel[12]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[11]] < c_b)
                                                    if( p[pixel[12]] < c_b)
                                                        if( p[pixel[13]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[11]] < c_b)
                                                if( p[pixel[12]] < c_b)
                                                    if( p[pixel[13]] < c_b)
                                                        if( p[pixel[14]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[11]] < c_b)
                                            if( p[pixel[12]] < c_b)
                                                if( p[pixel[13]] < c_b)
                                                    if( p[pixel[14]] < c_b)
                                                        if( p[pixel[15]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    if( p[pixel[9]] > cb)
                        if( p[pixel[10]] > cb)
                            if( p[pixel[11]] > cb)
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] > cb)
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        if( p[pixel[8]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if( p[pixel[9]] < c_b)
                        if( p[pixel[7]] < c_b)
                            if( p[pixel[8]] < c_b)
                                if( p[pixel[10]] < c_b)
                                    if( p[pixel[11]] < c_b)
                                        if( p[pixel[6]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[4]] < c_b)
                                                    if( p[pixel[3]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        if( p[pixel[12]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                else
                                                    if( p[pixel[12]] < c_b)
                                                        if( p[pixel[13]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[12]] < c_b)
                                                    if( p[pixel[13]] < c_b)
                                                        if( p[pixel[14]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[12]] < c_b)
                                                if( p[pixel[13]] < c_b)
                                                    if( p[pixel[14]] < c_b)
                                                        if( p[pixel[15]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
            else if( p[pixel[1]] < c_b)
                if( p[pixel[8]] > cb)
                    if( p[pixel[9]] > cb)
                        if( p[pixel[10]] > cb)
                            if( p[pixel[11]] > cb)
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] > cb)
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[2]] > cb)
                                    if( p[pixel[3]] > cb)
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if( p[pixel[8]] < c_b)
                    if( p[pixel[7]] < c_b)
                        if( p[pixel[9]] < c_b)
                            if( p[pixel[6]] < c_b)
                                if( p[pixel[5]] < c_b)
                                    if( p[pixel[4]] < c_b)
                                        if( p[pixel[3]] < c_b)
                                            if( p[pixel[2]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[10]] < c_b)
                                                    if( p[pixel[11]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[10]] < c_b)
                                                if( p[pixel[11]] < c_b)
                                                    if( p[pixel[12]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[10]] < c_b)
                                            if( p[pixel[11]] < c_b)
                                                if( p[pixel[12]] < c_b)
                                                    if( p[pixel[13]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[10]] < c_b)
                                        if( p[pixel[11]] < c_b)
                                            if( p[pixel[12]] < c_b)
                                                if( p[pixel[13]] < c_b)
                                                    if( p[pixel[14]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[10]] < c_b)
                                    if( p[pixel[11]] < c_b)
                                        if( p[pixel[12]] < c_b)
                                            if( p[pixel[13]] < c_b)
                                                if( p[pixel[14]] < c_b)
                                                    if( p[pixel[15]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else
                if( p[pixel[8]] > cb)
                    if( p[pixel[9]] > cb)
                        if( p[pixel[10]] > cb)
                            if( p[pixel[11]] > cb)
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[15]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] > cb)
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[2]] > cb)
                                    if( p[pixel[3]] > cb)
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[7]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if( p[pixel[8]] < c_b)
                    if( p[pixel[7]] < c_b)
                        if( p[pixel[9]] < c_b)
                            if( p[pixel[10]] < c_b)
                                if( p[pixel[6]] < c_b)
                                    if( p[pixel[5]] < c_b)
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[3]] < c_b)
                                                if( p[pixel[2]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    if( p[pixel[11]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[11]] < c_b)
                                                    if( p[pixel[12]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[11]] < c_b)
                                                if( p[pixel[12]] < c_b)
                                                    if( p[pixel[13]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[11]] < c_b)
                                            if( p[pixel[12]] < c_b)
                                                if( p[pixel[13]] < c_b)
                                                    if( p[pixel[14]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[11]] < c_b)
                                        if( p[pixel[12]] < c_b)
                                            if( p[pixel[13]] < c_b)
                                                if( p[pixel[14]] < c_b)
                                                    if( p[pixel[15]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
        else if( p[pixel[0]] < c_b)
            if( p[pixel[1]] > cb)
                if( p[pixel[8]] > cb)
                    if( p[pixel[7]] > cb)
                        if( p[pixel[9]] > cb)
                            if( p[pixel[6]] > cb)
                                if( p[pixel[5]] > cb)
                                    if( p[pixel[4]] > cb)
                                        if( p[pixel[3]] > cb)
                                            if( p[pixel[2]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[10]] > cb)
                                                    if( p[pixel[11]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[10]] > cb)
                                                if( p[pixel[11]] > cb)
                                                    if( p[pixel[12]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[10]] > cb)
                                            if( p[pixel[11]] > cb)
                                                if( p[pixel[12]] > cb)
                                                    if( p[pixel[13]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[10]] > cb)
                                        if( p[pixel[11]] > cb)
                                            if( p[pixel[12]] > cb)
                                                if( p[pixel[13]] > cb)
                                                    if( p[pixel[14]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[10]] > cb)
                                    if( p[pixel[11]] > cb)
                                        if( p[pixel[12]] > cb)
                                            if( p[pixel[13]] > cb)
                                                if( p[pixel[14]] > cb)
                                                    if( p[pixel[15]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if( p[pixel[8]] < c_b)
                    if( p[pixel[9]] < c_b)
                        if( p[pixel[10]] < c_b)
                            if( p[pixel[11]] < c_b)
                                if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] < c_b)
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[2]] < c_b)
                                    if( p[pixel[3]] < c_b)
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else if( p[pixel[1]] < c_b)
                if( p[pixel[2]] > cb)
                    if( p[pixel[9]] > cb)
                        if( p[pixel[7]] > cb)
                            if( p[pixel[8]] > cb)
                                if( p[pixel[10]] > cb)
                                    if( p[pixel[6]] > cb)
                                        if( p[pixel[5]] > cb)
                                            if( p[pixel[4]] > cb)
                                                if( p[pixel[3]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    if( p[pixel[11]] > cb)
                                                        if( p[pixel[12]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[11]] > cb)
                                                    if( p[pixel[12]] > cb)
                                                        if( p[pixel[13]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[11]] > cb)
                                                if( p[pixel[12]] > cb)
                                                    if( p[pixel[13]] > cb)
                                                        if( p[pixel[14]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[11]] > cb)
                                            if( p[pixel[12]] > cb)
                                                if( p[pixel[13]] > cb)
                                                    if( p[pixel[14]] > cb)
                                                        if( p[pixel[15]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if( p[pixel[9]] < c_b)
                        if( p[pixel[10]] < c_b)
                            if( p[pixel[11]] < c_b)
                                if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] < c_b)
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if( p[pixel[2]] < c_b)
                    if( p[pixel[3]] > cb)
                        if( p[pixel[10]] > cb)
                            if( p[pixel[7]] > cb)
                                if( p[pixel[8]] > cb)
                                    if( p[pixel[9]] > cb)
                                        if( p[pixel[11]] > cb)
                                            if( p[pixel[6]] > cb)
                                                if( p[pixel[5]] > cb)
                                                    if( p[pixel[4]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        if( p[pixel[12]] > cb)
                                                            if( p[pixel[13]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                else
                                                    if( p[pixel[12]] > cb)
                                                        if( p[pixel[13]] > cb)
                                                            if( p[pixel[14]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[12]] > cb)
                                                    if( p[pixel[13]] > cb)
                                                        if( p[pixel[14]] > cb)
                                                            if( p[pixel[15]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if( p[pixel[10]] < c_b)
                            if( p[pixel[11]] < c_b)
                                if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if( p[pixel[3]] < c_b)
                        if( p[pixel[4]] > cb)
                            if( p[pixel[13]] > cb)
                                if( p[pixel[7]] > cb)
                                    if( p[pixel[8]] > cb)
                                        if( p[pixel[9]] > cb)
                                            if( p[pixel[10]] > cb)
                                                if( p[pixel[11]] > cb)
                                                    if( p[pixel[12]] > cb)
                                                        if( p[pixel[6]] > cb)
                                                            if( p[pixel[5]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                if( p[pixel[14]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                        else
                                                            if( p[pixel[14]] > cb)
                                                                if( p[pixel[15]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if( p[pixel[13]] < c_b)
                                if( p[pixel[11]] > cb)
                                    if( p[pixel[5]] > cb)
                                        if( p[pixel[6]] > cb)
                                            if( p[pixel[7]] > cb)
                                                if( p[pixel[8]] > cb)
                                                    if( p[pixel[9]] > cb)
                                                        if( p[pixel[10]] > cb)
                                                            if( p[pixel[12]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if( p[pixel[11]] < c_b)
                                    if( p[pixel[12]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                if( p[pixel[10]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                if( p[pixel[10]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                if( p[pixel[5]] > cb)
                                    if( p[pixel[6]] > cb)
                                        if( p[pixel[7]] > cb)
                                            if( p[pixel[8]] > cb)
                                                if( p[pixel[9]] > cb)
                                                    if( p[pixel[10]] > cb)
                                                        if( p[pixel[11]] > cb)
                                                            if( p[pixel[12]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else if( p[pixel[4]] < c_b)
                            if( p[pixel[5]] > cb)
                                if( p[pixel[14]] > cb)
                                    if( p[pixel[7]] > cb)
                                        if( p[pixel[8]] > cb)
                                            if( p[pixel[9]] > cb)
                                                if( p[pixel[10]] > cb)
                                                    if( p[pixel[11]] > cb)
                                                        if( p[pixel[12]] > cb)
                                                            if( p[pixel[13]] > cb)
                                                                if( p[pixel[6]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    if( p[pixel[15]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if( p[pixel[14]] < c_b)
                                    if( p[pixel[12]] > cb)
                                        if( p[pixel[6]] > cb)
                                            if( p[pixel[7]] > cb)
                                                if( p[pixel[8]] > cb)
                                                    if( p[pixel[9]] > cb)
                                                        if( p[pixel[10]] > cb)
                                                            if( p[pixel[11]] > cb)
                                                                if( p[pixel[13]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if( p[pixel[12]] < c_b)
                                        if( p[pixel[13]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                if( p[pixel[10]] < c_b)
                                                                    if( p[pixel[11]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    if( p[pixel[6]] > cb)
                                        if( p[pixel[7]] > cb)
                                            if( p[pixel[8]] > cb)
                                                if( p[pixel[9]] > cb)
                                                    if( p[pixel[10]] > cb)
                                                        if( p[pixel[11]] > cb)
                                                            if( p[pixel[12]] > cb)
                                                                if( p[pixel[13]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else if( p[pixel[5]] < c_b)
                                if( p[pixel[6]] > cb)
                                    if( p[pixel[15]] < c_b)
                                        if( p[pixel[13]] > cb)
                                            if( p[pixel[7]] > cb)
                                                if( p[pixel[8]] > cb)
                                                    if( p[pixel[9]] > cb)
                                                        if( p[pixel[10]] > cb)
                                                            if( p[pixel[11]] > cb)
                                                                if( p[pixel[12]] > cb)
                                                                    if( p[pixel[14]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if( p[pixel[13]] < c_b)
                                            if( p[pixel[14]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        if( p[pixel[7]] > cb)
                                            if( p[pixel[8]] > cb)
                                                if( p[pixel[9]] > cb)
                                                    if( p[pixel[10]] > cb)
                                                        if( p[pixel[11]] > cb)
                                                            if( p[pixel[12]] > cb)
                                                                if( p[pixel[13]] > cb)
                                                                    if( p[pixel[14]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else if( p[pixel[6]] < c_b)
                                    if( p[pixel[7]] > cb)
                                        if( p[pixel[14]] > cb)
                                            if( p[pixel[8]] > cb)
                                                if( p[pixel[9]] > cb)
                                                    if( p[pixel[10]] > cb)
                                                        if( p[pixel[11]] > cb)
                                                            if( p[pixel[12]] > cb)
                                                                if( p[pixel[13]] > cb)
                                                                    if( p[pixel[15]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if( p[pixel[7]] < c_b)
                                        if( p[pixel[8]] < c_b)
                                            goto is_a_corner;
                                        else
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[13]] > cb)
                                        if( p[pixel[7]] > cb)
                                            if( p[pixel[8]] > cb)
                                                if( p[pixel[9]] > cb)
                                                    if( p[pixel[10]] > cb)
                                                        if( p[pixel[11]] > cb)
                                                            if( p[pixel[12]] > cb)
                                                                if( p[pixel[14]] > cb)
                                                                    if( p[pixel[15]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[12]] > cb)
                                    if( p[pixel[7]] > cb)
                                        if( p[pixel[8]] > cb)
                                            if( p[pixel[9]] > cb)
                                                if( p[pixel[10]] > cb)
                                                    if( p[pixel[11]] > cb)
                                                        if( p[pixel[13]] > cb)
                                                            if( p[pixel[14]] > cb)
                                                                if( p[pixel[6]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    if( p[pixel[15]] > cb)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                if( p[pixel[10]] < c_b)
                                                                    if( p[pixel[11]] < c_b)
                                                                        goto is_a_corner;
                                                                    else
                                                                        goto is_not_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            if( p[pixel[11]] > cb)
                                if( p[pixel[7]] > cb)
                                    if( p[pixel[8]] > cb)
                                        if( p[pixel[9]] > cb)
                                            if( p[pixel[10]] > cb)
                                                if( p[pixel[12]] > cb)
                                                    if( p[pixel[13]] > cb)
                                                        if( p[pixel[6]] > cb)
                                                            if( p[pixel[5]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                if( p[pixel[14]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                        else
                                                            if( p[pixel[14]] > cb)
                                                                if( p[pixel[15]] > cb)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else if( p[pixel[11]] < c_b)
                                if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                if( p[pixel[10]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                if( p[pixel[10]] < c_b)
                                                                    goto is_a_corner;
                                                                else
                                                                    goto is_not_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                    else
                        if( p[pixel[10]] > cb)
                            if( p[pixel[7]] > cb)
                                if( p[pixel[8]] > cb)
                                    if( p[pixel[9]] > cb)
                                        if( p[pixel[11]] > cb)
                                            if( p[pixel[12]] > cb)
                                                if( p[pixel[6]] > cb)
                                                    if( p[pixel[5]] > cb)
                                                        if( p[pixel[4]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            if( p[pixel[13]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                    else
                                                        if( p[pixel[13]] > cb)
                                                            if( p[pixel[14]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                else
                                                    if( p[pixel[13]] > cb)
                                                        if( p[pixel[14]] > cb)
                                                            if( p[pixel[15]] > cb)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else if( p[pixel[10]] < c_b)
                            if( p[pixel[11]] < c_b)
                                if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            if( p[pixel[9]] < c_b)
                                                                goto is_a_corner;
                                                            else
                                                                goto is_not_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                else
                    if( p[pixel[9]] > cb)
                        if( p[pixel[7]] > cb)
                            if( p[pixel[8]] > cb)
                                if( p[pixel[10]] > cb)
                                    if( p[pixel[11]] > cb)
                                        if( p[pixel[6]] > cb)
                                            if( p[pixel[5]] > cb)
                                                if( p[pixel[4]] > cb)
                                                    if( p[pixel[3]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        if( p[pixel[12]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                else
                                                    if( p[pixel[12]] > cb)
                                                        if( p[pixel[13]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[12]] > cb)
                                                    if( p[pixel[13]] > cb)
                                                        if( p[pixel[14]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[12]] > cb)
                                                if( p[pixel[13]] > cb)
                                                    if( p[pixel[14]] > cb)
                                                        if( p[pixel[15]] > cb)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else if( p[pixel[9]] < c_b)
                        if( p[pixel[10]] < c_b)
                            if( p[pixel[11]] < c_b)
                                if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] < c_b)
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        if( p[pixel[8]] < c_b)
                                                            goto is_a_corner;
                                                        else
                                                            goto is_not_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
            else
                if( p[pixel[8]] > cb)
                    if( p[pixel[7]] > cb)
                        if( p[pixel[9]] > cb)
                            if( p[pixel[10]] > cb)
                                if( p[pixel[6]] > cb)
                                    if( p[pixel[5]] > cb)
                                        if( p[pixel[4]] > cb)
                                            if( p[pixel[3]] > cb)
                                                if( p[pixel[2]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    if( p[pixel[11]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                            else
                                                if( p[pixel[11]] > cb)
                                                    if( p[pixel[12]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[11]] > cb)
                                                if( p[pixel[12]] > cb)
                                                    if( p[pixel[13]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[11]] > cb)
                                            if( p[pixel[12]] > cb)
                                                if( p[pixel[13]] > cb)
                                                    if( p[pixel[14]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[11]] > cb)
                                        if( p[pixel[12]] > cb)
                                            if( p[pixel[13]] > cb)
                                                if( p[pixel[14]] > cb)
                                                    if( p[pixel[15]] > cb)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else if( p[pixel[8]] < c_b)
                    if( p[pixel[9]] < c_b)
                        if( p[pixel[10]] < c_b)
                            if( p[pixel[11]] < c_b)
                                if( p[pixel[12]] < c_b)
                                    if( p[pixel[13]] < c_b)
                                        if( p[pixel[14]] < c_b)
                                            if( p[pixel[15]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[3]] < c_b)
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[2]] < c_b)
                                    if( p[pixel[3]] < c_b)
                                        if( p[pixel[4]] < c_b)
                                            if( p[pixel[5]] < c_b)
                                                if( p[pixel[6]] < c_b)
                                                    if( p[pixel[7]] < c_b)
                                                        goto is_a_corner;
                                                    else
                                                        goto is_not_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
        else
            if( p[pixel[7]] > cb)
                if( p[pixel[8]] > cb)
                    if( p[pixel[9]] > cb)
                        if( p[pixel[6]] > cb)
                            if( p[pixel[5]] > cb)
                                if( p[pixel[4]] > cb)
                                    if( p[pixel[3]] > cb)
                                        if( p[pixel[2]] > cb)
                                            if( p[pixel[1]] > cb)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[10]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[10]] > cb)
                                                if( p[pixel[11]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[10]] > cb)
                                            if( p[pixel[11]] > cb)
                                                if( p[pixel[12]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[10]] > cb)
                                        if( p[pixel[11]] > cb)
                                            if( p[pixel[12]] > cb)
                                                if( p[pixel[13]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[10]] > cb)
                                    if( p[pixel[11]] > cb)
                                        if( p[pixel[12]] > cb)
                                            if( p[pixel[13]] > cb)
                                                if( p[pixel[14]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            if( p[pixel[10]] > cb)
                                if( p[pixel[11]] > cb)
                                    if( p[pixel[12]] > cb)
                                        if( p[pixel[13]] > cb)
                                            if( p[pixel[14]] > cb)
                                                if( p[pixel[15]] > cb)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else if( p[pixel[7]] < c_b)
                if( p[pixel[8]] < c_b)
                    if( p[pixel[9]] < c_b)
                        if( p[pixel[6]] < c_b)
                            if( p[pixel[5]] < c_b)
                                if( p[pixel[4]] < c_b)
                                    if( p[pixel[3]] < c_b)
                                        if( p[pixel[2]] < c_b)
                                            if( p[pixel[1]] < c_b)
                                                goto is_a_corner;
                                            else
                                                if( p[pixel[10]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                        else
                                            if( p[pixel[10]] < c_b)
                                                if( p[pixel[11]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                    else
                                        if( p[pixel[10]] < c_b)
                                            if( p[pixel[11]] < c_b)
                                                if( p[pixel[12]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                else
                                    if( p[pixel[10]] < c_b)
                                        if( p[pixel[11]] < c_b)
                                            if( p[pixel[12]] < c_b)
                                                if( p[pixel[13]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                            else
                                if( p[pixel[10]] < c_b)
                                    if( p[pixel[11]] < c_b)
                                        if( p[pixel[12]] < c_b)
                                            if( p[pixel[13]] < c_b)
                                                if( p[pixel[14]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                        else
                            if( p[pixel[10]] < c_b)
                                if( p[pixel[11]] < c_b)
                                    if( p[pixel[12]] < c_b)
                                        if( p[pixel[13]] < c_b)
                                            if( p[pixel[14]] < c_b)
                                                if( p[pixel[15]] < c_b)
                                                    goto is_a_corner;
                                                else
                                                    goto is_not_a_corner;
                                            else
                                                goto is_not_a_corner;
                                        else
                                            goto is_not_a_corner;
                                    else
                                        goto is_not_a_corner;
                                else
                                    goto is_not_a_corner;
                            else
                                goto is_not_a_corner;
                    else
                        goto is_not_a_corner;
                else
                    goto is_not_a_corner;
            else
                goto is_not_a_corner;

    is_a_corner:
        bmin=b;
        goto end_if;

    is_not_a_corner:
        bmax=b;
        goto end_if;

    end_if:

		if(bmin == bmax - 1 || bmin == bmax)
			return bmin;
		b = (bmin + bmax) / 2;
    }
}


int* fast9_score(const byte* i, int stride, xy* corners, int num_corners, int b)
{	
	int *scores;
	int n;
	int pixel[16];

    if (num_corners < 1) return NULL;

    scores = (int*)malloc(sizeof(int)*num_corners);
    if (!scores) return NULL;

	make_offsets(pixel, stride);

    for(n=0; n < num_corners; n++)
        scores[n] = fast9_corner_score(i + corners[n].y*stride + corners[n].x, pixel, b);

	return scores;
}
