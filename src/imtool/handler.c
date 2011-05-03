#include "handler_intern.h"

static _IMT_Handler _dummy_head_handler = {NULL};

IMT_Handler *IMT_HandlerFromFilename(const char *filename)
{
    _IMT_Handler *handler;

    handler = &_dummy_head_handler;
    while (NULL != (handler = handler->next))
    {
        if (handler->recognize_from_name && handler->recognize_from_name(filename))
            return (IMT_Handler *)handler;
    }

    return NULL;
}

void IMT_Init(void)
{
	extern _IMT_Handler _imt_png_handler;
	_dummy_head_handler.next = &_imt_png_handler;
}

int IMT_Load(const char *filename, IMT_Image **p_image, void *options)
{
    _IMT_Handler *handler;
    int err;

    /* Let the system do the job first then try internals */
    if ((NULL != _system_handler) && (NULL != _system_handler->load))
    {
        err = _system_handler->load(filename, p_image, options);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }

    handler = (_IMT_Handler *)IMT_HandlerFromFilename(filename);
    if ((NULL != handler) && (NULL != handler->load))
    {
        err = handler->load(filename, p_image, options);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }
    
    /* try all handlers */
    handler = &_dummy_head_handler;
    while (NULL != (handler = handler->next))
    {
        if (NULL == handler->load) continue;

        err = handler->load(filename, p_image, options);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }

    return IMT_ERR_NO_HANDLER;
}

int IMT_Save(const char *filename, IMT_Image *image, void *options)
{
    _IMT_Handler *handler;
    int err;
	
    /* Let the system do the job first then try internals */
    if ((NULL != _system_handler) && (NULL != _system_handler->save))
    {
		
        err = _system_handler->save(filename, image, options);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }

    handler = (_IMT_Handler *)IMT_HandlerFromFilename(filename);
    if ((NULL != handler) && (NULL != handler->save))
    {
        err = handler->save(filename, image, options);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }

    return IMT_ERR_NO_HANDLER;
}