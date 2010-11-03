#include "crt_i_process.h"
#include "k_globals.h"

send_buf_t * _crt_buf;

void crt_register_shmem(send_buf_t * crt_buf)
{
    _crt_buf = crt_buf;
}

void start_crt_i_process()
{
}
