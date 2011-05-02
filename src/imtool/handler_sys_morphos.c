#define USE_INLINE_STDARG

#include "handler_intern.h"

#include <proto/exec.h>
#include <proto/dos.h>

_IMT_Handler *_system_handler = NULL;
