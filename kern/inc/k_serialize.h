#ifndef _KERN_SERIALIZE_
#define _KERN_SERIALIZE_

void k_memcpy(void *dst, const void *src, int num);

void k_serialize(void **buf, const void * src, int size);

#endif
