#ifndef IMT_HANDLER_H
#define IMT_HANDLER_H

#include <imtool/image.h>

enum
{
    IMT_ERR_NOERROR=0,
    IMT_ERR_MEM,
    IMT_ERR_VALUE,
    IMT_ERR_NO_HANDLER,
    IMT_ERR_INVALID_FILE,
};

typedef int IMT_Handler;

extern int IMT_load_file(const char *filename, IMT_Image **p_image);
extern int IMT_sys_load_file(const char *filename, IMT_Image **p_image);

extern int IMT_save_file(const char *filename, IMT_Image *p_image);
extern int IMT_sys_save_file(const char *filename, IMT_Image *p_image);

extern IMT_Handler *IMT_handler_from_filename(const char *filename);

#endif /* IMT_HANDLER_H */
