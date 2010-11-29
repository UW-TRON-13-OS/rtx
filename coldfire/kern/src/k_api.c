#include "rtx.h"
#include "trap_table.h"

int send_message(int dest_pid, MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );
    asm( "move.l %d2, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // dest_pid
    asm( "move.l 12(%a6), %d2" ); // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_SEND_MESSAGE) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d2" );
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

MsgEnv * receive_message()
{
    MsgEnv *ret = NULL;

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_RECEIVE_MESSAGE) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Retrieve the return message
    asm( "move.l %d0, -4(%a6)" );

    return ret;
}

MsgEnv * request_msg_env()
{
    MsgEnv *ret = NULL;

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_REQUEST_MSG_ENV) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Retrieve the return message
    asm( "move.l %d0, -4(%a6)" );

    return ret;
}

int release_msg_env(MsgEnv * msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_RELEASE_MSG_ENV) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int release_processor()
{
    int retCode = 0;

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_RELEASE_PROCESSOR) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );
    
    return retCode;
}

int request_process_status(MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_REQUEST_PROCESS_STATUS) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int terminate()
{
    int retCode = 0;

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_TERMINATE) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );
    
    return retCode;
}

int change_priority(int new_priority, int target_process_id)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );
    asm( "move.l %d2, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // new_priority
    asm( "move.l 12(%a6), %d2" ); // target_process_id

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_CHANGE_PRIORITY) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d2" );
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );
    asm( "move.l %d2, -(%a7)" );
    asm( "move.l %d3, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // time_delay
    asm( "move.l 12(%a6), %d2" ); // wakeup_code
    asm( "move.l 16(%a6), %d3" ); // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_CHANGE_PRIORITY) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d3" );
    asm( "move.l (%a7)+, %d2" );
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int send_console_chars(MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_SEND_CONSOLE_CHARS) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int get_console_chars(MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_GET_CONSOLE_CHARS) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int get_trace_buffers(MsgEnv* msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d1, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d1" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_GET_TRACE_BUFFERS) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d1" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

void kern_swi_handler()
{
}
