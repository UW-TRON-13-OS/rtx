#NO_APP
	.file	"primitives.c"
	.text
	.align	2
	.globl	send_message
	.type	send_message, @function
send_message:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 9 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 10 "src/primitives.c" 1
	move.l %d2, -(%a7)
| 0 "" 2
| 13 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 14 "src/primitives.c" 1
	move.l 12(%a6), %d2
| 0 "" 2
| 17 "src/primitives.c" 1
	move.l #TRAP_TABLE_SEND_MESSAGE, %d0
| 0 "" 2
| 18 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 21 "src/primitives.c" 1
	move.l (%a7)+, %d2
| 0 "" 2
| 22 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 25 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	send_message, .-send_message
	.align	2
	.globl	receive_message
	.type	receive_message, @function
receive_message:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 35 "src/primitives.c" 1
	move.l #TRAP_TABLE_RECEIVE_MESSAGE, %d0
| 0 "" 2
| 36 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 39 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	receive_message, .-receive_message
	.align	2
	.globl	request_msg_env
	.type	request_msg_env, @function
request_msg_env:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 49 "src/primitives.c" 1
	move.l #TRAP_TABLE_REQUEST_MSG_ENV, %d0
| 0 "" 2
| 50 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 53 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	request_msg_env, .-request_msg_env
	.align	2
	.globl	release_msg_env
	.type	release_msg_env, @function
release_msg_env:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 63 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 66 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 69 "src/primitives.c" 1
	move.l #TRAP_TABLE_RELEASE_MSG_ENV, %d0
| 0 "" 2
| 70 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 73 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 76 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	release_msg_env, .-release_msg_env
	.align	2
	.globl	release_processor
	.type	release_processor, @function
release_processor:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 86 "src/primitives.c" 1
	move.l #TRAP_TABLE_RELEASE_PROCESSOR, %d0
| 0 "" 2
| 87 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 90 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	release_processor, .-release_processor
	.align	2
	.globl	request_process_status
	.type	request_process_status, @function
request_process_status:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 100 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 103 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 106 "src/primitives.c" 1
	move.l #TRAP_TABLE_REQUEST_PROCESS_STATUS, %d0
| 0 "" 2
| 107 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 110 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 113 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	request_process_status, .-request_process_status
	.align	2
	.globl	terminate
	.type	terminate, @function
terminate:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 123 "src/primitives.c" 1
	move.l #TRAP_TABLE_TERMINATE, %d0
| 0 "" 2
| 124 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 127 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	terminate, .-terminate
	.align	2
	.globl	change_priority
	.type	change_priority, @function
change_priority:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 137 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 138 "src/primitives.c" 1
	move.l %d2, -(%a7)
| 0 "" 2
| 141 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 142 "src/primitives.c" 1
	move.l 12(%a6), %d2
| 0 "" 2
| 145 "src/primitives.c" 1
	move.l #TRAP_TABLE_CHANGE_PRIORITY, %d0
| 0 "" 2
| 146 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 149 "src/primitives.c" 1
	move.l (%a7)+, %d2
| 0 "" 2
| 150 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 153 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	change_priority, .-change_priority
	.align	2
	.globl	request_delay
	.type	request_delay, @function
request_delay:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 163 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 164 "src/primitives.c" 1
	move.l %d2, -(%a7)
| 0 "" 2
| 165 "src/primitives.c" 1
	move.l %d3, -(%a7)
| 0 "" 2
| 168 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 169 "src/primitives.c" 1
	move.l 12(%a6), %d2
| 0 "" 2
| 170 "src/primitives.c" 1
	move.l 16(%a6), %d3
| 0 "" 2
| 173 "src/primitives.c" 1
	move.l #TRAP_TABLE_CHANGE_PRIORITY, %d0
| 0 "" 2
| 174 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 177 "src/primitives.c" 1
	move.l (%a7)+, %d3
| 0 "" 2
| 178 "src/primitives.c" 1
	move.l (%a7)+, %d2
| 0 "" 2
| 179 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 182 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	request_delay, .-request_delay
	.align	2
	.globl	send_console_chars
	.type	send_console_chars, @function
send_console_chars:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 192 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 195 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 198 "src/primitives.c" 1
	move.l #TRAP_TABLE_SEND_CONSOLE_CHARS, %d0
| 0 "" 2
| 199 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 202 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 205 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	send_console_chars, .-send_console_chars
	.align	2
	.globl	get_console_chars
	.type	get_console_chars, @function
get_console_chars:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 215 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 218 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 221 "src/primitives.c" 1
	move.l #TRAP_TABLE_GET_CONSOLE_CHARS, %d0
| 0 "" 2
| 222 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 225 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 228 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	get_console_chars, .-get_console_chars
	.align	2
	.globl	get_trace_buffers
	.type	get_trace_buffers, @function
get_trace_buffers:
	link.w %fp,#-4
	clr.l -4(%fp)
#APP
| 238 "src/primitives.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 241 "src/primitives.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 244 "src/primitives.c" 1
	move.l #TRAP_TABLE_GET_TRACE_BUFFERS, %d0
| 0 "" 2
| 245 "src/primitives.c" 1
	TRAP #KERN_SWI
| 0 "" 2
| 248 "src/primitives.c" 1
	move.l (%a7)+, %d1
| 0 "" 2
| 251 "src/primitives.c" 1
	move.l %d0, -4(%a6)
| 0 "" 2
#NO_APP
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	get_trace_buffers, .-get_trace_buffers
	.ident	"GCC: (Sourcery G++ Lite 4.3-54) 4.3.2"
