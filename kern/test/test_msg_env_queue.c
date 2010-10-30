#include "utest.h"
#include "msg_env_queue.h"
#include "rtx.h"

void test_create()
{
    msg_env_queue_t * q = msg_env_queue_create();
    utest_assert(q != NULL, "Message Queue was Null");
}

void test_enqueue_border_cases()
{
    MsgEnv env;
    msg_env_queue_t * q = msg_env_queue_create();
    utest_assert(msg_env_queue_enqueue(NULL, NULL) == ERROR_NULL_ARG, 
            "Did not check for Null params");
    utest_assert(msg_env_queue_enqueue(q, NULL) == ERROR_NULL_ARG, 
            "Did not check for Null params");
    utest_assert(msg_env_queue_enqueue(NULL, &env) == ERROR_NULL_ARG, 
            "Did not check for Null params");
}

int main(int argc, char *argv[])
{
    test_create();
    test_enqueue_border_cases();
    return utest_test_results(); 
}
