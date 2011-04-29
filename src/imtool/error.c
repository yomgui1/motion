#include "error.h"

const char *IMT_get_error_string(int err)
{
    const char *s;

    switch (err)
    {
        default: s = "unknown error";
    }

    return s;
}
