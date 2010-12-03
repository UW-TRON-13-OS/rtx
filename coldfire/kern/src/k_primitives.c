#include "k_primitives.h"
#include "k_structs.h"
#include "k_globals.h"
#include "k_clock.h"
#include "trace.h"

msg_env_queue_t *free_env_q;

/** 5.1 Interprocess Communication **/
int k_send_message(int dest_pid, MsgEnv *msg_env)
{
    if(msg_env == NULL || msg_env->msg == NULL)
    {
        return ERROR_NULL_ARG;
    }

    pcb_t *dest_pcb = k_get_process(dest_pid);
    if(dest_pcb == NULL)
    {
        return ERROR_ILLEGAL_ARG;
    }

    msg_env->send_pid = current_process->pid;
    msg_env->dest_pid = dest_pid;

    msg_env_queue_enqueue(dest_pcb->recv_msgs, msg_env);
    dest_pcb->env_owned++;
    current_process->env_owned--;
    if (dest_pcb->state == P_BLOCKED_ON_RECEIVE)
    {
        dest_pcb->state = P_READY;
        proc_pq_enqueue(ready_pq, dest_pcb );
    }

    log_msg_event(&send_trace_buf, msg_env);

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
    log_msg_event(&recv_trace_buf, msg_env);

    return msg_env;
}

/** 5.2 Storage Management **/
MsgEnv * k_request_msg_env()
{
    if (msg_env_queue_is_empty(free_env_q))
    {
        if (current_process->is_i_process && 
                !msg_env_queue_is_empty(sys_free_env_q))
        {
            current_process->env_owned++;
            return msg_env_queue_dequeue(sys_free_env_q);
        }

        if (current_process->is_i_process)
        {
            return NULL;
        }

        while (msg_env_queue_is_empty(free_env_q))
        {
            proc_pq_enqueue(blocked_request_env_pq, current_process);
            k_process_switch(P_BLOCKED_ON_ENV_REQUEST);
        }
    }

    current_process->env_owned++;
    return msg_env_queue_dequeue(free_env_q);
}

int k_release_msg_env(MsgEnv * msg_env)
{
    if (msg_env == NULL || msg_env->msg == NULL)
    {
        return ERROR_NULL_ARG;
    }

    if (k_is_msg_env_sys_env(msg_env))
    {
        msg_env_queue_enqueue(sys_free_env_q, msg_env);
    }
    else
    {
        msg_env_queue_enqueue(free_env_q, msg_env);
        pcb_t * blocked_process = proc_pq_dequeue(blocked_request_env_pq);
        if (blocked_process)
        {
            blocked_process->state = P_READY;
            proc_pq_enqueue(ready_pq, blocked_process);
        }
    }
    current_process->env_owned--;
    return CODE_SUCCESS;
}

/** 5.3 Processor Management **/
int k_release_processor()
{
    proc_pq_enqueue(ready_pq, current_process);
    k_process_switch(P_READY);
    return CODE_SUCCESS;
}

int k_request_process_status(MsgEnv *msg_env)
{
    if (msg_env == NULL || msg_env->msg == NULL)
    {
        return ERROR_NULL_ARG;
    }

    int32_t * data = (int32_t *) msg_env->msg;
    int pid;
    *data++ = k_get_num_processes();
    for (pid = 0; pid < k_get_num_processes(); pid++)
    {
        pcb_t * process = k_get_process(pid_table[pid]);
        *data++ = process->pid;
        *data++ = process->state;
        *data++ = process->priority;
    }
    return CODE_SUCCESS;
}

int k_terminate()
{       
    /* Store fake value of 0 for return value from main */ \
    asm("move.l #0, %d7"); \
    /* Get back to the monitor */ \
    asm("move.l #0,%d0"); \
    asm("trap #15"); \

    return -1;
}

int k_change_priority(int new_priority, int target_process_id)
{
    if (new_priority < 0 || new_priority >= NUM_PRIORITIES)
    {
        return ERROR_ILLEGAL_ARG;
    }


    pcb_t *pcb = k_get_process(target_process_id);
    if (pcb->is_i_process || pcb->pid < 0 || pcb->pid == NULL_PID)
    {
        return ERROR_ILLEGAL_ARG;
    }

    if (pcb->priority == new_priority)
    {
        return CODE_SUCCESS;
    }

    switch (pcb->state)
    {
        case P_READY:
            proc_pq_remove(ready_pq, pcb);
            pcb->priority = new_priority;
            proc_pq_enqueue(ready_pq, pcb);
            break;

        case P_BLOCKED_ON_ENV_REQUEST:
            proc_pq_remove(blocked_request_env_pq, pcb);
            pcb->priority = new_priority;
            proc_pq_enqueue(blocked_request_env_pq, pcb);
            break;

        default:
            pcb->priority = new_priority;
            break;
    }

    return CODE_SUCCESS;
}

/** 5.4 Timing Servicies **/
int k_request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env)
{
    msg_env->msg_type = wakeup_code;
    int * delay_data = (int *) msg_env->msg;
    *delay_data = time_delay;
    return k_send_message(TIMER_I_PROCESS_PID, msg_env);
}

/** 5.5 System Console I/O **/
int k_send_console_chars(MsgEnv *msg_env, bool request_ack)
{
    if (msg_env == NULL)
        return ERROR_NULL_ARG;
    if (request_ack)
    {
        msg_env->msg_type = CONSOLE_OUTPUT_REQUEST_ACK;
    }
    else
    {
        msg_env->msg_type = CONSOLE_OUTPUT;
    }
    int ret = k_send_message(UART_I_PROCESS_PID, msg_env);
    SERIAL1_IMR = 3;
    return ret;
}

/** 5.6 Interprocess Message Trace **/
int k_get_trace_buffers(MsgEnv* msg_env)
{
    if (msg_env == NULL || msg_env->msg == NULL)
    {
        return ERROR_NULL_ARG;
    }

    int i, send_head, recv_head;

    // Find the heads of the trace buffers
    send_head = find_trace_buf_head(&send_trace_buf);
    recv_head = find_trace_buf_head(&recv_trace_buf);

    // Dump the sent messages and received messages
    ipc_trace_t *send_dump = (ipc_trace_t *) msg_env->msg;
    ipc_trace_t *recv_dump = send_dump + IPC_MESSAGE_TRACE_HISTORY_SIZE;

    for (i = 0; i < IPC_MESSAGE_TRACE_HISTORY_SIZE; i++)
    {
        ipc_trace_t *trace = &send_trace_buf.buf[circle_index(send_head+i)];
        send_dump->dest_pid = trace->dest_pid;
        send_dump->send_pid = trace->send_pid;
        send_dump->msg_type = trace->msg_type;
        send_dump->time_stamp = trace->time_stamp;
        send_dump++;

        trace = &recv_trace_buf.buf[circle_index(recv_head+i)];
        recv_dump->dest_pid = trace->dest_pid;
        recv_dump->send_pid = trace->send_pid;
        recv_dump->msg_type = trace->msg_type;
        recv_dump->time_stamp = trace->time_stamp;
        recv_dump++;
    }
    return CODE_SUCCESS;
}
