#include "rtx.h"

/** 5.1 Interprocess Communication **/
int k_send_message(int dest_pid, MsgEnv *msg_env);
MsgEnv * k_receive_message();

/** 5.2 Storage Management **/
MsgEnv * k_request_msg_env();
int k_release_msg_env(MsgEnv * msg_env);

/** 5.3 Processor Management **/
int k_release_processor();
int k_request_process_status(MsgEnv *msg_env);
int k_terminate();
int k_change_priority(int new_priority, int target_process_id);

/** 5.4 Timing Servicies **/
int k_request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env);

/** 5.5 System Console I/O **/
int k_send_console_chars(MsgEnv *msg_env);
int k_get_console_chars(MsgEnv *msg_env);

/** 5.6 Interprocess Message Trace **/
int k_get_trace_buffers(MsgEnv* msg_env);
