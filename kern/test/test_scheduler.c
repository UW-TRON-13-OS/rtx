#include "rtx.h"
#include "k_init.h"
#include "k_process.h"
#include "k_storage.h"
#include "k_globals.h"
#include "k_scheduler.h"
#include "utest.h"

#include <setjmp.h>
#include <string.h>

//                         1
//               012345678901
char *expected = "abbcdedefab";
char result[20] = { 0 } ;
int append_i = 0;
jmp_buf test_buf;

void append_result()
{
    result[append_i++] = current_process->name[0];
}

void start_a()
{
    while (1)
    {
        append_result();
        MsgEnv *env;
        switch(append_i)
        {
            case 1:
                env = receive_message();
                release_msg_env(env);
                break;
            case 10:
                release_processor();
                break;
            default:
                printf("append_i %d\n", append_i);
                utest_assert(0, "b process append_i not recongized");
                break;
        }
        release_processor();
    }
}

void start_b()
{
    while (1)
    {
        append_result();
        MsgEnv *env;
        switch (append_i)
        {
            case 2:
                break;
            case 3:
                env = receive_message();
                release_msg_env(env);
                break;
            case 11:
                longjmp(test_buf, 1);
                break;
            default:
                printf("append_i %d\n", append_i);
                utest_assert(0, "b process append_i not recongized");
                break;
        }
        release_processor();
    }
}

void start_c()
{
    while (1)
    {
        append_result();
        MsgEnv * env = receive_message();
        release_msg_env(env);
        utest_assert(0, "C should not run for a second time");
        release_processor();
    }
}

void start_d()
{
    while (1)
    {
        append_result();
        MsgEnv *env;
        switch (append_i)
        {
            case 5:
                break;
            case 7:
                env = receive_message();
                release_msg_env(env);
                utest_assert(0, "D should not run for a second time");
                break;
            default:
                printf("append_i %d\n", append_i);
                utest_assert(0, "d process append_i not recongized");
                break;
        }
        release_processor();
    }
}

void start_e()
{
    while (1)
    {
        append_result();
        MsgEnv *env;
        switch (append_i)
        {
            case 6:
                break;
            case 8:
                env = receive_message();
                release_msg_env(env);
                utest_assert(0, "D should not run for a second time");
                break;
            default:
                printf("append_i %d\n", append_i);
                utest_assert(0, "d process append_i not recongized");
                break;
        }
        release_processor();
    }
}

void start_f()
{
    while (1)
    {
        append_result();
        MsgEnv *env = request_msg_env();
        utest_assert(send_message(0, env) == CODE_SUCCESS, 
                "F could not send an env to A");
        env = request_msg_env();
        utest_assert(send_message(1, env) == CODE_SUCCESS, 
                "F could not send an env to B");
        release_processor();
    }
}

int main(int argc, char * argv[])
{
    k_storage_init();

    proc_cfg_t itable[] = {
        { 0, "a", 0, 0, start_a },
        { 1, "b", 0, 0, start_b },
        { 2, "c", 1, 0, start_c },
        { 3, "d", 1, 0, start_d },
        { 4, "e", 1, 0, start_e },
        { 5, "f", 2, 0, start_f },
    };

    k_process_init(6, itable);

    if (setjmp(test_buf) == 0)
    {
        k_enter_scheduler();
    }

    utest_assert(strcmp(result, expected) == 0, "Result did not match expected sequence");
    if (strcmp(result, expected) != 0)
    {
        printf("expected : %s\n", expected);
        printf("got      : %s\n", result);
    }

    utest_test_results();
}
