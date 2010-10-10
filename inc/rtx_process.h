#ifndef _RTX_PROCESS_H_
#define _RTX_PROCESS_H_

int release_processor();
int request_process_status(MsgEnv *msg_env_ptr);
int terminate();
int change_priority(int new_priority, int target_process_id);

#endif
