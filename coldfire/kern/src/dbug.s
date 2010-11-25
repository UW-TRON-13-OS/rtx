#NO_APP
	.file	"dbug.c"
	.text
	.align	2
	.globl	rtx_dbug_out_char
	.type	rtx_dbug_out_char, @function
rtx_dbug_out_char:
	link.w %fp,#-4
	move.l 8(%fp),%d0
	move.b %d0,-2(%fp)
#APP
| 20 "dbug.c" 1
	move.l %d0, -(%a7)
| 0 "" 2
| 21 "dbug.c" 1
	move.l %d1, -(%a7)
| 0 "" 2
| 24 "dbug.c" 1
	move.l 8(%a6), %d1
| 0 "" 2
| 27 "dbug.c" 1
	move.l #0x13, %d0
| 0 "" 2
| 28 "dbug.c" 1
	trap #15
| 0 "" 2
| 31 "dbug.c" 1
	 move.l (%a7)+, %d1
| 0 "" 2
| 32 "dbug.c" 1
	 move.l (%a7)+, %d0
| 0 "" 2
#NO_APP
	unlk %fp
	rts
	.size	rtx_dbug_out_char, .-rtx_dbug_out_char
	.align	2
	.globl	rtx_dbug_outs
	.type	rtx_dbug_outs, @function
rtx_dbug_outs:
	link.w %fp,#-4
	tst.l 8(%fp)
	jne .L6
	moveq #-1,%d0
	move.l %d0,-4(%fp)
	jra .L5
.L7:
	move.l 8(%fp),%a0
	move.b (%a0),%d0
	extb.l %d0
	addq.l #1,8(%fp)
	move.l %d0,-(%sp)
	jsr rtx_dbug_out_char
	addq.l #4,%sp
.L6:
	move.l 8(%fp),%a0
	move.b (%a0),%d0
	tst.b %d0
	jne .L7
	clr.l -4(%fp)
.L5:
	move.l -4(%fp),%d0
	unlk %fp
	rts
	.size	rtx_dbug_outs, .-rtx_dbug_outs
	.ident	"GCC: (Sourcery G++ Lite 4.3-54) 4.3.2"
