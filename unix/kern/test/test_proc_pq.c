#include "utest.h"
#include "proc_pq.h"
#include "k_process.h"

void test_create()
{
    proc_pq_t *q = proc_pq_create(4);
    utest_assert(q != NULL, "Create returned NULL");
    utest_assert(proc_pq_get_num_prorities(q) == 4, "Did not set num of priorities");
    proc_pq_destroy(q);
    q = proc_pq_create(0);
    utest_assert(q == NULL, "0 priority pq did not return NULL");
    utest_assert(proc_pq_get_num_prorities(NULL) == -1, "Did not set num of priorities");
}

void test_enqueue_boundary_cases()
{
    pcb_t pcb;
    proc_pq_t *q = proc_pq_create(4);
    utest_assert(proc_pq_enqueue(NULL, NULL) == ERROR_NULL_ARG, "Enqueue did not handle NULL,NULL");
    utest_assert(proc_pq_enqueue(q, NULL) == ERROR_NULL_ARG, "Enqueue did not handle q,NULL");
    utest_assert(proc_pq_enqueue(NULL, &pcb) == ERROR_NULL_ARG, "Enqueue did not handle NULL,pcb");
    proc_pq_destroy(q);
}

void test_dequeue_boundary_cases()
{
    proc_pq_t *q = proc_pq_create(4);
    utest_assert(proc_pq_dequeue(NULL) == NULL, "Dequeue did not handle NULL");
    utest_assert(proc_pq_dequeue(q) == NULL, "Dequeue did not return NULL for empty queue");
    proc_pq_destroy(q);
}

void test_remove_boundary_cases()
{
    pcb_t pcb;
    proc_pq_t *q = proc_pq_create(4);
    utest_assert(proc_pq_remove(NULL, NULL) == NULL, "Remove did not handle NULL,NULL");
    utest_assert(proc_pq_remove(q, NULL) == NULL, "Remove did not handle q,NULL");
    utest_assert(proc_pq_remove(NULL, &pcb) == NULL, "Remove did not handle NULL,pcb");
    proc_pq_destroy(q);
}

void test_queue()
{
    proc_pq_t *q = proc_pq_create(4);
    
#define N_PCBS 9
    pcb_t pcbs[] = { { NULL, 0, "a", 3 }, { NULL, 0, "b", 0 }, { NULL, 0, "c", 2 }, 
                     { NULL, 0, "d", 1 }, { NULL, 0, "e", 0 }, { NULL, 0, "f", 3 },
                     { NULL, 0, "g", 1 }, { NULL, 0, "h", 0 }, { NULL, 0, "i", 3 } };
    int i;
    for (i = 0; i < N_PCBS; i++)
    {
        utest_assert(proc_pq_enqueue(q, &pcbs[i]) == CODE_SUCCESS, "Inserting pcb did not succed");
    }

    char results[] = "begcfi";
    utest_assert(proc_pq_remove(q, &pcbs[3]) == &pcbs[3], "Did not remove 'd' proc properly");
    utest_assert(proc_pq_remove(q, &pcbs[7]) == &pcbs[7], "Did not remove 'h' proc properly");
    utest_assert(proc_pq_remove(q, &pcbs[0]) == &pcbs[0], "Did not remove 'a' proc properly");
    for (i = 0; i < N_PCBS-3; i++)
    {
        pcb_t *proc = proc_pq_dequeue(q);
        utest_assert(proc != NULL, "Dequeue didn't dequeue the proper pcb, gave null");
        utest_assert(proc->name[0] == results[i], 
                "Dequeue didn't dequeue the proper pcb, gave wrong pcb");
        utest_assert(proc->next == NULL, "Dequeue did not unlink dequeued pcb");
    }
    utest_assert(proc_pq_dequeue(q) == NULL, "Did not return NULL on an empty queue");

    proc_pq_destroy(q);
}

int main(int argc, char *argv[])
{
    test_create();
    test_enqueue_boundary_cases();
    test_dequeue_boundary_cases();
    test_queue();
    utest_test_results(); 
}
