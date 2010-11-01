#ifndef _K_IPC_H_
#define _K_IPC_H_

#include <rtx.h>

void k_ipc_init();

int k_send_message(int dest_pid, MsgEnv *msg_env);
MsgEnv * k_receive_message();

MsgEnv * k_request_msg_env();
int k_release_msg_env(MsgEnv * msg_env);

#endif
