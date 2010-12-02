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

extern bool enable_debug;

void rtx_dbug_out_char( char c );   // output a char to janusROM terminal
int32_t rtx_dbug_outs( char* s );    // output a string to janusROM terminal

int32_t rtx_dbug_uint(uint32_t num);
int32_t rtx_dbug_ptr(void * ptr);
int32_t rtx_dbug_hex(uint32_t hex);

#endif /* _DBUG_H_ */
