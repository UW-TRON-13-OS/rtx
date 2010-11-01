#include "k_init.h"
#include "k_config.h"
#include "k_process.h"
#include "k_storage.h"
#include "k_ipc.h"
#include "rtx.h"

void k_init()
{
    k_ipc_init();
    k_storage_init();

    proc_cfg_t init_table[NUM_PROCESSES] = { };
    k_init_processes(init_table);
}
