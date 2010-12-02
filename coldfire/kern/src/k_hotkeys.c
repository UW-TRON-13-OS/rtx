#include "k_hotkeys.h"
#include "k_globals.h"
#include "trace.h"
#include "dbug.h"

bool hotkey(char c)
{
    int i;
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
    }
    return 0;
}
