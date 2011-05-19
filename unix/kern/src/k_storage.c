#include "k_storage.h"
#include "k_globals.h"
#include "k_config.h"
#include "k_process.h"
#include "k_scheduler.h"
#include "k_config.h"

#include <stdlib.h>

msg_env_queue_t * free_env_q;
MsgEnv* env_pool;
proc_pq_t *env_blocked_pq;

void k_storage_init()
{
	// create all the queues used by the OS
    env_blocked_pq = proc_pq_create(NUM_PRIORITIES);
    free_env_q = msg_env_queue_create();
    env_pool = malloc(sizeof(*env_pool) * IPC_NUM_FREE_MSG_ENVS);
    int i;
    for (i = 0; i < IPC_NUM_FREE_MSG_ENVS; i++)
    {
        env_pool[i].msg = malloc(sizeof(*env_pool[i].msg) * MSG_ENV_MSG_SIZE);
        env_pool[i].next = NULL;
        msg_env_queue_enqueue(free_env_q, &env_pool[i]);
    }
}

MsgEnv * k_request_msg_env()
{
    while (msg_env_queue_is_empty(free_env_q))
    {
        if (current_process->is_i_process)
        {
            return NULL;
        }
        proc_pq_enqueue(env_blocked_pq, current_process);
        k_process_switch(P_BLOCKED_ON_ENV_REQUEST);
    }

    MsgEnv *env = msg_env_queue_dequeue(free_env_q);
    return env;
}

int k_release_msg_env(MsgEnv * msg_env)
{
	// do a null check on the envelope
    if (msg_env == NULL)
    {
        return ERROR_NULL_ARG;
    }

    msg_env_queue_enqueue(free_env_q, msg_env);
    pcb_t * blocked_process = proc_pq_dequeue(env_blocked_pq);
    if (blocked_process)
    {
        blocked_process->status = P_READY;
        assert(proc_pq_enqueue(ready_pq, blocked_process) == CODE_SUCCESS);
    }
    return CODE_SUCCESS;
}

int k_is_msg_env_valid(MsgEnv *msg_env)
{
    return msg_env >= env_pool && msg_env < env_pool + IPC_NUM_FREE_MSG_ENVS;
}
