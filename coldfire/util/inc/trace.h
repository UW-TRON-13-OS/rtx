#ifndef _TRACE_H_
#define _TRACE_H_

#include "coldfire.h"
#include "dbug.h"

enum trace_level { 
    CHATTY, TRACE, DEBUG, WARNING, ERROR, ALWAYS
};

#ifndef TRACE_LEVEL
#define TRACE_LEVEL DEBUG
#endif

#define trace_char(level, c) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_out_char(c); \
        } \
    } while (0)

#define trace_inline(level, msg) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_outs(msg); \
        } \
    } while (0)

#define trace(level,msg) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_outs(msg); \
            rtx_dbug_outs(CRLF); \
        } \
    } while (0)
        
#define trace_str(level,msg,str) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_outs(msg); \
            rtx_dbug_outs((str)); \
            rtx_dbug_outs(CRLF); \
        } \
    } while (0)

#define trace_int(level,msg,num) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_outs(msg); \
            rtx_dbug_int(num); \
            rtx_dbug_outs(CRLF); \
        } \
    } while (0)

#define trace_uint(level,msg,num) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_outs(msg); \
            rtx_dbug_uint(num); \
            rtx_dbug_outs(CRLF); \
        } \
    } while (0)

#define trace_hex(level,msg,num) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_outs(msg); \
            rtx_dbug_hex(num); \
            rtx_dbug_outs(CRLF); \
        } \
    } while (0)

#define trace_ptr(level,msg,ptr) do { \
        if ((level) >= TRACE_LEVEL) \
        { \
            rtx_dbug_outs(msg); \
            rtx_dbug_ptr(ptr); \
            rtx_dbug_outs(CRLF); \
        } \
    } while (0)

#endif
