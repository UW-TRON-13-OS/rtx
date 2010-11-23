#ifndef _KERN_STORAGE_H_
#define _KERN_STORAGE_H_

#include <rtx.h>

void k_storage_init();

MsgEnv * k_request_msg_env();
int k_release_msg_env(MsgEnv * msg_env);

int k_is_msg_env_valid(MsgEnv *msg_env);

#endif
