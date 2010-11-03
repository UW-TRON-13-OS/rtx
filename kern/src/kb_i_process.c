#include "kb_i_process.h"
#include "k_globals.h"

recv_buf_t * _kb_buf;

void kb_register_shmem(recv_buf_t * kb_buf)
{
    _kb_buf = kb_buf;
}

void start_kb_i_process()
{
}
