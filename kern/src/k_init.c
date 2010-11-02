#include "k_init.h"
#include "k_config.h"
#include "k_process.h"
#include "k_storage.h"
#include "k_ipc.h"
#include "rtx.h"
#include "k_globals.h"
#include "k_signal_handler.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "keyboard_process.h"
#include "crt_process.h"

#define KEYBOARD_SHMEM_FILE "keyboard.dat"
#define CRT_SHMEM_FILE "crt.dat"

void k_init()
{
    k_ipc_init();
    k_storage_init();

    proc_cfg_t init_table[NUM_PROCESSES] = { };
    k_init_processes(init_table);

    // Register for the appropriate unix signals
    // TODO register for die
    //struct sigaction die_sig;
    //die_sig.sa_handler = handle_signal;
    //sigaddset(&i_process_mask, SIGINT);  // Ctrl-C
    
    //struct sigaction i_process_action;
    //sigset_t i_process_mask;
    //sigemptyset(&i_process_mask);
    //sigaddset(&i_process_mask, SIGALRM); // Timeout signal
    //sigaddset(&i_process_mask, SIGUSR1); // CRT signal
    //sigaddset(&i_process_mask, SIGUSR2); // KB signal
    //i_process_action.sa_handler = handle_signal;
    //i_process_action.sa_mask = i_process_mask;
    //i_process_action.sa_flags = 0;
    sigset(SIGALRM, handle_signal);
    sigset(SIGUSR1, handle_signal);
    sigset(SIGUSR2, handle_signal);


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

    status = ftruncate(kb_fid, sizeof(send_buf_t));
    if (status != 0) 
    {
        printf("Could not truncate the file %s to %d bytes. status %d\n", 
                CRT_SHMEM_FILE, sizeof(send_buf_t), status);
        exit(1);
    }
    
    pid_t parent_pid = getpid();

    // Setup mmap file for keyboard and fork it
    void *  mmap_ptr = mmap(NULL, sizeof(recv_buf_t), PROT_READ | PROT_WRITE, MAP_SHARED, kb_fid, 0);
    if (mmap_ptr == MAP_FAILED)
    {
        printf("Could not create mmap pointer to %s\n", KEYBOARD_SHMEM_FILE);
        exit(1);
    }
    kb_buf = (recv_buf_t *) mmap_ptr;

    int kb_child_pid = fork();
    if (kb_child_pid == 0)
    {
        // TODO uncomment when keyboard process is done
        //start_keyboard_process(parent_pid, kb_buf);
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

    int crt_child_pid = fork();
    if (crt_child_pid == 0)
    {
        //TODO uncomment when crt process is done
        //start_crt_process(parent_pid, crt_buf);
        exit(0);
    }

    // Jump to the first process
    pcb_t * first_pcb = proc_pq_dequeue(first_pcb);
    current_process = first_pcb;
    first_pcb->status = P_EXECUTING;
    longjmp(first_pcb->context);
}
