#include "k_atomic.h"
#include <rtx.h> // NULL definition
#include "k_globals.h"
#include "processes.h"
#include <signal.h>
#include <stdio.h>

#define FIRST_ON 1
#define LAST_OFF 0
void atomic(int on_off)
{
#ifdef DEBUG_KERN
    if (current_process != NULL && current_process->is_i_process)
    {
        printf("ERROR atomic %d is being called from an i process %s\n", on_off, current_process->name);
        assert(!current_process->is_i_process);
    }
#endif

    static sigset_t oldmask;
    sigset_t newmask;
    if (on_off == ON)
    {
        current_process->atomic_count++;
        if (current_process->atomic_count == FIRST_ON) 
        {
            sigemptyset(&newmask);
            sigaddset(&newmask, SIGALRM); // Timeout signal
            sigaddset(&newmask, SIGINT);  // Ctrl-C
            sigaddset(&newmask, SIGUSR1); // CRT signal
            sigaddset(&newmask, SIGUSR2); // KB signal
            sigprocmask(SIG_BLOCK, &newmask, &oldmask);
        }
    }
    else
    {
        current_process->atomic_count--;
        assert(current_process->atomic_count >= 0);
        if (current_process->atomic_count == LAST_OFF)
        {
            sigprocmask(SIG_SETMASK, &oldmask, NULL);
        }
    }
}
