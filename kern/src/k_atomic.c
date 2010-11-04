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
    //if (1) return;
    static sigset_t oldmask;
    sigset_t newmask;
#ifdef DEBUG_KERN
    if (current_process->pid != PROCESS_NULL_PID)
        printf("%s count %d on_off %d\n", current_process->name, current_process->atomic_count, on_off);
#endif
    if (on_off == ON)
    {
        current_process->atomic_count++;
        //assert(current_process->atomic_count < 2);
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
