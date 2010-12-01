#include "k_init.h"
#include "rtx.h"
#include "dbug.h"
#include "k_globals.h"
#include "msg_env_queue.h"

pcb_context_t main_context;

SINT32 coldfire_vbr_init( VOID )
{
    /*
     * Move the VBR into real memory
     *
     */
    asm( "move.l %a0, -(%a7)" );
    asm( "move.l #0x10000000, %a0 " );
    asm( "movec.l %a0, %vbr" );
    asm( "move.l (%a7)+, %a0" );

    return RTX_SUCCESS;
}

void init_uart()
{
    UINT32 mask;

    /*
     * Store the serial ISR at user vector #64
     */
    asm( "move.l #asm_uart_entry,%d0" );
    asm( "move.l %d0,0x10000100" );

    /* Reset the entire UART */
    SERIAL1_UCR = 0x10;

    /* Reset the receiver */
    SERIAL1_UCR = 0x20;

    /* Reset the transmitter */
    SERIAL1_UCR = 0x30;

    /* Reset the error condition */
    SERIAL1_UCR = 0x40;

    /* Install the interupt */
    SERIAL1_ICR = 0x17;
    SERIAL1_IVR = 64;

    /* enable interrupts on rx only */
    SERIAL1_IMR = 0x02;

    /* Set the baud rate */
    SERIAL1_UBG1 = 0x00;
#ifdef _CFSERVER_           /* add -D_CFSERVER_ for cf-server build */
    SERIAL1_UBG2 = 0x49;    /* cf-server baud rate 19200 */
#else
    SERIAL1_UBG2 = 0x92;    /* lab board baud rate 9600 */
#endif /* _CFSERVER_ */

    /* Set clock mode */
    SERIAL1_UCSR = 0xDD;

    /* Setup the UART (no parity, 8 bits ) */
    SERIAL1_UMR = 0x13;

    /* Setup the rest of the UART (noecho, 1 stop bit ) */
    SERIAL1_UMR = 0x07;

    /* Setup for transmit and receive */
    SERIAL1_UCR = 0x05;

    /* Enable interupts */
    mask = SIM_IMR;
    mask &= 0x0003dfff;
    SIM_IMR = mask;
}

void init_kern_swi()
{
    // TRAP #0
    asm( "move.l #asm_kern_swi, %d0" );
    asm( "move.l %d0,0x10000080" );
}

/*
 * initialises timer 
 */
void init_timer()
{
    UINT32 mask;

    /*
     * Store the timer ISR at auto-vector #6
     */
    asm( "move.l #asm_timer_entry,%d0" );
    asm( "move.l %d0,0x10000078" );

    /*
     * Setup to use auto-vectored interupt level 6, priority 3
     */
    TIMER0_ICR = 0x9B;
    // 1_00_110_11
    // autovector enable_(clear)_interrupt level_interrupt priority

    /*
     * Set the reference counts, ~10ms
     */
    TIMER0_TRR = 1758;

    /*
     * Setup the timer prescaler and stuff
     */
    TIMER0_TMR = 0xFF1B;
    //11111111_00_0_1_1_01_1
    //prescale_disable interrupt on capture event_output mode
    //enable interrupt on reaching ref_restart on ref value
    //set input clock as sysbus div 1_enable timer

    /*
     * Set the interupt mask
     */
    mask = SIM_IMR;
    mask &= 0x0003fdff;
    SIM_IMR = mask;    
}

void init_ipc()
{
#define BUF_SIZE = (sizeof(*send_trace_buf.buf) * IPC_MESSAGE_TRACE_HISTORY_SIZE)
    free_env_q = msg_env_queue_create();
    send_trace_buf.tail = 0;
    send_trace_buf.buf = k_malloc(sizeof(*send_trace_buf.buf) * 
                                  IPC_MESSAGE_TRACE_HISTORY_SIZE);
    recv_trace_buf.tail = 0;
    recv_trace_buf.buf = k_malloc(sizeof(*recv_trace_buf.buf) * 
                                  IPC_MESSAGE_TRACE_HISTORY_SIZE);
}

int k_init(pcb_init_t process_init[], uint32_t num_processes)
{
    /* Disable all interupts */
 //   asm("move.w #0x2700,%sr");

    dbug("Initializing vbr...");
    coldfire_vbr_init();
    dbug("Initializing uart...");
    init_uart();
    dbug("Initializing kern swi...");
    init_kern_swi();
    dbug("Initializing kern ipc...");
    init_ipc();

    dbug("Initializing priority queues...");
    ready_pq = proc_pq_create(NUM_PRIORITIES + ADDITIONAL_NULL_PRIORITY);
    blocked_request_env_pq = proc_pq_create(NUM_PRIORITIES);
    rtx_dbug_outs("ready_pq ");
    rtx_dbug_uint((uint32_t)ready_pq);
    dbug("");
    rtx_dbug_outs("blocked_request_env_pq ");
    rtx_dbug_uint((uint32_t)blocked_request_env_pq);
    dbug("");

    // Initialize all of the processes
    dbug("Initializing processes...");
    init_processes(process_init, num_processes);

    /* Enable all interupts */
//    asm("move.w #0x2000,%sr");

    rtx_dbug_outs("Dequeueing ");
    current_process = proc_pq_dequeue(ready_pq);
    current_process->state = P_EXECUTING;
    //dbug(current_process->name);
    dbug("Starting OS!");
    k_context_switch(&main_context, &current_process->context);

    rtx_dbug_outs("SHOULD NOT ARRIVE HERE\r\n");

    return 0;
}
