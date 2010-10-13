#include "k_ipc.h"

#include <rtx_ipc.h>
#include <rtx_int.h>
#include <rtx_assert.h>

#include "k_config.h"

// Internal data structs
typedef struct trace_circle_buf {
    uint32_t head, tail;
    ipc_trace_t buf[IPC_MESSAGE_TRACE_HISTORY_SIZE];
} trace_circle_buf_t;

// Static variables
trace_circle_buf_t _send_trace_buf;
trace_circle_buf_t _recv_trace_buf;

// Static functions
void _log_msg_event(trace_circle_buf_t *buf, MsgEnv *msg_env);

/** Kernel Only **/
void k_ipc_init()
{
    _send_trace_buf.head = _send_trace_buf.tail = 0;
    _recv_trace_buf.head = _recv_trace_buf.tail = 0;
}

/** 5.6  Interprocess Message Trace **/
int get_trace_buffers( MsgEnv *message_envelope )
{
    return 0;
}

void _log_msg_event(trace_circle_buf_t *buf, MsgEnv *msg_env)
{

}


