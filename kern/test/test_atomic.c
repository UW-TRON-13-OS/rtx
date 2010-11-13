#define _XOPEN_SOURCE 500

#include "utest.h"
#include "k_atomic.h"
#include "k_globals.h"
#include "k_process.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int i = 0;
char result[200];
char c;
int pid;
volatile int go;

void handler(int sig_num)
{
    atomic(ON);
    result[i++] = c;
    if (sig_num != SIGUSR2)
    {
        kill(pid, SIGUSR2);
        c++;
    }
    atomic(OFF);
    go = 1;
}

int main(int argc, char *argv[])
{
    c = 'a';

    pcb_t pcb;
    pcb.is_i_process = 0;
    pcb.atomic_count = 0;
    current_process = &pcb;
    
    sigset(SIGUSR1, handler);
    sigset(SIGUSR2, handler);
    sigset(SIGINT, handler);

    pid = getpid();

    kill(pid, SIGUSR1);
    c++;

    atomic(ON);

    kill(pid, SIGUSR1);
    kill(pid, SIGINT);
    c++;

//    atomic(OFF);

    kill(pid, SIGUSR1);
    c++;

    char *expected = "ab";
    if (strcmp(expected, result) != 0)
    {
        printf("Got %s but expected %s\n", result, expected);
    }
    utest_assert_str(result, expected, "Didnt block signals properly");

    utest_test_results();
}

