#include "timeout_i_process.h"
#include "k_ipc.h"
#include "k_globals.h"
#include "msg_env_queue.h"
#include "k_signal_handler.h"
#include "k_clock.h"
#include "timeout_queue.h"

void start_timeout_i_process()
{
    while(1)
    {
        // Record a tick
        k_clock_incr_system_time();

        MsgEnv* msg_env = k_receive_message();
        while (msg_env != NULL)
        {
            timeout_queue_insert(msg_env);
            msg_env = k_receive_message();
        }

		msg_env = check_timeout_queue(msg_env);
		
		// Send the envelope back
		k_send_message(msg_env->send_pid, msg_env);
		
		//exit i process
        k_i_process_exit();
    }
}
