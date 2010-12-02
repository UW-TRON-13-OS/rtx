/**
 * @file: dbug.c
 * @brief: output to janusROM terminal by using trap #15
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2010/05/03
 */

#include "dbug.h"

/**
 * @brief: C Function wrapper for TRAP #15 function to output a character
 * @param: c the charcter to output to janusROM  
 */

bool enable_debug = 0;

void rtx_dbug_out_char( char c )
{
    if (!enable_debug)
    {
        return;
    }
	
    /* Store registers */
    asm( "move.l %d0, -(%a7)" );
    asm( "move.l %d1, -(%a7)" );

    /* Load CHAR c into d1 */
    asm( "move.l 8(%a6), %d1" );
    
    /* Setup trap function */
    asm( "move.l #0x13, %d0" );
    asm( "trap #15" );

    /* Restore registers  */
    asm(" move.l (%a7)+, %d1" );
    asm(" move.l (%a7)+, %d0" );
}


/**
 * @brief: Prints a C-style null terminated string
 * @param: s the string to output to janusROM terminal 
 */
int32_t rtx_dbug_outs( char* s )
{
    if ( s == NULL )
    {
        return RTX_ERROR;
    }
    while ( *s != '\0' )
    {
        rtx_dbug_out_char( *s++ );
    }
    return RTX_SUCCESS;
}

int32_t rtx_dbug_uint(uint32_t num)
{
    if (num == 0)
    {
        rtx_dbug_outs("0");
    }

    char buf[128];
    buf[127] = '\0';
    int i = 126;
    while (num)
    {
        buf[i--] = (num % 10) + '0';
        num /= 10;
    }
    return rtx_dbug_outs(&buf[i+1]);
}

int32_t rtx_dbug_hex(uint32_t hex)
{
    return rtx_dbug_ptr((void *) hex);
}

int32_t rtx_dbug_ptr(void * ptr)
{
    rtx_dbug_outs("0x");

    char * map = "0123456789ABCDEF";
    uint32_t nptr = (uint32_t) ptr;
    if (nptr == 0)
    {
        return rtx_dbug_outs("0\r\n");
    }

    char buf[128];
    buf[127] = '\0';
    int i = 126;
    while (nptr)
    {
        buf[i--] = map[nptr % 16];
        nptr /= 16;
    }
    return rtx_dbug_outs(&buf[i+1]);
}

