#include "k_ipc.h"

#include <rtx.h>

#include "k_serialize.h"
#include "k_config.h"
#include "k_process.h"
#include "k_scheduler.h"
#include "k_globals.h"
#include "k_clock.h"
#include "msg_env_queue.h"

#ifdef DEBUG_KERN
#include <stdio.h>
#endif

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
#ifdef DEBUG_KERN
    printf("%s is sending %s a message of type %d\n", current_process->name, p_table[dest_pid].name, msg_env->msg_type);
#endif
    if(msg_env == NULL)
    {
#ifdef DEBUG_KERN
        printf("Error: gave send_message a null envelope\n");
#endif
        return ERROR_NULL_ARG;
    }

    if(dest_pid < 0 || dest_pid >= k_get_num_processes())
    {
#ifdef DEBUG_KERN
        printf("Error: gave send_message an invalid pid\n");
#endif
        return ERROR_ILLEGAL_ARG;
    }

    msg_env->send_pid = current_process->pid;
    msg_env->dest_pid = dest_pid;
    pcb_t *dest_pcb = &p_table[dest_pid];
    if (msg_env_queue_enqueue(dest_pcb ->recv_msgs, msg_env) != 0)
    {
#ifdef DEBUG_KERN
        printf("Error: gave send_message could not enqueue the message\n");
#endif
        return ERROR_ERROR_ARG;
    }

    //if the destination process is blocked waiting for message then
    // Don't need to do this now as it will make stuff more complicated.
    if (dest_pcb->status == P_BLOCKED_ON_RECEIVE)
    {
#ifdef DEBUG_KERN
        printf("send_message found a process waiting for a receive. waking up %s\n", dest_pcb->name);
#endif
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
#ifdef DEBUG_KERN
        printf("%s blocked on receive\n", current_process->name);
#endif
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
   elem->time_stamp = k_clock_get_system_time();

   // update the head and tail
   tbuf->tail = (tbuf->tail + 1) % IPC_MESSAGE_TRACE_HISTORY_SIZE; 
}


