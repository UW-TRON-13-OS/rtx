	.globl asm_uart_entry
	.even		
asm_uart_entry:
    move.w #0x2700, %sr 

	move.l %d0, -(%a7)
	move.l %d1, -(%a7)
	move.l %d2, -(%a7)
	move.l %d3, -(%a7)
	move.l %d4, -(%a7)
	move.l %d5, -(%a7)
	move.l %d6, -(%a7)
	move.l %d7, -(%a7)
	move.l %a0, -(%a7)
	move.l %a1, -(%a7)
	move.l %a2, -(%a7)
	move.l %a3, -(%a7)
	move.l %a4, -(%a7)
	move.l %a5, -(%a7)
	move.l %a6, -(%a7)

    move.l #1, %d1
    move.l %d1, -(%a7)
		
	jsr isr_handler
	
    move.l (%a7)+, %d1

	move.l (%a7)+, %a6
	move.l (%a7)+, %a5
	move.l (%a7)+, %a4
	move.l (%a7)+, %a3
	move.l (%a7)+, %a2
	move.l (%a7)+, %a1
	move.l (%a7)+, %a0
	move.l (%a7)+, %d7
	move.l (%a7)+, %d6
	move.l (%a7)+, %d5
	move.l (%a7)+, %d4
	move.l (%a7)+, %d3
	move.l (%a7)+, %d2
	move.l (%a7)+, %d1
	move.l (%a7)+, %d0

    move.w #0x2000, %sr 
	
	rte

	.globl asm_timer_entry
	.even		
asm_timer_entry:
    move.w #0x2700, %sr 

	move.l %d0, -(%a7)
	move.l %d1, -(%a7)
	move.l %d2, -(%a7)
	move.l %d3, -(%a7)
	move.l %d4, -(%a7)
	move.l %d5, -(%a7)
	move.l %d6, -(%a7)
	move.l %d7, -(%a7)
	move.l %a0, -(%a7)
	move.l %a1, -(%a7)
	move.l %a2, -(%a7)
	move.l %a3, -(%a7)
	move.l %a4, -(%a7)
	move.l %a5, -(%a7)
	move.l %a6, -(%a7)

    move.l #2, %d1
    move.l %d1, -(%a7)
		
	jsr isr_handler

    move.l (%a7)+, %d1
	
	move.l (%a7)+, %a6
	move.l (%a7)+, %a5
	move.l (%a7)+, %a4
	move.l (%a7)+, %a3
	move.l (%a7)+, %a2
	move.l (%a7)+, %a1
	move.l (%a7)+, %a0
	move.l (%a7)+, %d7
	move.l (%a7)+, %d6
	move.l (%a7)+, %d5
	move.l (%a7)+, %d4
	move.l (%a7)+, %d3
	move.l (%a7)+, %d2
	move.l (%a7)+, %d1
	move.l (%a7)+, %d0

    move.w #0x2000, %sr 
	
	rte

    .globl asm_kern_swi
    .even
asm_kern_swi:
    move.w #0x2700, %sr 
	jsr	kern_swi_handler
    move.w #0x2000, %sr 
	
	rte
