#include "rtx.h"
#include "k_primitives.h"
#include "msg_env_queue.h"
#include "trace.h"
#include "crt_process.h"
#include "k_globals.h"

void start_crt_process()
{
    while (1)
    {
        MsgEnv* message = k_receive_message();
        enable_debug = 1;
        trace_ptr(ALWAYS, "message ", message);
        if (message != NULL)
        {
            // Enable TX Interrupt
            k_send_message(UART_I_PROCESS_PID, message);
            SERIAL1_IMR = 3;
        }
    }
}
