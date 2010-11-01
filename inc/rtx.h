#ifndef _RTX_H_
#define _RTX_H_

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#define CODE_SUCCESS        0
#define ERROR_NULL_ARG      1
#define ERROR_ILLEGAL_ARG   2

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

#endif
