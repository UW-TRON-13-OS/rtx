/**
 * @file: dbug.h
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang  
 * @date: 2010/05/03
 * @brief: Header file of dbug.c.  
 */
#ifndef _DBUG_H_
#define _DBUG_H_

#include "coldfire.h"

void rtx_dbug_out_char( char c );   // output a char to janusROM terminal
int32_t rtx_dbug_outs( char* s );    // output a string to janusROM terminal

#endif /* _DBUG_H_ */
