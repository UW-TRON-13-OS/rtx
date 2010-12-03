#include "k_structs.h"
#include "k_globals.h"
#include "trace.h"

#define STACK_OFFSET 16

pcb_t * current_process;
static uint32_t num_processes;
static pcb_t *p_table;
int32_t *pid_table;

pcb_t * k_get_process(int32_t pid)
{
    int i = 0;
    for (i = 0; i < num_processes; i++)
    {
        if (p_table[i].pid == pid)
        {
            return &p_table[i];
        }
    }
    return NULL;
}

int32_t k_get_num_processes()
{
    return num_processes;
}

void init_fake_stack_frame(pcb_t * pcb, bool is_sys_process)
{
#define FAKE_ISR_USER 0x456720000
#define FAKE_ISR_SUPERVISOR 0x45672000
#define FAKE_ISR_INTERRUPT 0x456727000
    uint32_t * fake_stack_ptr = pcb->stack_bottom;
    *fake_stack_ptr = (uint32_t) pcb->start;
    trace_ptr(TRACE, "fake stack ptr ", fake_stack_ptr);
    trace_hex(TRACE, "getting start address ", *fake_stack_ptr);
    fake_stack_ptr--;
    if (pcb->is_i_process)
    {
        *fake_stack_ptr = 0x45672700;
    }
    else if (is_sys_process)
    {
        *fake_stack_ptr = 0x45672000;
    }
    else
    {
        *fake_stack_ptr = 0x45672000;
    }
    trace_ptr(TRACE, "Exception Stack frame ", fake_stack_ptr);
    trace_hex(TRACE, "          ISR ", fake_stack_ptr[0]);
    trace_hex(TRACE, "          PC ", fake_stack_ptr[1]);

    // Fake some registers
    int i;
    for (i = 0; i < 8; i++)
    {
        *(--fake_stack_ptr) = 0; // d#i
    }
    for (i = 0; i < 7; i++)
    {
        *(--fake_stack_ptr) = 0; // a#i
    }
    pcb->context.stack_ptr = (uint32_t) fake_stack_ptr;
}

void init_processes(pcb_init_t process_init[], uint32_t num_procs)
{
    trace_uint(TRACE, "Entered init processes. num_proces ", num_procs);
    p_table = k_malloc(sizeof(*p_table) * num_procs);
    pid_table = k_malloc(sizeof(*pid_table) * num_procs);
    trace_uint(TRACE, "  p_table ", (uint32_t) p_table);
    trace_uint(TRACE, "  size of pcb_t ", sizeof(pcb_t));
    trace_uint(TRACE, "  num_procs  ", num_procs);
    num_processes = num_procs;
    int process_num;
    for (process_num = 0; process_num < num_procs; process_num++)
    {
        pcb_t * process = &p_table[process_num];
        pcb_init_t * init = &process_init[process_num];
        pid_table[process_num] = init->pid;

        process->next = NULL;
        process->pid = init->pid;
        process->name = init->name;
        process->priority = init->priority;
        process->state = P_READY;
        process->is_i_process = init->is_i_process;
        process->is_sys_process = init->is_sys_process || init->is_i_process;
        process->recv_msgs = msg_env_queue_create();
        process->start = init->start;
        process->stack_begin = k_malloc(init->stack_size);
        byte * stack_boundary = process->stack_begin + init->stack_size;
        process->stack_bottom = (uint32_t *) (stack_boundary - STACK_OFFSET);
        process->first_time = 1;
        process->env_owned = 0;

        trace(TRACE, process->name);
        trace_ptr(TRACE, " stack boundary ", stack_boundary);

        // clear the bottom of the stack
        byte * ptr = (byte *) process->stack_bottom;
        while (ptr < stack_boundary)
        {
            *ptr++ = 0;
        }

        init_fake_stack_frame(process, init->is_sys_process);

        if (!process->is_i_process)
        {
            proc_pq_enqueue(ready_pq, process);
        }
    }
}

