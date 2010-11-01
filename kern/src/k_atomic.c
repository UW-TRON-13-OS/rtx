#include "k_atomic.h"
#include <rtx.h> // NULL definition
#include <signal.h>

#define FIRST_ON 1
#define LAST_OFF 0
static int atomic_count = 0;
void atomic(int on_off)
{
    static sigset_t oldmask;
    sigset_t newmask;
    if (on_off == ON)
    {
        atomic_count++;
        if (atomic_count == FIRST_ON) 
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
        atomic_count--;
        if (atomic_count == LAST_OFF)
        {
            sigprocmask(SIG_SETMASK, &oldmask, NULL);
        }
    }
}
