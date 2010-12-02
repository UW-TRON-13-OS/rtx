#include "uart_i_process.h"
#include "rtx.h"
#include "k_primitives.h"
#include "dbug.h"
#include "k_globals.h"
#include "proc_pq.h"

CHAR OutBuffer[100] = {0};
CHAR InBuffer[100] = {0};
uint32_t inputIndex;
uint32_t outputIndex;
bool output_print_char = FALSE;

/*
 * This function is called by the assembly STUB function
 */
void uart_i_process()
{
    enable_debug = 1;
    int i;
    BYTE temp;
    temp = SERIAL1_UCSR;    // Ack the interrupt
    volatile BYTE CharIn = ' ';
        
    // There is data to be read
    if( temp & 1 )
    {
        CharIn = SERIAL1_RD;
        if (CharIn != '\0') // enter in a character
        {
            if (CharIn == '`')
            {
                dbug(interrupted_process->name);
                proc_pq_print(ready_pq);
            }
            InBuffer[inputIndex] = CharIn;
            inputIndex++;
            SERIAL1_IMR = 3;
            SERIAL1_WD = CharIn;
            SERIAL1_IMR = 2;
        }
        else // enter key is pressed
        {
            SERIAL1_IMR = 3;
            SERIAL1_WD = '\n';
            SERIAL1_IMR = 2;
            InBuffer[inputIndex] = '\n';
            inputIndex++;
            InBuffer[inputIndex] = '\0';
            inputIndex++;
            MsgEnv* message = k_request_msg_env();
            dbug_ptr("request env ", message);
            if (message != NULL)
            {
                for (i = 0; i < inputIndex; i++)
                {
                    message->msg[i] = InBuffer[i];
                }
                message->msg_type = CONSOLE_INPUT;
                dbug("Sending message off to CCI");
                k_send_message(CCI_PID, message);
            }
            inputIndex = 0;
        }
    }
    // Check to see if data can be written out
    else if ( temp & 4 )
    {
        if (outputIndex == 0 && output_print_char == FALSE)
        {
            dbug("Check 5");
            MsgEnv* message = k_receive_message();
            dbug_ptr("message check ", message);
            if (message != NULL)
            {
                i = 0;
                while (message->msg[i] != '\0')
                {
                    OutBuffer[i] = message->msg[i];
                    i++;
                }
                output_print_char = TRUE;
            }
        }
        if (output_print_char)
        {
            if (OutBuffer[outputIndex] == '\0')
            {
                dbug("Check 6");
                SERIAL1_IMR = 2;        // Disable tx Interupt
                outputIndex = 0;
                output_print_char = FALSE;
            }
            else
            {
                dbug("Check 7");
                SERIAL1_WD = OutBuffer[outputIndex]; // Write data
                outputIndex++;
            }
        }
    }
    enable_debug = 0;
    return;
}
