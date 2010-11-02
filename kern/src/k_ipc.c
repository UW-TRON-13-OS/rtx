#include "k_ipc.h"

#include <rtx.h>

#include "k_serialize.h"
#include "k_config.h"

// Internal data structs
typedef struct trace_circle_buf {
    uint32_t tail;
    ipc_trace_t buf[IPC_MESSAGE_TRACE_HISTORY_SIZE];
} trace_circle_buf_t;

// Static variables
trace_circle_buf_t _send_trace_buf = { 0 };
trace_circle_buf_t _recv_trace_buf = { 0 };

// Static functions
int _find_trace_buf_head(trace_circle_buf_t *buf);
void _log_msg_event(trace_circle_buf_t *buf, MsgEnv *msg_env);

/** Kernel Only **/
void k_ipc_init()
{
    _send_trace_buf.tail = 0;
    _recv_trace_buf.tail = 0;
}

int k_send_message(int dest_pid, MsgEnv *msg_env)
{
    if(msg_env == NULL)
        return ERROR_NULL_ARGUMENT
    else if(dest_pid < 0 || dest_pid > NUM_PROCESSES)
        return ERROR_ILLEGAL_ARGUMENT

    int send_pid = current_process->pid;
    msg_env->send_pid = send_pid;
    msg_env->dest_pid = dest_pid;

    pcb_t *dest_pcb =  p_table[dest_pid]; 
    if (msg_env_queue_enqueue(dest_pcb->recv_msgs, msg_env) != 0)
        return ERROR_ERROR_ARGUMENT

    //if the destination process is blocked waiting for message then
    // Don't need to do this now as it will make stuff more complicated.
    p_table[dest_pid]->status = P_READY;
    proc_pq_enqueue(ready_pq, p_table[dest_pid]);
    _log_msg_event(_send_trace_buf, msg_env);
    return CODE_SUCCESS
}

MsgEnv * k_receive_message()
{
    return NULL;
}

int k_get_trace_buffers( MsgEnv *msg_env )
{
    int i, send_head, recv_head;

    // Find the heads of the trace buffers
    send_head = _find_trace_buf_head(&_send_trace_buf);
    recv_head = _find_trace_buf_head(&_recv_trace_buf);

    // Dump the sent messages and received messages
    ipc_trace_t *send_dump = (ipc_trace_t *) msg_env->msg;
    ipc_trace_t *recv_dump = (ipc_trace_t *) &msg_env->msg[sizeof(ipc_trace_t) * 
                                                    IPC_MESSAGE_TRACE_HISTORY_SIZE];
    for (i = 0; i < IPC_MESSAGE_TRACE_HISTORY_SIZE; i++)
    {
        k_memcpy(send_dump, 
                 &_send_trace_buf.buf[(send_head + i) % IPC_MESSAGE_TRACE_HISTORY_SIZE],
                 sizeof(ipc_trace_t));
        k_memcpy(recv_dump, 
                 &_recv_trace_buf.buf[(recv_head + i) % IPC_MESSAGE_TRACE_HISTORY_SIZE],
                 sizeof(ipc_trace_t));

        send_dump++;
        recv_dump++;
    }
    return 0;
}

int _find_trace_buf_head(trace_circle_buf_t *tbuf)
{
    // Check for an emtpy trace buffer
    if (tbuf->buf[tbuf->tail].time_stamp == 0)
    {
        return tbuf->tail;
    }

    int head = tbuf->tail;
    while (tbuf->buf[head].time_stamp == 0) // while not initialized
    {
        head = (head + 1) % IPC_MESSAGE_TRACE_HISTORY_SIZE;
    }
    return head;
}

void _log_msg_event(trace_circle_buf_t *tbuf, MsgEnv *msg_env)
{
   ipc_trace_t *elem = &tbuf->buf[tbuf->tail];
   elem->dest_pid = msg_env->dest_pid;
   elem->send_pid = msg_env->send_pid;
   elem->msg_type = msg_env->msg_type;
   elem->time_stamp = 0; // get time stamp

   // update the head and tail
   tbuf->tail = (tbuf->tail + 1) % IPC_MESSAGE_TRACE_HISTORY_SIZE; 
}


