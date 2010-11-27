#include "utest.h"
#include "timeout_queue.h"
#include "rtx.h"
#include <stdlib.h>

void test_create()
{
	MsgEnv* msg_env = NULL;
    timeout_queue_insert(msg_env);
    utest_assert(get_timeout_queue() == NULL, "Timeout Queue was not Null");
}

void test_queue()
{
    MsgEnv msg_a = { NULL, 0, 0, 1, (char *)malloc(100) };
    MsgEnv msg_b = { NULL, 0, 0, 1, (char *)malloc(100) };
    MsgEnv msg_c = { NULL, 0, 0, 1, (char *)malloc(100) };

	*((int *) msg_a.msg) = 100;
	*((int *) msg_b.msg) = 1000;
	*((int *) msg_c.msg) = 10;
	
    timeout_queue_insert(&msg_a);
    timeout_queue_insert(&msg_b);
    timeout_queue_insert(&msg_c);

	MsgEnv* queue = get_timeout_queue();
	
    while (queue->next != NULL)
    {
		int chk1 = *((int *) queue->msg) == 10 + 1;
        utest_assert(chk1, "The first block is not 10");
		
		queue = queue->next;
		int chk2 = *((int *) queue->msg) == 100 + 1;
		utest_assert(chk2, "The first block is not 100");
		
		queue = queue->next;
		int chk3 = *((int *) queue->msg) == 1000 + 1;
		utest_assert(chk3, "The first block is not 1000");
    }
}

void test_check()
{
	MsgEnv msg_a = { NULL, 0, 0, 1, (char *)malloc(100) };
    MsgEnv msg_b = { NULL, 0, 0, 1, (char *)malloc(100) };
    MsgEnv msg_c = { NULL, 0, 0, 1, (char *)malloc(100) };

	*((int *) msg_a.msg) = 100;
	*((int *) msg_b.msg) = 1000;
	*((int *) msg_c.msg) = 10;
	
    timeout_queue_insert(&msg_a);
    timeout_queue_insert(&msg_b);
    timeout_queue_insert(&msg_c);

	k_set_system_time(500);
	
	MsgEnv msg_a2 = { NULL, 0, 0, 1, (char *)malloc(100) };

	MsgEnv* msg_env = check_timeout_queue(&msg_a2);
	
	int chk1 = msg_env != NULL;
    utest_assert(chk1, "The msg env is NULL");
	
	if(msg_env)
	{
		int chk2 = *((int *) msg_env->msg) == 10 + 1;
		utest_assert(chk2, "The first block is not 10 + 1");	
		
		MsgEnv* msg_env2 = check_timeout_queue(&msg_a2);
		int chk3 = *((int *) msg_env2->msg) == 100 + 1;
		utest_assert(chk3, "The first block is not 100 + 1");		
		
		MsgEnv* msg_env3 = check_timeout_queue(&msg_a2);
		int chk4 = msg_env3 == NULL;
		utest_assert(chk4, "The first block is not NULL");
	}
}

int main(int argc, char *argv[])
{
    //test_create();
    //test_queue();
	test_check();
    utest_test_results();
}
