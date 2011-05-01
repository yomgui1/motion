#include "fast/fast.h"

#include "imtool/handler.h"
#include "imtool/convertor.h"
#include "imtool/error.h"
#include "imtool/image.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#ifdef __MORPHOS__
static u_quad_t getcounter(void)
{
    register u_int32_t tbu, tb, tbu2;

  loop:
    __asm volatile ("mftbu %0" : "=r" (tbu) );
    __asm volatile ("mftb  %0" : "=r" (tb)  );
    __asm volatile ("mftbu %0" : "=r" (tbu2));
    if (tbu != tbu2) goto loop;

    return (((u_quad_t) tbu) << 32) + tb;
}
#endif

int main(int argc, char **argv)
{
    int rc = 1, err, threshold;
    IMT_Image *image=NULL, *gray=NULL;

    if (argc < 3)
    {
        fprintf(stderr, "Bad parameter: not enough arguments\nUSAGE: %s <source_image> <threshold> [result_file]\n", argv[0]);
        return -1;
    }

    threshold = strtoul(argv[2], NULL, 10);
    if (threshold > 255)
    {
        fprintf(stderr, "Bad parameter: threshold is bigger than 255 (%u)\n", threshold);
        return -1;
    }

	IMT_init();

    err = IMT_load_file(argv[1], &image, NULL);
    if (NULL != image)
    {
        err = IMT_Grayscale(image, &gray);
        if (!err)
        {
            int num_corners, num_nonmax_corners;
            xy *corners, *nonmax_corners;
			u_quad_t t0, t1;
		
            printf("Trying simple fast9 detection... ");
            num_corners = -1;
            t0 = getcounter();
            corners = fast9_detect(gray->data, gray->width, gray->height, gray->stride, threshold, &num_corners);
            t1 = getcounter() - t0;

            if (NULL == corners)
                printf("[FAILED] (NULL result)\n");
            else if (num_corners < 0)
                printf("[FAILED] (num_corners not set)\n");
            else if (num_corners == 0)
                printf("[WARNING] (no corners found)\n");
            else
                printf("[OK] (%u corners in %gs)\n", num_corners, t1*4/166005898.);
            
            printf("Trying fast9 detection + nonmax removal... ");
            num_nonmax_corners = -1;
            nonmax_corners = fast9_detect_nonmax(gray->data, gray->width, gray->height, gray->stride, threshold, &num_nonmax_corners);

            if (NULL == nonmax_corners)
                printf("[FAILED] (NULL result)\n");
            else if (num_nonmax_corners < 0)
                printf("[FAILED] (num_corners not set)\n");
            else if (num_nonmax_corners == 0)
                printf("[WARNING] (no corners found)\n");
            else
                printf("[OK] (%u corners)\n", num_nonmax_corners);

            if (num_corners >= 2)
            {
                int num_limited_corners = num_corners / 2;
                xy* limited_corners;

                printf("Trying fast9 limited (%u features only) simple detection ... ", num_limited_corners);
                limited_corners = fast9_detect_limited(gray->data, gray->width, gray->height, gray->stride, threshold, &num_limited_corners, 0);

                if (NULL == limited_corners)
                    printf("[FAILED] (NULL result)\n");
                else if (num_limited_corners == 0)
                    printf("[WARNING] (no corners found)\n");
                else if (num_limited_corners > num_corners / 2)
                    printf("[FAILED] (corners number incorrect: waited %d, get %d)\n", num_corners / 2, num_limited_corners);
                else
                {
                    int i, j, unknowns=0;

                    /* checking if the limited corners set is inside the complet set */
                    for (i=0; i < num_limited_corners; i++)
                    {
						int found = 0;
                        xy *corner = &limited_corners[i];

                        for (j=0; j < num_corners; j++)
                        {
                            if ((corners[j].x == corner->x) && (corners[j].y == corner->y))
                            {
								found = 1;
								break;
							}
                        }
                        
						if (!found)
							unknowns++;
                    }

                    if (unknowns > 0)
                    {
                        printf("[WARNING] (%u corners found but got %u corners not inside the full set)\n", num_limited_corners, unknowns);
                    }
                    else
                        printf("[OK] (%u corners)\n", num_limited_corners);
                }

                free(limited_corners);
            }
					
			if (argc >= 4)
			{
				int i;
				FILE *outfile;
				
				outfile = fopen(argv[3], "w");
				fprintf(outfile, "%u %u %u\n", gray->width, gray->height, num_nonmax_corners);
				for (i=0; i < num_nonmax_corners; i++)
					fprintf(outfile, "%u %u\n", nonmax_corners[i].x, nonmax_corners[i].y);
				fclose(outfile);
			}

            free(nonmax_corners);
            free(corners);
            
            IMT_FreeImage(gray);
        }
        else
            fprintf(stderr, "Converting input image to greyscale failed: %s\n", IMT_get_error_string(err));
		
        IMT_FreeImage(image);
    }
    else
        fprintf(stderr, "Loading '%s' failed: %s\n", argv[1], IMT_get_error_string(err));

    return rc;
}
