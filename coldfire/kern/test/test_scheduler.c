#include "rtx.h"
#include "utest.h"
#include "k_init.h"
#include "k_globals.h"
#include "trace.h"
#include "proc_pq.h"
#include "utils.h"

char *expected = "abbcdedefab";
char result[20];
int append_i = 0;

void append_result()
{
    trace_uint(ALWAYS,"append i ", append_i);
    result[append_i++] = current_process->name[0];
    result[append_i] = '\0';
    trace(ALWAYS,result);
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
                trace(ALWAYS,"10");
                release_processor();
                break;
            default:
                trace_uint(ALWAYS,"append_i ", append_i);
                utest_assert(0, "b process append_i not recongized");
                break;
        }
        trace(ALWAYS,"end of loop");
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
                trace(ALWAYS,"doneee !!!");
                trace_ptr(ALWAYS,"rtx_strcmp ", rtx_strcmp);
                trace_ptr(ALWAYS,"result ", result);
                trace_ptr(ALWAYS,"expected ", expected);
                trace(ALWAYS,"result");
                trace(ALWAYS,result);
                trace(ALWAYS,"expected");
                trace(ALWAYS,expected);
                if (rtx_strcmp(result, expected) == 0)
                {
                    trace(ALWAYS,"YAY IT WORKS!!!");
                }
                utest_assert(rtx_strcmp(result, expected) == 0, "Result did not match expected sequence");
                if (rtx_strcmp(result, expected) != 0)
                {
                    trace_str(ALWAYS, "expected : ", expected);
                    trace_str(ALWAYS, "got      : ", result);
                }
                trace(ALWAYS,"test result timeee");

                utest_test_results();
                break;
            default:
                trace_uint(ALWAYS,"append_i ", append_i);
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
                trace_uint(ALWAYS,"append_i ", append_i);
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
                trace_uint(ALWAYS,"append_i ", append_i);
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
    utest_start();
    enable_debug = 1;

    append_i = 0;
    trace_uint(ALWAYS,"append i ", append_i);
    trace_ptr(ALWAYS,"expected ", expected);
    trace(ALWAYS,expected);

    pcb_init_t itable[6];

    itable[0].pid = 0;
    itable[0].name = "a";
    itable[0].priority = 0;
    itable[0].start = start_a;
    itable[0].stack_size = 4096;
    itable[0].is_i_process = 0;
    itable[0].is_sys_process = 0;

    itable[1].pid = 1;
    itable[1].name = "b";
    itable[1].priority = 0;
    itable[1].start = start_b;
    itable[1].stack_size = 4096;
    itable[1].is_i_process = 0;
    itable[1].is_sys_process = 0;

    itable[2].pid = 2;
    itable[2].name = "c";
    itable[2].priority = 1;
    itable[2].start = start_c;
    itable[2].stack_size = 4096;
    itable[2].is_i_process = 0;
    itable[2].is_sys_process = 0;

    itable[3].pid = 3;
    itable[3].name = "d";
    itable[3].priority = 1;
    itable[3].start = start_d;
    itable[3].stack_size = 4096;
    itable[3].is_i_process = 0;
    itable[3].is_sys_process = 0;

    itable[4].pid = 4;
    itable[4].name = "e";
    itable[4].priority = 1;
    itable[4].start = start_e;
    itable[4].stack_size = 4096;
    itable[4].is_i_process = 0;
    itable[4].is_sys_process = 0;

    itable[5].pid = 5;
    itable[5].name = "f";
    itable[5].priority = 2;
    itable[5].start = start_f;
    itable[5].stack_size = 4096;
    itable[5].is_i_process = 0;
    itable[5].is_sys_process = 0;

    k_init(itable, 6, FALSE, FALSE);

    return 0;
}
