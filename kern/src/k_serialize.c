#include "k_serialize.h"

void k_memcpy(void *dst, const void *src, int num)
{
    char *d = dst;
    const char *s = src;

    int i;
    for (i = 0; i < num; i++)
    {
        d[i] = s[i];
    }
}

void k_serialize(void **buf, const void * src, int size)
{
    k_memcpy(*buf, src, size);
    *buf += size;
}
