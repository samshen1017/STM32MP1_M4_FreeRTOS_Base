#include "phex.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <FreeRTOS.h>

char *pHex(const char *src, int len)
{
    assert(src != NULL);
    static char *hex = NULL;
    int hex_len = len * 2 + 1;

    if (hex != NULL)
    {
        vPortFree(hex);
        hex = NULL;
    }

    hex = (char *)pvPortMalloc(hex_len);
    if (hex == NULL)
    {
        printf("hex pvPortMalloc faile, len:%d.\n", hex_len);
        goto _ERR_NOMEM;
    }
    memset(hex, 0, hex_len);

    for (int i = 0; i < len; ++i)
    {
        sprintf(hex + (i * 2), "%02x", src[i]);
    }

_ERR_NOMEM:
    return hex;
}
