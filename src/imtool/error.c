#include "error.h"

const char *IMT_GetErrorString(int err)
{
    const char *s;

    switch (err)
    {
		case IMT_ERR_NOERROR: s = "no error"; break;
		case IMT_ERR_SYS: s = "system error"; break;
		case IMT_ERR_MEM: s = "memory error"; break;
		case IMT_ERR_IO: s = "io error"; break;
		case IMT_ERR_VALUE: s = "bad value"; break;
		case IMT_ERR_NO_HANDLER: s = "no handler found"; break;
		case IMT_ERR_INVALID_FILE: s = "invalid file"; break;
		case IMT_ERR_FORMAT: s = "bad pixel format"; break;
        default: s = "unknown error";
    }

    return s;
}
