#include "rtx.h"
#include "k_primitives.h"
#include "trap_table.h"
#include "k_clock.h"
#include "dbug.h"

int send_message(int dest_pid, MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d2, -(%a7)" );
    asm( "move.l %d3, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d2" );  // dest_pid
    asm( "move.l 12(%a6), %d3" ); // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_SEND_MESSAGE) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d3" );
    asm( "move.l (%a7)+, %d2" );

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
    asm( "move.l %d2, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d2" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_RELEASE_MSG_ENV) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d2" );

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
    asm( "move.l %d2, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d2" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_REQUEST_PROCESS_STATUS) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d2" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int terminate()
{
    int retCode = 0;

    //Call kernel primitive
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
    asm( "move.l %d2, -(%a7)" );
    asm( "move.l %d3, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d2" );  // new_priority
    asm( "move.l 12(%a6), %d3" ); // target_process_id

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_CHANGE_PRIORITY) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d3" );
    asm( "move.l (%a7)+, %d2" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d2, -(%a7)" );
    asm( "move.l %d3, -(%a7)" );
    asm( "move.l %d4, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d2" );  // time_delay
    asm( "move.l 12(%a6), %d3" ); // wakeup_code
    asm( "move.l 16(%a6), %d4" ); // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_CHANGE_PRIORITY) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d4" );
    asm( "move.l (%a7)+, %d3" );
    asm( "move.l (%a7)+, %d2" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int send_console_chars(MsgEnv *msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d2, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d2" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_SEND_CONSOLE_CHARS) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d2" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

int get_trace_buffers(MsgEnv* msg_env)
{
    int retCode = 0;

    // Save registers 
    asm( "move.l %d2, -(%a7)" );

    // Set the parameters
    asm( "move.l 8(%a6), %d2" );  // msg_env

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_GET_TRACE_BUFFERS) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Restore data registers
    asm( "move.l (%a7)+, %d2" );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );

    return retCode;
}

uint32_t get_system_time()
{
    int retCode = 0;

    // Call kernel primitive
    asm( "move.l #" STR(TRAP_TABLE_GET_SYSTEM_TIME) ", %d0" );
    asm( "TRAP #" STR(KERN_SWI) );

    // Retrieve the return code
    asm( "move.l %d0, -4(%a6)" );
    
    return retCode;
}

void kern_swi_handler()
{
    // Get the primitive number
    int primitive;
    MsgEnv* arg_msg;
    int arg_int1, arg_int2, ret_value;

    asm("move.l %%d0, %0" : "=m" (primitive));

    switch (primitive)
    {
        case TRAP_TABLE_SEND_MESSAGE:
            asm("move.l %%d2, %0" : "=m" (arg_int1));
            asm("move.l %%d3, %0" : "=m" (arg_msg));
            ret_value = k_send_message(arg_int1, arg_msg);
            break;
        case TRAP_TABLE_RECEIVE_MESSAGE:
            ret_value = (int) k_receive_message();
            break;
        case TRAP_TABLE_REQUEST_MSG_ENV:
            ret_value = (int) k_request_msg_env();
            break;
        case TRAP_TABLE_RELEASE_MSG_ENV:
            asm("move.l %%d2, %0" : "=m" (arg_msg));
            ret_value = k_release_msg_env(arg_msg);
            break;
        case TRAP_TABLE_RELEASE_PROCESSOR:
            ret_value = k_release_processor();
            break;
        case TRAP_TABLE_REQUEST_PROCESS_STATUS:
            asm("move.l %%d2, %0" : "=m" (arg_msg));
            ret_value = k_request_process_status(arg_msg);
            break;
        case TRAP_TABLE_TERMINATE:
            ret_value = k_terminate();
            break;
        case TRAP_TABLE_CHANGE_PRIORITY:
            asm("move.l %%d2, %0" : "=m" (arg_int1));
            asm("move.l %%d3, %0" : "=m" (arg_int2));
            ret_value = k_change_priority(arg_int1, arg_int2);
            break;
        case TRAP_TABLE_REQUEST_DELAY:
            asm("move.l %%d2, %0" : "=m" (arg_int1));
            asm("move.l %%d3, %0" : "=m" (arg_int2));
            asm("move.l %%d4, %0" : "=m" (arg_msg));
            ret_value = k_request_delay(arg_int1, arg_int2, arg_msg);
            break;
        case TRAP_TABLE_SEND_CONSOLE_CHARS:
            asm("move.l %%d2, %0" : "=m" (arg_msg));
            ret_value = k_send_console_chars(arg_msg);
            break;
        case TRAP_TABLE_GET_TRACE_BUFFERS:
            asm("move.l %%d2, %0" : "=m" (arg_msg));
            ret_value = k_get_trace_buffers(arg_msg);
            break;
        case TRAP_TABLE_GET_SYSTEM_TIME:
            asm("move.l %%d2, %0" : "=m" (arg_msg));
            ret_value = k_clock_get_system_time(arg_msg);
            break;
        default:
            rtx_dbug_outs("Illegal syscall\n");
            ret_value = -1;
    }

    // Finally push the return value back to d0
   asm("move.l %0, %d0" : : "m" (ret_value));
}
