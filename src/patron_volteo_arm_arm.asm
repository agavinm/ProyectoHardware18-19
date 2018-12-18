.globl   patron_volteo_arm_arm
	.type    patron_volteo_arm_arm,%function

	// Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
	
patron_volteo_arm_arm:
	mov r12, sp
	push {r11, r12, lr}
	sub r11, r12, #4
	push {r1-r6}
	mov r12, sp
	
	// FA = FA + SF
	// r2 = FA
	ldrb r4, [r11, #4]   // r4 = SF
	add r2, r2, r4
	bic r2, r2, #0x100 // limpia el bit 8 para que la suma sea de 1 Byte
	
	// CA = CA + SC
	// r3 = CA
	ldrb r5, [r11, #8]   // r5 = SC
	add r3, r3, r5
	bic r3, r3, #0x100
	
	// r4 y r5 libres
	strb r5, [sp, #-8]
	strb r4, [sp, #-12]
	
	// if ((FA < DIM) && (FA >= 0) && (CA < DIM) && (CA >= 0) && (tablero[FA][CA] != CASILLA_VACIA))
	cmp r2, #8			 // FA < DIM
	bge patron_volteo_arm_arm+112
	cmp r2, #0			 // FA >= 0
	blt patron_volteo_arm_arm+112
	cmp r3, #8			 // CA < DIM
	bge patron_volteo_arm_arm+112
	cmp r3, #0			 // CA >= 0
	blt patron_volteo_arm_arm+112
	add r4, r3, r2, LSL #3 // (FA*DIM) + CA
	ldrb r4, [r0, r4]	 // tablero[FA][CA]
	cmp r4, #0			 // tablero[FA][CA] != CASILLA_VACIA
	beq patron_volteo_arm_arm+112
	mov r5, #1 // posicion_valida = 1
	mov r6, r4 // casilla = tablero[FA][CA]
	b patron_volteo_arm_arm+120
	mov r5, #0			 // <patron_volteo_arm_arm+112> posicion_valida = 0
	mov r6, #0 // casilla = 0

	// <patron_volteo_arm_arm+120>
	// r0 = @tablero, r1 = @longitud, r2 = FA, r3 = CA, r4 = tablero[FA][CA], r5 = posicion_valida, r6 = casilla
	
	ldrb r4, [r11, #12]   // r4 = color
	
	// r0 = @tablero
	// r1 = @longitud
	// r2 = FA
	// r3 = CA
	// r4 = color
	// r5 = posicion_valida
	// r6 = casilla
	
	// if ((posicion_valida == 1) && (casilla != color))
	cmp r5, #1
	bne patron_volteo_arm_arm+192
	cmp r6, r4
	beq patron_volteo_arm_arm+172
	// r5 y r6 libres
	ldr r5, [r1] // r5 = longitud
	add r5, r5, #1
	str r5, [r1] // *longitud = *longitud + 1;
	strb r4, [sp, #-4] // color en pila
	sub sp, sp, #12
	bl patron_volteo_arm_arm // r0 = patron
	add sp, sp, #12
	b patron_volteo_arm_arm+196
	
	// else if ((posicion_valida == 1) && (casilla == color)) // <patron_volteo_arm_arm+172>
	ldr r5, [r1] // r5 = longitud
	cmp r5, #0
	ble patron_volteo_arm_arm+192 // if (*longitud <= 0)
	mov r0, #1 // if (*longitud > 0) return PATRON_ENCONTRADO;
	b patron_volteo_arm_arm+196
	
	
	// else // <patron_volteo+192>
	mov r0, #0 // return NO_HAY_PATRON;
	
	// <patron_volteo_arm_arm+196>
	pop {r1-r6}
	pop {r11, r12, lr}
	mov sp, r12
	bx lr
