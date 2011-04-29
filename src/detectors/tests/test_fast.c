#include "fast/fast.h"

#include "imtool/handler.h"
#include "imtool/convertor.h"
#include "imtool/error.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rc = 1, err, threshold;
    IMT_Image *image=NULL, *grey=NULL;

    if (argc < 3)
    {
        fprintf(stderr, "Bad parameter: not enough arguments\nUSAGE: %s <source_image> <threshold>\n", argv[0]);
        return -1;
    }

    threshold = strtoul(argv[2]);
    if (threshold > 255)
    {
        fprintf(stderr, "Bad parameter: threshold is bigger than 255 (%u)\n", threshold);
        return -1;
    }

    err = IMT_load_file(argv[1], &image);
    if (NULL != image)
    {
        err = IMT_convertor(image, &grey, IMT_CONVERTOR_GREY8, NULL);
        if (!err)
        {
            int num_corners, num_nonmax_corners;
            struct xy* corners, *nonmax_corners;

            printf("Trying simple fast9 detection... ");
            num_corners = -1;
            corners = fast9_detect(grey->data, grey->width, grey->height, grey->stride, threshold, &num_corners);

            if (NULL == corners)
                printf("[FAILED] (NULL result)\n");
            else if (num_corners < 0)
                printf("[FAILED] (num_corners not set)\n");
            else if (num_corners == 0)
                printf("[WARNING] (no corners found)\n");
            else
                printf("[OK] (%u corners)\n", num_corners);
                
            printf("Trying fast9 detection + nonmax removal... ");
            num_nonmax_corners = -1;
            nonmax_corners = fast9_detect_nonmax(grey->data, grey->width, grey->height, grey->stride, threshold, &num_nonmax_corners);

            if (NULL == nonmax_corners)
                printf("[FAILED] (NULL result)\n");
            else if (num_nonmax_corners < 0)
                printf("[FAILED] (num_corners not set)\n");
            else if (num_nonmax_corners == 0)
                printf("[WARNING] (no corners found)\n");
            else
                printf("[OK] (%u corners)\n", num_nonmax_corners);

            free(nonmax_corners);

            if (num_corners >= 2)
            {
                int num_limited_corners = num_corners / 2;
                struct xy* limited_corners;

                printf("Trying fast9 limited (%u features only) simple detection ... ", num_corners);
                corners = fast9_detect_limited(grey->data, grey->width, grey->height, grey->stride, threshold, &num_limited_corners);

                if (NULL == corners)
                    printf("[FAILED] (NULL result)\n");
                else if (num_limited_corners != num_corners / 2)
                    printf("[FAILED] (corners number incorrect: waited %d, get %d)\n", num_corners / 2, num_limited_corners);
                else if ( == 0)
                    printf("[WARNING] (no corners found)\n");
                else
                {
                    int i, j, unknowns=0;

                    /* checking if the limited corners set is inside the complet set */
                    for (i=0; i < num_limited_corners; i++)
                    {
                        struct xy *corner = &limited_corners[i];

                        for (j=0; j < num_corners; j++)
                        {
                            if ((corners[j].x != corner->x) || (corners[j].y != corner->y))
                                unknowns++;
                        }
                    }

                    if (unknowns > 0)
                    {
                        printf("[WARNING] (%u corners found but got %u corners not inside the full set)\n", num_limited_corners, unknowns);
                    }
                    else
                        printf("[OK] (%u corners)\n", num_limited_corners);
                }

                free(num_limited_corners);
            }
           
            free(corners);
        }
        else
            fprintf(stderr, "Converting input image to greyscale failed: %s\n", IMT_get_error_string(err));

        IMT_free(image);
    }
    else
        fprintf(stderr, "Loading '%s' failed: %s\n", argv[1], IMT_get_error_string(err));

    return rc;
}
