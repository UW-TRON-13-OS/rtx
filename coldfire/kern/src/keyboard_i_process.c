#include "rtx.h"
#include "dbug.h"
#include "k_primitives.h"

/*
 * Global Variables
 */
volatile byte CharIn = '!';
volatile bool Caught = TRUE;
volatile byte CharOut = '\0';
char inputString[1000] = {0};
int cur = 0;

/*
 * This function is called by the assembly STUB function
 */
void c_serial_handler( void )
{
    byte temp;

    temp = SERIAL1_UCSR;    /* Ack the interrupt */

#ifdef _DEBUG_
    rtx_dbug_outs((CHAR *) "Enter: c_serial_handler ");
#endif /* _DEBUG */
    
    /* See if data is waiting.... */    
    if( temp & 1 )
    {
#ifdef _DEBUG_
        rtx_dbug_outs((CHAR *) "reading data: ");
        rtx_dbug_out_char(CharIn);
    	rtx_dbug_outs((CHAR *) "\r\n");
#endif /* _DEBUG */
        CharIn = SERIAL1_RD;
        Caught = FALSE;
    }

    /* See if port is ready to accept data */    
    else if ( temp & 4 )
    {
#ifdef _DEBUG_
        rtx_dbug_outs((CHAR *) "writing data: ");
        rtx_dbug_out_char(CharOut);
    	rtx_dbug_outs((CHAR *) "\r\n");
#endif /* _DEBUG_*/
        SERIAL1_WD = CharOut;   /* Write data to port */
        SERIAL1_IMR = 2;        /* Disable tx Interupt */
    }
    return;
}

// outputs things into the monitor
/*SINT32 serial_print(CHAR* str)
{
    BYTE temp;
    if ( str == NULL )
    {
        return RTX_ERROR;
    }
    SERIAL1_IMR = 3;
    while ( *str != '\0' )
    {
    	temp = SERIAL1_UCSR;
        while (!(temp & 4))
	{
	    temp = SERIAL1_UCSR;
	}
	SERIAL1_WD = *str++;
    }
    SERIAL1_IMR = 2;
    return RTX_SUCCESS;
}*/

void start_kb_i_process()
{
    // Enable interrupts
    asm( "move.w #0x2000,%sr" );
    int i;
    while (1)
    {
        MsgEnv* message = k_receive_message();
        if (message != NULL)
        {
            for (i = 0; i < kb_buf->length; i++)
            {
                message->msg[i] = kb_buf->data[i];
            }
            message->msg[i] = '\0';
            message->msg_type = CONSOLE_INPUT;
            kb_buf->kb_wait_flag = KB_FLAG_FREE;
            k_send_message(message->send_pid, message);
        }
        kb_buf->length = 0;
        k_i_process_exit();
    }

    /* Busy Loop */
    while(1)
    {
        if( !Caught )
        {
            Caught = TRUE;
            CharOut = CharIn;
            if (CharIn != '\0' &&  CharIn != '+')
            {
                /* Nasty hack to get a dynamic string format,
                 * grab the character before turning the interrupts back on.
                 */
                inputString[cur] = CharIn;
                cur++;
                /* enable tx interrupts  */
                SERIAL1_IMR = 3;
            }
            else
            {
                /* Now print the string to debug,
                 * note that interrupts are now back on.
                 */
                inputString[cur] = '\n';
                inputString[cur+1] = '\0';
                rtx_dbug_outs( inputString );
                cur = 0;
            }
        }
    } 
    /* Disable all interupts */
    asm( "move.w #0x2700,%sr" );
}

