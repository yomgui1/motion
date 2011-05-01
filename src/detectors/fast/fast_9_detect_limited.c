/* Original author: Guillaume Roguez
 */

#include "fast.h"
#include "fast_utils.h"

#include <stdlib.h> /* qsort/malloc/free */
#include <sys/param.h> /* MIN */

struct corner_item {
    int index;
    int score;
};

static int corner_item_cmp(const void *left, const void *right)
{
    const struct corner_item *item1, *item2;

    item1 = left;
    item2 = right;

    if (item1->score > item2->score)
        return -1;
    else if (item1->score < item2->score)
        return 1;

    return 0;
}

xy* fast9_detect_limited(const byte* im,
                         int xsize, int ysize, int stride,
                         int b, int* ret_num_corners,
                         int with_nonmax)
{
	xy* corners, *final_corners=NULL;
	int num_corners, i;
    int pixel[16];
    struct corner_item* corners_mapping=NULL;
    
    /* corners detections as usual */
	corners = fast9_detect(im, xsize, ysize, stride, b, &num_corners);

    if (with_nonmax)
    {
        int nonmax_num_corners;
        int* scores;
        xy* nonmax;
            
        scores = fast9_score(im, stride, corners, num_corners, b);
        nonmax = nonmax_suppression(corners, scores, num_corners, &nonmax_num_corners);

        free(scores);
        free(corners);

        corners = nonmax;
        num_corners = nonmax_num_corners;
    }

    if (corners && (num_corners > 0))
    {
        corners_mapping = malloc(sizeof(struct corner_item) * num_corners);
        if (corners_mapping)
        {
            make_offsets(pixel, stride);

            /* For each corners compute score and create a corner/score mapping */
            for (i=0; i < num_corners; i++)
            {
                corners_mapping[i].index = i;
                corners_mapping[i].score = fast9_corner_score(im + corners[i].y*stride + corners[i].x, pixel, b);
            }

            /* Quick-sort the mapping using score as sorting value,
             * then return a new corners with only wanted N best corners.
             */
            qsort(corners_mapping, num_corners, sizeof(*corners_mapping), corner_item_cmp);
            
            num_corners = MIN(*ret_num_corners, num_corners);
            final_corners = malloc(sizeof(xy) * num_corners);
            if (final_corners)
            {
                for (i=0; i < num_corners; i++)
                {
                    final_corners[i].x = corners[corners_mapping[i].index].x;
                    final_corners[i].y = corners[corners_mapping[i].index].y;
                }
            }
            else
                *ret_num_corners = 0;
        }
        else
            *ret_num_corners = 0;
    }
    else
        *ret_num_corners = 0;
        
    free(corners);
	free(corners_mapping);
  
	return final_corners;
}
