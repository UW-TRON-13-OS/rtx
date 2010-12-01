#include "k_structs.h"
#include "k_globals.h"

pcb_t * current_process;
static num_processes = 0;
static pcb_t *p_table = NULL;

pcb_t * k_get_process(int32_t pid)
{
    return NULL;
}

int32_t k_get_num_processes()
{
    return 0;
}

void init_fake_stack_frame(pcb_t * pcb, bool is_sys_process)
{
#define FORMAT 0x40000000  // Assume 'a7' was previous aligned on 0 modulo 4
#define FS_3_2 0x0     // No fault
#define Vector_7_0 (32<<(2+16))   // Trap interrupt
#define FS_1_0 0x0     // No fault
#define STATUS_REG 0x70 // Interrupt 7 - cleared CSR
#define USER_MODE 0x0000
#define SUPERVISOR_MODE 0x2000
#define FAKE_FV_STATUS_CONSTANT (FORMAT|FS_3_2|Vector_7_0|FS_1_0|STATUS_REG)
    uint32_t * fake_stack_ptr = pcb->stack_bottom;
    *fake_stack_ptr-- = (uint32_t) pcb->start;
    if (is_sys_process)
    {
        *fake_stack_ptr = FAKE_FV_STATUS_CONSTANT | SUPERVISOR_MODE;
    }
    else
    {
        *fake_stack_ptr = FAKE_FV_STATUS_CONSTANT | USER_MODE;
    }
    pcb->context.stack_ptr = *fake_stack_ptr;
}

void init_processes(pcb_init_t process_init[], uint32_t num_processes)
{
    p_table = k_malloc(sizeof(*p_table) * num_processes);
    int process_num;
    for (process_num = 0; process_num < num_processes; process_num++)
    {
        pcb_t * process = &p_table[process_num];
        pcb_init_t * init = &process_init[process_num];

        process->next = NULL;
        process->pid = init->pid;
        process->name = init->name;
        process->priority = init->priority;
        process->state = P_READY;
        process->is_i_process = init->is_i_process;
        process->recv_msgs = msg_env_queue_create();
        process->start = init->start;
        process->stack_begin = k_malloc(init->stack_size);
        byte * stack_boundary = process->stack_begin + init->stack_size;
        process->stack_bottom = (uint32_t *) stack_boundary - 1;
        init_fake_stack_frame(process, init->is_sys_process);
    }
}

