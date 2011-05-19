#############################################################
#   SINT32 rtx_memset( VOID* ptr, UINT32 value, UINT32 size )
#  
#   a0: start address
#   a1: end address
#   d1: value to set (lsb)
#   d0: return code
#############################################################
	
	.even
	.globl rtx_memset
rtx_memset:
	# save registers
	link    %a6, #0
	sub.l   #12, %a7
	movem.l %a0/%a1/%d1, (%a7)

	# initialize working vars
	clr.l	%d0
	move.l  8(%a6), %a0
	cmp.l   #0, %a0
	beq	.rtx_memset_2
	move.l  %a0, %a1
	add.l   16(%a6), %a1
	move.l  12(%a6), %d1

.rtx_memset_1:
	cmp.l   %a0,%a1
	beq     .rtx_memset_3
	move.b  %d1, (%a0)+
	bra     .rtx_memset_1

.rtx_memset_2:
# Return an error	
	move.l  #-1, %d0
	
.rtx_memset_3:
	movem.l (%a7), %a0/%a1/%d1
	add.l   #12, %a7
	unlk    %a6
	rts
