.globl   ex_data_abort
	.type    ex_data_abort,%function

	// Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
	
ex_data_abort:
	ldr r0, =0x0a333333
	str r0, [r0]
	mov pc, lr
