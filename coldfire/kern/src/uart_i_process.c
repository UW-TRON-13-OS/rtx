#include "uart_i_process.h"
#include "rtx.h"
#include "k_primitives.h"
#include "msg_env_queue.h"
#include "dbug.h"
#include "k_globals.h"

volatile BYTE CharOut = '\0';
CHAR OutBuffer[100] = {0};
CHAR InBuffer[100] = {0};
int inputIndex = 0;
int outputIndex = 0;
msg_env_queue_t* input_queue;
msg_env_queue_t* output_queue;

/*
 * This function is called by the assembly STUB function
 */
void uart_i_process()
{
    int i;
    BYTE temp;
    temp = SERIAL1_UCSR;    // Ack the interrupt
    volatile BYTE CharIn = ' ';
    if (input_queue == NULL)
    {
        input_queue = msg_env_queue_create();
    }
    if (output_queue == NULL)
    {
        output_queue = msg_env_queue_create();
    }
    
    MsgEnv* message = k_receive_message();
    if (message != NULL)
    {
        if (message->msg_type == CONSOLE_INPUT)
        {
            msg_env_queue_enqueue(input_queue, message);
        }
        else if (message->msg_type == CONSOLE_INPUT)
        {
            msg_env_queue_enqueue(output_queue, message);
        }
        else
        {
            rtx_dbug_outs("ERROR"); // should never get here
        }
    }

    // There is data to be read
    if( temp & 1 )
    {
        CharIn = SERIAL1_RD;
        InBuffer[inputIndex] = CharIn;
        inputIndex++;
        SERIAL1_IMR = 3;
        if (CharIn != '\0') // enter in a character
        {
            SERIAL1_WD = CharOut;
        }
        else // enter key is pressed
        {
            SERIAL1_WD = '\n';
            message = msg_env_queue_dequeue(input_queue);
            if (message != NULL)
            {
                for (i = 0; i < inputIndex; i++)
                {
                    message->msg[i] = InBuffer[i];
                }
                k_send_message(CCI_PID, message);
            }
            inputIndex = 0;
        }
    }
    // Check to see if data can be written out
    else if ( temp & 4 )
    {
        if (outputIndex == 0)
        {
            message = msg_env_queue_dequeue(output_queue);
            if (message != NULL)
            {
                i = 0;
                while (message->msg[i] != '\0')
                {
                    OutBuffer[i] = message->msg[i];
                }
            }
        }
        
        if (OutBuffer[outputIndex] == '\0')
        {
            SERIAL1_IMR = 2;        // Disable tx Interupt
            outputIndex = 0;
        }
        else
        {
            SERIAL1_WD = OutBuffer[outputIndex]; // Write data
            outputIndex++;
        }
    }
    return;
}
