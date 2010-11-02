#ifndef _RTX_H_
#define _RTX_H_

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#define CODE_SUCCESS        0
#define ERROR_NULL_ARG      1
#define ERROR_ILLEGAL_ARG   2

#define IPC_MESSAGE_TRACE_HISTORY_SIZE 16

typedef void (*start_pc)();
typedef struct MsgEnv {
    struct MsgEnv *next;
    uint32_t dest_pid;
    uint32_t send_pid;
    uint32_t msg_type;
    char *   msg;
} MsgEnv;

typedef struct ipc_trace {
    uint32_t dest_pid;
    uint32_t send_pid;
    uint32_t msg_type;
    uint64_t time_stamp;
} ipc_trace_t;

typedef enum p_status {
    P_READY, P_EXECUTING, P_BLOCKED_ON_ENV_REQUEST, P_BLOCKED_ON_RECEIVE
} p_status_t;

typedef struct process_status {
    uint32_t    pid;
    p_status_t  status;
    uint32_t priority;
} process_status_t;

/** 5.1 Interprocess Communication **/
int send_message(int dest_pid, MsgEnv *msg_env);
MsgEnv * receive_message();

/** 5.2 Storage Management **/
MsgEnv * request_msg_env();
int release_msg_env(MsgEnv * msg_env);

/** 5.3 Processor Management **/
int release_processor();
int request_process_status(MsgEnv *msg_env);
int terminate();
int change_priority(int new_priority, int target_process_id);

/** 5.4 Timing Servicies **/
int request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env);

/** 5.5 System Console I/O **/
int send_console_chars(MsgEnv *msg_env);
int get_console_chars(MsgEnv *msg_env);

/** 5.6 Interprocess Message Trace **/
int get_trace_buffers(MsgEnv* msg_env);

#endif
