#ifndef _RTX_IPC_H_
#define _RTX_IPC_H_

typedef struct ipc_trace {
    uint32_t dest_pid;
    uint32_t send_pid;
    uint32_t msg_type;
    uint64_t time_stamp;
} ipc_trace_t;

/** 5.6  Interprocess Message Trace **/
int get_trace_buffers( MsgEnv *message_envelope );

#endif
