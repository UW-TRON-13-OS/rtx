#include "utest.h"
#include "rtx.h"
#include "k_process.h"
#include "k_ipc.h"
#include "k_storage.h"
#include "k_globals.h"
#include "k_scheduler.h"
#include "k_uart.h"
#include "kb_i_process.h"
#include "crt_i_process.h"
#include "timeout_i_process.h"
#include "null_process.h"
#include "processes.h"

#include <setjmp.h>
#include <unistd.h>

jmp_buf test_buf;
void start_test()
{
    char * msg = "Testing send console chars\n";
    MsgEnv * env = request_msg_env();
    utest_assert(env != NULL, "Could not allocate envelope");
    utest_assert(send_console_chars(NULL) == ERROR_NULL_ARG, "Send console chars did not handle null env");
    if (env)
    {
        env->msg = msg; // note this actually causes a memory link
    }
    utest_assert(send_console_chars(env) == CODE_SUCCESS, "Send console chars with env was not sucessful");
    usleep(100 * 1000); // give some time for crt process to poll
    release_msg_env(env);
    longjmp(test_buf, 1);
}

int main(int argc, char * argv[])
{
    k_ipc_init();
    k_storage_init();

#define NUM_PROCESSES 5
    proc_cfg_t itable[] = {
        { KB_I_PROCESS_PID, "kb-i", 0, IS_I_PROCESS, start_kb_i_process },
        { CRT_I_PROCESS_PID, "crt-i", 0, IS_I_PROCESS, start_crt_i_process },
        { TIMEOUT_I_PROCESS_PID, "timeout-i", 0, IS_I_PROCESS, start_timeout_i_process },
        { PROCESS_NULL_PID, "null", 3, IS_NOT_I_PROCESS, start_null },
        { 4, "test", 0, IS_NOT_I_PROCESS, start_test }
    };
    k_process_init(NUM_PROCESSES, itable);
    k_uart_init();
    if (setjmp(test_buf) == 0)
    {
        k_enter_scheduler();
    }
    k_uart_cleanup();
    utest_test_results();
}
