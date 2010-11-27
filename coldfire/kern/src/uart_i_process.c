#include "uart_i_process.h"
#include "rtx.h"
#include "k_primitives.h"
#include "msg_env_queue.h"
#include "dbug.h"

volatile BYTE CharIn = ' ';
volatile BYTE CharOut = '\0';
volatile BOOLEAN Caught = TRUE;
CHAR OutBuffer[100] = {0};
CHAR InBuffer[100] = {0};
int inputIndex = 0;
int outputIndex = 0;

/*
 * This function is called by the assembly STUB function
 */
void start_uart_i_process()//VOID c_serial_handler( VOID )
{
    int i;
    BYTE temp;
    temp = SERIAL1_UCSR;    // Ack the interrupt

    // There is data to be read
    if( temp & 1 )
    {
        CharIn = SERIAL1_RD;
        Caught = FALSE;
        if (CharIn != '\0') // enter in a character
        {
            InBuffer[inputIndex] = CharIn;
            inputIndex++;
            SERIAL1_IMR = 3;
            SERIAL1_WD = CharOut;
        }
        else // enter key is pressed
        {
            InBuffer[inputIndex] = CharIn;
            inputIndex++;
            SERIAL1_IMR = 3;
            SERIAL1_WD = '\n';
            MsgEnv* message = k_request_msg_env();
            message->msg_type = CONSOLE_INPUT;
            for (i = 0; i < inputIndex; i++)
            {
                message->msg[i] = InBuffer[i];
            }
            k_send_message(CCI_PID, message);
            inputIndex = 0;
        }
    }
    // Check to see if data can be written out
    else if ( temp & 4 )
    {
        MsgEnv* message = k_receive_message();
        if (message != NULL)
        {
            i = 0;
            while (message->msg[i] != '\0')
            {
                OutBuffer[i] = message->msg[i];
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