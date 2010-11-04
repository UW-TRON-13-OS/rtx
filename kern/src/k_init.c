// needed for sigset
#define _XOPEN_SOURCE 500

#include "k_init.h"
#include "k_config.h"
#include "k_process.h"
#include "k_storage.h"
#include "k_scheduler.h"
#include "k_ipc.h"
#include "rtx.h"
#include "k_globals.h"
#include "msg_env_queue.h"
#include "proc_pq.h"
#include "kb_i_process.h"
#include "crt_i_process.h"
#include "k_uart.h"
#include "processes.h"
#include "timeout_i_process.h"
#include "null_process.h"
#include "k_signal_handler.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define NUM_I_PROCESSES 3
#define NULL_PROCESS 1
#define TOTAL_NUM_PROCESSES (NUM_I_PROCESSES+NUM_USER_PROCESSES+NULL_PROCESS)

void die()
{
    k_terminate();
}

void k_init()
{
    printf("Intializing rtx\n");

    printf("Intializing ipc...");
    fflush(stdout);
    k_ipc_init();
    printf("done\n");

    printf("Intializing storage...");
    fflush(stdout);
    k_storage_init();
    printf("done\n");

    proc_cfg_t init_table[TOTAL_NUM_PROCESSES] = {
    //  { pid, name, priority, is_i_process, start_fn}
        { KB_I_PROCESS_PID,      "kb-i",        0, IS_I_PROCESS,     start_kb_i_process },
        { CRT_I_PROCESS_PID,     "crt-i",       0, IS_I_PROCESS,     start_crt_i_process },
        { TIMEOUT_I_PROCESS_PID, "timeout-i",   0, IS_I_PROCESS,     start_timeout_i_process },
        { PROCESS_NULL_PID,      "null",        3, IS_NOT_I_PROCESS, start_null },
        { PROCESS_CCI_PID,       "cci",         0, IS_NOT_I_PROCESS, start_cci }
    };
    printf("Intializing processes...");
    fflush(stdout);
    k_process_init(TOTAL_NUM_PROCESSES, init_table);
    printf("done\n");

    // Register for the appropriate unix signals
    printf("Registering for signals...");
    fflush(stdout);
    sigset(SIGALRM, handle_signal);
    sigset(SIGINT, handle_signal);
    sigset(SIGBUS, handle_signal);
    sigset(SIGHUP, handle_signal);
    sigset(SIGILL, handle_signal);
    sigset(SIGQUIT, handle_signal);
    sigset(SIGSEGV, handle_signal);
    sigset(SIGTERM, handle_signal);
    sigset(SIGABRT, handle_signal);

    // Register for timeout alarm signal
    ualarm(DELAY_TIME, TIMEOUT_100MS);
    printf("done\n");

    k_uart_init();

    printf("Done Bootup...Starting RTX\n"
           "rtx pid: %u keyboard pid: %u crt pid: %u\n"
            "=============================================\n", rtx_pid, kb_child_pid, crt_child_pid);

    // Jump to the first process
    k_enter_scheduler();
}

int k_terminate()
{
    printf("\n");
    printf("Shutting down...%u\n", getpid());
    printf("Killing keyboard child...%u\n", kb_child_pid);
    printf("Killing crt child...%u\n", crt_child_pid);

    k_uart_cleanup();

    // Free allocated memory
    int pid;
    for (pid = 0; pid < k_get_num_processes(); pid++)
    {
        msg_env_queue_destroy(p_table[pid].recv_msgs);
    }
    k_storage_cleanup();
    proc_pq_destroy(ready_pq);
    exit(0);
}
