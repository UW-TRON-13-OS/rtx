#include "rtx.h"
#include "dbug.h"
#include "k_globals.h"
#include "msg_env_queue.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

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

void init_uart(void)
{
    UINT32 mask;

    /*
     * Store the serial ISR at user vector #64
     */
    asm( "move.l #asm_serial_entry,%d0" );
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
    asm( "move.l #kern_swi_entry, %d0" );
    asm( "move.l %d0,0x10000080" );
}

/*
 * initialises timer 
 */
void init_timer( void )
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

void init_msg_envs()
{
    free_env_q = msg_env_queue_create();
}

int main (void)
{
    /* Disable all interupts */
    asm( "move.w #0x2700,%sr" );

    coldfire_vbr_init();
    init_uart();
    init_kern_swi();

    /* Enable all interupts */
    asm( "move.w #0x2000,%sr" );

    return 0;
}
