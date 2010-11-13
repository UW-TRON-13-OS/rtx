#include "k_ipc.h"

#include <rtx.h>

#include "k_config.h"
#include "k_process.h"
#include "k_scheduler.h"
#include "k_globals.h"
#include "k_clock.h"
#include "msg_env_queue.h"

#ifdef DEBUG_KERN
#include <stdio.h>
#endif

#define circle_index(i) ((i)%IPC_MESSAGE_TRACE_HISTORY_SIZE)

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
    {
        return ERROR_NULL_ARG;
    }

    if(dest_pid < 0 || dest_pid >= k_get_num_processes())
    {
        return ERROR_ILLEGAL_ARG;
    }

    msg_env->send_pid = current_process->pid;
    msg_env->dest_pid = dest_pid;
    pcb_t *dest_pcb = &p_table[dest_pid];
    if (msg_env_queue_enqueue(dest_pcb ->recv_msgs, msg_env) != 0)
    {
        return ERROR_ERROR_ARG;
    }

    // If the destination process is blocked waiting for message then
    // Don't need to do this now as it will make stuff more complicated.
    if (dest_pcb->status == P_BLOCKED_ON_RECEIVE)
    {
        dest_pcb->status = P_READY;
        proc_pq_enqueue(ready_pq, dest_pcb );
    }

    _log_msg_event(&_send_trace_buf, msg_env);

    return CODE_SUCCESS;
}

MsgEnv * k_receive_message()
{
    while (msg_env_queue_is_empty(current_process->recv_msgs))
    {
        if (current_process->is_i_process)
        {
            return NULL;
        }
        k_process_switch(P_BLOCKED_ON_RECEIVE);
    }

    MsgEnv *msg_env = msg_env_queue_dequeue(current_process->recv_msgs);
    _log_msg_event(&_recv_trace_buf, msg_env);

    return msg_env;
}

int k_get_trace_buffers( MsgEnv *msg_env )
{
    int i, send_head, recv_head;

    // Find the heads of the trace buffers
    send_head = _find_trace_buf_head(&_send_trace_buf);
    recv_head = _find_trace_buf_head(&_recv_trace_buf);

    // Dump the sent messages and received messages
    ipc_trace_t *send_dump = (ipc_trace_t *) msg_env->msg;
    ipc_trace_t *recv_dump = (ipc_trace_t *) 
        &msg_env->msg[sizeof(*recv_dump) * IPC_MESSAGE_TRACE_HISTORY_SIZE];

    for (i = 0; i < IPC_MESSAGE_TRACE_HISTORY_SIZE; i++)
    {
        ipc_trace_t *trace = &_send_trace_buf.buf[circle_index(send_head+i)];
        send_dump->dest_pid = trace->dest_pid;
        send_dump->send_pid = trace->send_pid;
        send_dump->msg_type = trace->msg_type;
        send_dump->time_stamp = trace->time_stamp;
        send_dump++;

        trace = &_recv_trace_buf.buf[circle_index(recv_head+i)];
        recv_dump->dest_pid = trace->dest_pid;
        recv_dump->send_pid = trace->send_pid;
        recv_dump->msg_type = trace->msg_type;
        recv_dump->time_stamp = trace->time_stamp;
        recv_dump++;
    }
    return 0;
}

int _find_trace_buf_head(trace_circle_buf_t *tbuf)
{
    // Check for an emtpy trace buffer
    if (tbuf->buf[circle_index(tbuf->tail+IPC_MESSAGE_TRACE_HISTORY_SIZE-1)].time_stamp == 0)
    {
        return tbuf->tail;
    }

    int head = tbuf->tail;
    while (tbuf->buf[head].time_stamp == 0) // while not initialized
    {
        head = circle_index(head + 1);
    }
    return head;
}

void _log_msg_event(trace_circle_buf_t *tbuf, MsgEnv *msg_env)
{
    ipc_trace_t *elem = &tbuf->buf[tbuf->tail];
    elem->dest_pid = msg_env->dest_pid;
    elem->send_pid = msg_env->send_pid;
    elem->msg_type = msg_env->msg_type;
    elem->time_stamp = k_clock_get_system_time();
    tbuf->tail = circle_index(tbuf->tail + 1);
}


