#ifndef _K_IPC_H_
#define _K_IPC_H_

#include <rtx.h>

int k_send_message(int dest_pid, MsgEnv *msg_env);
MsgEnv * k_receive_message();

int k_get_trace_buffers(MsgEnv *msg_env);

void k_ipc_init ();

#endif
