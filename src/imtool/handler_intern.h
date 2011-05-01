#ifndef IMT_HANDLER_INTERN_H
#define IMT_HANDLER_INTERN_H

#include "handler.h"
#include "error.h"

#include <stdlib.h>

typedef struct _IMT_Handler
{
    struct _IMT_Handler *next;
    int (*recognize_from_name)(const char *filename);
    int (*load)(const char *filename, IMT_Image **p_image, void *options);
    int (*save)(const char *filename, IMT_Image *image, void *options);
    void *hdata;
} _IMT_Handler;

extern _IMT_Handler *_system_handler;

#endif /* IMT_HANDLER_INTERN_H */
