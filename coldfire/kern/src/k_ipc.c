#include "k_init.h"
#include "k_globals.h"
#include "k_clock.h"

msg_env_queue_t *free_env_q;
msg_env_queue_t *sys_free_env_q;

MsgEnv * env_pool;
MsgEnv * sys_env_pool;

trace_circle_buf_t send_trace_buf;
trace_circle_buf_t recv_trace_buf;

bool k_is_msg_env_sys_env(MsgEnv *env)
{
    if (env >= sys_env_pool && env < sys_env_pool + IPC_NUM_SYS_FREE_MSG_ENVS)
    {
        uint32_t offset = (uint32_t) env - (uint32_t) sys_env_pool;
        return offset % sizeof(*env) == 0;
    }
    return FALSE;
}

void init_ipc()
{
#define BUF_SIZE = (sizeof(*send_trace_buf.buf) * IPC_MESSAGE_TRACE_HISTORY_SIZE)
    free_env_q = msg_env_queue_create();
    sys_free_env_q = msg_env_queue_create();
    env_pool = k_malloc(sizeof(*env_pool)*IPC_NUM_FREE_MSG_ENVS);
    sys_env_pool = k_malloc(sizeof(*env_pool)*IPC_NUM_SYS_FREE_MSG_ENVS);
    int env_no;
    for (env_no = 0; env_no < IPC_NUM_FREE_MSG_ENVS; env_no++)
    {
        env_pool[env_no].next = NULL;
        env_pool[env_no].msg_type = 0;
        env_pool[env_no].msg = k_malloc(IPC_MSG_ENV_MSG_SIZE);
        env_pool[env_no].msg[0] = '\0';
        msg_env_queue_enqueue(free_env_q, &env_pool[env_no]);
    }

    for (env_no = 0; env_no < IPC_NUM_SYS_FREE_MSG_ENVS; env_no++)
    {
        sys_env_pool[env_no].next = NULL;
        sys_env_pool[env_no].msg_type = 0;
        sys_env_pool[env_no].msg = k_malloc(IPC_MSG_ENV_MSG_SIZE);
        sys_env_pool[env_no].msg[0] = '\0';
        msg_env_queue_enqueue(sys_free_env_q, &sys_env_pool[env_no]);
    }

    send_trace_buf.tail = 0;
    send_trace_buf.buf = k_malloc(sizeof(*send_trace_buf.buf) * 
                                  IPC_MESSAGE_TRACE_HISTORY_SIZE);
    recv_trace_buf.tail = 0;
    recv_trace_buf.buf = k_malloc(sizeof(*recv_trace_buf.buf) * 
                                  IPC_MESSAGE_TRACE_HISTORY_SIZE);
    for (env_no = 0; env_no < IPC_MESSAGE_TRACE_HISTORY_SIZE; env_no++)
    {
        send_trace_buf.buf[env_no].time_stamp = MAX_UINT32;
        recv_trace_buf.buf[env_no].time_stamp = MAX_UINT32;
    }
}

int find_trace_buf_head(trace_circle_buf_t *tbuf)
{
    int head = circle_index(tbuf->tail);
    while (tbuf->buf[head].time_stamp == MAX_UINT32 &&
           head != circle_index(tbuf->tail-1)) 
    {
        head = circle_index(head + 1);
    }
    return head;
}

void log_msg_event(trace_circle_buf_t *tbuf, MsgEnv *msg_env)
{
    tbuf->buf[tbuf->tail].dest_pid = msg_env->dest_pid;
    tbuf->buf[tbuf->tail].send_pid = msg_env->send_pid;
    tbuf->buf[tbuf->tail].msg_type = msg_env->msg_type;
    tbuf->buf[tbuf->tail].time_stamp = k_clock_get_system_time();
    tbuf->tail = circle_index(tbuf->tail + 1);
}
