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
            "Did not check for Both Null");
    utest_assert(msg_env_queue_enqueue(q, NULL) == ERROR_NULL_ARG, 
            "Did not check for Null msg env");
    utest_assert(msg_env_queue_enqueue(NULL, &env) == ERROR_NULL_ARG, 
            "Did not check for Null queue");
    msg_env_queue_destroy(q);
}

void test_dequeue_border_cases()
{
    msg_env_queue_t * q = msg_env_queue_create();
    utest_assert(msg_env_queue_dequeue(NULL) == NULL, 
            "Did not check for Null");
    utest_assert(msg_env_queue_dequeue(q) == NULL, 
            "Empty queue did not return NULL on dequeue");
    msg_env_queue_destroy(q);
}

void test_is_empty()
{
    MsgEnv env, env2;
    msg_env_queue_t * q = msg_env_queue_create();
    utest_assert(msg_env_queue_is_empty(NULL) == ERROR_NULL_ARG,
            "Did not check for NULL");
    utest_assert(msg_env_queue_is_empty(q) == 1,
            "Is empty on an empty queue did not return 1");
    msg_env_queue_enqueue(q, &env);
    utest_assert(msg_env_queue_is_empty(q) == 0,
            "Is empty on a non-empty queue did not return 0");
    msg_env_queue_enqueue(q, &env2);
    utest_assert(msg_env_queue_is_empty(q) == 1,
            "Is empty on a non-empty queue of size 2 did not return 0");
    msg_env_queue_destroy(q);
}

void test_queue()
{
    msg_env_queue_t * q = msg_env_queue_create();
    MsgEnv msg_a = { NULL, 0, 0, 1, "a" };
    MsgEnv msg_b = { NULL, 0, 0, 1, "b" };
    MsgEnv msg_c = { NULL, 0, 0, 1, "c" };

    char results[] = { 'a', 'b', 'c' };

    msg_env_queue_enqueue(q, &msg_a);
    msg_env_queue_enqueue(q, &msg_b);
    msg_env_queue_enqueue(q, &msg_c);

    int i = 0;
    while (!msg_env_queue_is_empty(q))
    {
        MsgEnv* msg = msg_env_queue_dequeue(q);
        utest_assert(msg != NULL, "Dequeue returned NULL");
        utest_assert(i < 3, "Dequeue did not decrease the number of elements properly");
        utest_assert(msg->msg[0] == results[i], "Dequeue did not dequeue the right element");
        utest_assert(msg->next == NULL, "Dequeue did not unlink the dequeued's next pointer");
        i++;
    }

    msg_env_queue_destroy(q);
}

int main(int argc, char *argv[])
{
    test_create();
    test_enqueue_border_cases();
    test_dequeue_border_cases();
    test_queue();
    return utest_test_results(); 
}
