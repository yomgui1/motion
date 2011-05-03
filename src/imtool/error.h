#ifndef IMT_ERROR_H
#define IMT_ERROR_H

enum
{
    IMT_ERR_NOERROR=0,
    IMT_ERR_SYS,
    IMT_ERR_MEM,
    IMT_ERR_IO,
    IMT_ERR_VALUE,
    IMT_ERR_NO_HANDLER,
    IMT_ERR_INVALID_FILE,
    IMT_ERR_FORMAT,
};

extern const char *IMT_GetErrorString(int err);

#endif /* IMT_ERROR_H */
