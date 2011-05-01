#ifndef IMT_HANDLER_H
#define IMT_HANDLER_H

#include <imtool/image.h>

typedef int IMT_Handler;

extern void IMT_init(void);

extern int IMT_load_file(const char *filename, IMT_Image **p_image, void *options);
extern int IMT_sys_load_file(const char *filename, IMT_Image **p_image, void *options);

extern int IMT_save_file(const char *filename, IMT_Image *p_image, void *options);
extern int IMT_sys_save_file(const char *filename, IMT_Image *p_image, void *options);

extern IMT_Handler *IMT_handler_from_filename(const char *filename);

#endif /* IMT_HANDLER_H */
