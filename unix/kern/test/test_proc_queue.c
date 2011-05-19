#include "utest.h"
#include "proc_queue.h"
#include "k_process.h"
#include "rtx.h"

void test_create()
{
    proc_queue_t * q = proc_queue_create();
    utest_assert(q != NULL, "Process Queue was Null");
}

void test_enqueue_border_cases()
{
    pcb_t pcb;
    proc_queue_t * q = proc_queue_create();
    utest_assert(proc_queue_enqueue(NULL, NULL) == ERROR_NULL_ARG, 
            "Did not check for Both Null");
    utest_assert(proc_queue_enqueue(q, NULL) == ERROR_NULL_ARG, 
            "Did not check for Null pcb");
    utest_assert(proc_queue_enqueue(NULL, &pcb) == ERROR_NULL_ARG, 
            "Did not check for Null queue");
    proc_queue_destroy(q);
}

void test_dequeue_border_cases()
{
    proc_queue_t * q = proc_queue_create();
    utest_assert(proc_queue_dequeue(NULL) == NULL, 
            "Did not check for Null");
    utest_assert(proc_queue_dequeue(q) == NULL, 
            "Empty queue did not return NULL on dequeue");
    proc_queue_destroy(q);
}

void test_is_empty()
{
    pcb_t p1, p2;
    proc_queue_t * q = proc_queue_create();
    utest_assert(proc_queue_is_empty(NULL) == ERROR_NULL_ARG,
            "Did not check for NULL");
    utest_assert(proc_queue_is_empty(q) == 1,
            "Is empty on an empty queue did not return 1");
    proc_queue_enqueue(q, &p1);
    utest_assert(proc_queue_is_empty(q) == 0,
            "Is empty on a non-empty queue did not return 0");
    proc_queue_enqueue(q, &p2);
    utest_assert(proc_queue_is_empty(q) == 1,
            "Is empty on a non-empty queue of size 2 did not return 0");
    proc_queue_destroy(q);
}

void test_remove()
{
    pcb_t p1;
    proc_queue_t * q = proc_queue_create();

    utest_assert(proc_queue_remove(NULL, NULL) == NULL, 
            "Did not check for Both Null");
    utest_assert(proc_queue_remove(q, NULL) == NULL, 
            "Did not check for Null env in remove");
    utest_assert(proc_queue_remove(NULL, &p1) == NULL, 
            "Did not check for Null queue in remove");
    utest_assert(proc_queue_remove(q, &p1) == NULL, 
            "Did not return NULL for empty queue on remove");

    proc_queue_destroy(q);
}

void test_queue()
{
    proc_queue_t * q = proc_queue_create();
    pcb_t proc_a = { NULL, 0, "a" };
    pcb_t proc_b = { NULL, 0, "b" };
    pcb_t proc_c = { NULL, 0, "c" };
    pcb_t proc_d = { NULL, 0, "d" };
    pcb_t proc_e = { NULL, 0, "e" };

    char results[] = { 'a', 'b', 'd' };

    proc_queue_enqueue(q, &proc_a);
    proc_queue_enqueue(q, &proc_b);
    proc_queue_enqueue(q, &proc_c);
    proc_queue_enqueue(q, &proc_d);

    utest_assert(proc_queue_remove(q, &proc_e) == NULL, "Did not return NULL for non existant process");
    utest_assert(proc_queue_remove(q, &proc_c) == &proc_c, "Did not return proc c for remove");

    int i = 0;
    while (!proc_queue_is_empty(q))
    {
        pcb_t* proc = proc_queue_dequeue(q);
        utest_assert(proc != NULL, "Dequeue returned NULL");
        utest_assert(i < 3, "Dequeue did not decrease the number of elements properly");
        utest_assert(proc->name[0] == results[i], "Dequeue did not dequeue the right element");
        utest_assert(proc->next == NULL, "Dequeue did not unlink the dequeued's next pointer");
        i++;
    }

    proc_queue_destroy(q);
}

int main(int argc, char *argv[])
{
    test_create();
    test_enqueue_border_cases();
    test_dequeue_border_cases();
    test_queue();
    utest_test_results(); 
}
