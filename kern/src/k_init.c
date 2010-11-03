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
#include "k_signal_handler.h"
#include "msg_env_queue.h"
#include "proc_pq.h"
#include "kb_i_process.h"
#include "crt_i_process.h"
#include "k_uart.h"
#include "processes.h"
#include "timeout_i_process.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "keyboard_process.h"
#include "crt_process.h"

#define KEYBOARD_SHMEM_FILE "keyboard.dat"
#define CRT_SHMEM_FILE "crt.dat"

#define FAIL -1

#define NUM_I_PROCESSES 3
#define TOTAL_NUM_PROCESSES (NUM_I_PROCESSES+NUM_USER_PROCESSES)

pid_t rtx_pid;
pid_t kb_child_pid;
pid_t crt_child_pid;

recv_buf_t *kb_buf;
send_buf_t *crt_buf;

void die()
{
    k_terminate();
}

void k_init()
{
    k_ipc_init();
    k_storage_init();

    proc_cfg_t init_table[TOTAL_NUM_PROCESSES] = {
    //  { pid, name, priority, is_i_process, start_fn}
        { KB_I_PROCESS_PID,      "kb-i",        0, IS_I_PROCESS,     start_kb_i_process },
        { CRT_I_PROCESS_PID,     "crt-i",       0, IS_I_PROCESS,     start_crt_i_process },
        { TIMEOUT_I_PROCESS_PID, "timeout-i",   0, IS_I_PROCESS,     start_timeout_i_process },
        { PROCESS_CCI_PID,       "cci",         0, IS_NOT_I_PROCESS, start_cci }
    };
    k_init_processes(TOTAL_NUM_PROCESSES, init_table);

    // Register for the appropriate unix signals
    sigset(SIGALRM, handle_signal);
    sigset(SIGUSR1, handle_signal);
    sigset(SIGUSR2, handle_signal);
    sigset(SIGINT, handle_signal);
    sigset(SIGBUS, handle_signal);
    sigset(SIGHUP, handle_signal);
    sigset(SIGILL, handle_signal);
    sigset(SIGQUIT, handle_signal);
    sigset(SIGSEGV, handle_signal);
    sigset(SIGTERM, handle_signal);
    sigset(SIGABRT, handle_signal);

    ualarm(DELAY_TIME, TIMEOUT_100MS);


    // Initialize memory mapped files
    int kb_fid, crt_fid, status;
    kb_fid = open(KEYBOARD_SHMEM_FILE, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755);
    if (kb_fid < 0 )
    {
        printf("Bad open on mmap file %s\n", KEYBOARD_SHMEM_FILE);
        exit(1);
    }

    status = ftruncate(kb_fid, sizeof(recv_buf_t));
    if (status != 0) 
    {
        printf("Could not truncate the file %s to %d bytes. status %d\n", 
                KEYBOARD_SHMEM_FILE, sizeof(recv_buf_t), status);
        exit(1);
    }

    crt_fid = open(CRT_SHMEM_FILE, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755);
    if (crt_fid < 0 )
    {
        printf("Bad open on mmap file %s\n", CRT_SHMEM_FILE);
        exit(1);
    }

    status = ftruncate(crt_fid, sizeof(send_buf_t));
    if (status != 0) 
    {
        printf("Could not truncate the file %s to %d bytes. status %d\n", 
                CRT_SHMEM_FILE, sizeof(send_buf_t), status);
        exit(1);
    }
    
    rtx_pid = getpid();

    // Setup mmap file for keyboard and fork it
    void *  mmap_ptr = mmap(NULL, sizeof(recv_buf_t), PROT_READ | PROT_WRITE, MAP_SHARED, kb_fid, 0);
    if (mmap_ptr == MAP_FAILED)
    {
        printf("Could not create mmap pointer to %s\n", KEYBOARD_SHMEM_FILE);
        exit(1);
    }
    kb_buf = (recv_buf_t *) mmap_ptr;
    close(kb_fid);

    int kb_child_pid = fork();
    if (kb_child_pid == 0)
    {
        // TODO uncomment when keyboard process is done
        //start_keyboard_process(rtx_pid, kb_buf);
        exit(0);
    }
    
    
    // Setup mmap file for crt and fork it
    mmap_ptr = mmap(NULL, sizeof(send_buf_t), PROT_READ | PROT_WRITE, MAP_SHARED, crt_fid, 0);
    if (mmap_ptr == MAP_FAILED)
    {
        printf("Could not create mmap pointer to %s\n", CRT_SHMEM_FILE);
        exit(1);
    }
    crt_buf = (send_buf_t *)mmap_ptr;
    close(crt_fid);

    int crt_child_pid = fork();
    if (crt_child_pid == 0)
    {
        //TODO uncomment when crt process is done
        //start_crt_process(rtx_pid, crt_buf);
        exit(0);
    }

    // Jump to the first process
    k_enter_scheduler();
}

int k_terminate()
{
    printf("Shutting down...\n");

    // kill children
    kill(kb_child_pid, SIGINT);
    kill(crt_child_pid, SIGINT);

    // Wait until they die first
    waitpid(kb_child_pid, NULL, 0);
    waitpid(crt_child_pid, NULL, 0);

    // close shared memory
    int status = munmap(kb_buf, sizeof(*kb_buf));
    if (status == FAIL)
    {
        printf("Unmapping the keyboard shared memory failed\n");
    }

    status = munmap(crt_buf, sizeof(*crt_buf));
    if (status == FAIL)
    {
        printf("Unmapping the keyboard shared memory failed\n");
    }

    // Delete shared memory file
    status = unlink(KEYBOARD_SHMEM_FILE);
    if (status == FAIL)
    {
        printf("Deleting the keyboard shared memory file failed\n");
    }

    status = unlink(CRT_SHMEM_FILE);
    if (status == FAIL)
    {
        printf("Deleting the crt shared memory file failed\n");
    }


    // Free allocated memory
    int pid;
    for (pid = 0; pid < k_get_num_processes(); pid++)
    {
        msg_env_queue_destroy(p_table[pid].recv_msgs);
    }
    proc_pq_destroy(ready_pq);
    proc_pq_destroy(env_blocked_pq);
    exit(0);
}
