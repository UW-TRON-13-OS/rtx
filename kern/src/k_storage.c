#include "k_storage.h"
#include "k_globals.h"
#include "k_config.h"

#include <stdlib.h>

static msg_env_queue_t * free_env_q;
static MsgEnv* env_pool;
void k_storage_init()
{
    free_env_q = msg_env_queue_create();
    env_pool = malloc(sizeof(*env_pool) * IPC_NUM_FREE_MSG_ENVS);
}

void k_storage_cleanup()
{
    msg_env_queue_destroy(free_env_q);
    free(env_pool);
}

MsgEnv * k_request_msg_env()
{
    return NULL;
}

int k_release_msg_env(MsgEnv * msg_env)
{
    return -1;
}

