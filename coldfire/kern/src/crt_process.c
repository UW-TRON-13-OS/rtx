#include "rtx.h"
#include "k_primitives.h"
#include "msg_env_queue.h"
#include "dbug.h"
#include "crt_process.h"

void start_crt_process()
{
    MsgEnv* message = k_receive_message();
    if (message != NULL)
    {
        // Enable TX Interrupt
        SERIAL1_IMR = 3;
        k_send_message(UART_I_PROCESS_PID, message);
    }
    // sleep
}
