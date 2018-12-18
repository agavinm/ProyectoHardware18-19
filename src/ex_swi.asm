.globl   ex_swi
	.type    ex_swi,%function

	// Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
	
ex_swi:
	swi #0
	mov pc, lr
