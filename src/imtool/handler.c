#include "handler_intern.h"

static _IMT_Handler _dummy_head_handler = {NULL, NULL, NULL};

IMT_Handler *IMT_handler_from_filename(const char *filename)
{
    _IMT_Handler *handler;

    handler = _dummy_head_handler;
    while (NULL != (handler = handler->next))
    {
        if (handler->recognize_from_name(filename))
            return handler;
    }

    return NULL;
}

int IMT_load_file(const char *filename, IMT_Image **p_image)
{
    _IMT_Handler *handler;
    int err;

    /* Let the system do the job first then try internals */
    if ((NULL != _system_handler) && (NULL != _system_handler->load))
    {
        err = _system_handler->load(filename, p_image);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }

    handler = IMT_handler_from_filename(filename);
    if ((NULL != handler) && (NULL != handler->load))
    {
        err = handler->load(filename, p_image);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }
    
    /* try all handlers */
    handler = _dummy_head_handler;
    while (NULL != (handler = handler->next))
    {
        if (NULL == handler->load) continue;

        err = handler->load(filename, p_image);
        if (err != IMT_ERR_INVALID_FILE)
            return err;
    }

    return IMT_ERR_NO_LOADER;
}
