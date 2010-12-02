#include "uart_i_process.h"
#include "rtx.h"
#include "k_primitives.h"
#include "trace.h"
#include "k_globals.h"
#include "proc_pq.h"
#include "k_hotkeys.h"

#define INPUT_BUFFER_SIZE 100

CHAR OutBuffer[IPC_MSG_ENV_MSG_SIZE] = {0};
CHAR InBuffer[INPUT_BUFFER_SIZE] = {0};
uint32_t inputIndex;
uint32_t outputIndex;
bool output_print_char;

/*
 * This function is called by the assembly STUB function
 */
void uart_i_process()
{
    int i;
    BYTE temp;
    temp = SERIAL1_UCSR;    // Ack the interrupt
    volatile BYTE CharIn = ' ';
        
    // There is data to be read
    if( temp & 1 )
    {
        CharIn = SERIAL1_RD;
        if (CharIn != '\0' && inputIndex < INPUT_BUFFER_SIZE - 2) // enter in a character
        {
            if (!hotkey(CharIn))
            {
                InBuffer[inputIndex] = CharIn;
                inputIndex++;
                SERIAL1_IMR = 3;
                SERIAL1_WD = CharIn;
                SERIAL1_IMR = 2;
            }
        }
        else if (CharIn == '\0') // enter key is pressed
        {
            SERIAL1_IMR = 3;
            SERIAL1_WD = '\n';
            SERIAL1_IMR = 2;
            InBuffer[inputIndex] = '\n';
            inputIndex++;
            InBuffer[inputIndex] = '\0';
            inputIndex++;
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
    }
    // Check to see if data can be written out
    else if ( temp & 4 )
    {
        if (outputIndex == 0 && output_print_char == FALSE)
        {
            MsgEnv* message = k_receive_message();
            if (message != NULL)
            {
                i = 0;
                while (message->msg[i] != '\0')
                {
                    OutBuffer[i] = message->msg[i];
                    i++;
                }
                OutBuffer[i] = '\0';
                output_print_char = TRUE;
            }
            k_release_msg_env(message);
        }
        if (output_print_char)
        {
            if (OutBuffer[outputIndex] == '\0')
            {
                outputIndex = 0;
                output_print_char = FALSE;
                SERIAL1_IMR = 2; // Disable tx interrupt
            }
            else
            {
                SERIAL1_WD = OutBuffer[outputIndex]; // Write data
                outputIndex++;
            }
        }
    }
    return;
}
