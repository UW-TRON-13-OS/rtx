#include "uart_i_process.h"
#include "rtx.h"
#include "k_primitives.h"
#include "dbug.h"
#include "k_globals.h"

CHAR OutBuffer[100] = {0};
CHAR InBuffer[100] = {0};
uint32_t inputIndex;
uint32_t outputIndex;

/*
 * This function is called by the assembly STUB function
 */
void uart_i_process()
{
    dbug("Checking if this gets entered into");
    int i;
    BYTE temp;
    temp = SERIAL1_UCSR;    // Ack the interrupt
    volatile BYTE CharIn = ' ';
        
    // There is data to be read
    if( temp & 1 )
    {
        dbug("Check if it detects kb input");
        CharIn = SERIAL1_RD;
        rtx_dbug_out_char(CharIn);
        dbug_uint("inputIndex", inputIndex);
        InBuffer[inputIndex] = CharIn;
        inputIndex++;
        SERIAL1_IMR = 3;
        if (CharIn != '\0') // enter in a character
        {
            dbug("Check 0");
            SERIAL1_WD = CharIn;
            dbug("Check 1");
        }
        else // enter key is pressed
        {
            SERIAL1_WD = '\n';
            MsgEnv* message = k_request_msg_env();
            if (message != NULL)
            {
                for (i = 0; i < inputIndex; i++)
                {
                    message->msg[i] = InBuffer[i];
                }
                message->msg_type = CONSOLE_INPUT;
                k_send_message(CCI_PID, message);
            }
            inputIndex = 0;
        }
        dbug("Check 2");
    }
    // Check to see if data can be written out
    else if ( temp & 4 )
    {
        dbug("Check 3");
        if (outputIndex == 0)
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
        }
        dbug("Check 4");
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
    dbug("End Check");
    return;
}
