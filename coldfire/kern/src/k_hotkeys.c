#include "k_hotkeys.h"
#include "k_globals.h"
#include "k_mem.h"
#include "trace.h"
#include "dbug.h"

bool hotkey(char c)
{
    int i;
    char buf[128];
    switch(c)
    {
        case '`':
            proc_pq_print(ready_pq);
            proc_pq_print(blocked_request_env_pq);
            for (i = 0; i < k_get_num_processes(); i++)
            {
                pcb_t * pcb = k_get_process(pid_table[i]);
                switch (pcb->state)
                {
                    case P_EXECUTING:
                        trace_str(ALWAYS, pcb->name, "  executing");
                        break;
                    case P_BLOCKED_ON_ENV_REQUEST:
                        trace_str(ALWAYS, pcb->name, "  blocked on request env");
                        break;
                    case P_BLOCKED_ON_RECEIVE:
                        trace_str(ALWAYS, pcb->name, "  blocked on receive");
                        break;
                    default:
                        break;
                }
            }
            return 1;
       case '!':
            for (i = 0; i < k_get_num_processes(); i++)
            {
                pcb_t * pcb = k_get_process(pid_table[i]);
                trace_str(ALWAYS, "msg env queue of ", pcb->name);
                trace_inline(ALWAYS, "     -> ");
                msg_env_queue_print(pcb->recv_msgs);
            }
            return 1;
       case '@':
            trace(ALWAYS, "Memory summary");
            trace_uint(ALWAYS, "    Available: ", k_mem_get_available());
            trace_uint(ALWAYS, "    Used: ", k_mem_get_used());
            for (i = 0; i < k_get_num_processes(); i++)
            {
                pcb_t * pcb = k_get_process(pid_table[i]);
                trace_inline(ALWAYS, "    ");
                trace_inline(ALWAYS, pcb->name);
                trace_uint(ALWAYS, " stack space : ", k_mem_get_stack_left(pcb->pid));
            }
            return 1;
    }
    return 0;
}
