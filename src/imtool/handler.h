#ifndef IMT_HANDLER_H
#define IMT_HANDLER_H

#include <imtool/image.h>

typedef int IMT_Handler;

extern void IMT_Init(void);
extern int IMT_Load(const char *filename, IMT_Image **p_image, void *options);
extern int IMT_Save(const char *filename, IMT_Image *p_image, void *options);
extern IMT_Handler *IMT_HandlerFromFilename(const char *filename);

#endif /* IMT_HANDLER_H */
