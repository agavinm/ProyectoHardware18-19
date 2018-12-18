.globl   patron_volteo_arm_c
     //.p2align 2
	.type    patron_volteo_arm_c,%function

	// Autores: Andres Gavin Murillo 716358 y Eduardo Gimeno Soriano 721615
	
patron_volteo_arm_c:
	mov r12, sp
	push {r11, r12, lr}
	sub r11, r12, #4
	push {r1-r7}
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
	
	mov r4, r0			  // r4 = @tablero
	mov r5, r1			  // r5 = *longitud
	mov r6, r2			  // r6 = FA
	mov r7, r3			  // r7 = CA

	mov r1, r2            // r1 = FA
	mov r2, r3			  // r2 = CA
	sub sp, sp, #16
	mov r3, sp			  // r3 = @posicion_valida

	bl ficha_valida
	mov r1, r5			  // r1 = @longitud
	mov r2, r6			  // r2 = FA

	ldrb r5, [sp]         // r5 = posicion_valida
	mov r6, r0			  // r6 = casilla
	mov r3, r7			  // r3 = CA
	mov r0, r4			  // r0 = @tablero

	add sp, sp, #16
	
	// r0 = @tablero, r1 = @longitud, r2 = FA, r3 = CA, r5 = posicion_valida, r6 = casilla
	
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
	bne patron_volteo_arm_c+188
	cmp r6, r4
	beq patron_volteo_arm_c+168
	// r5 y r6 libres
	ldr r5, [r1] // r5 = longitud
	add r5, r5, #1
	str r5, [r1] // *longitud = *longitud + 1;
	strb r4, [sp, #-4] // color en pila
	sub sp, sp, #12
	bl patron_volteo_arm_c // r0 = patron
	add sp, sp, #12
	b patron_volteo_arm_c+192
	
	// else if ((posicion_valida == 1) && (casilla == color)) // <patron_volteo_arm_c+168>
	ldr r5, [r1] // r5 = longitud
	cmp r5, #0
	ble patron_volteo_arm_c+188 // if (*longitud <= 0)
	mov r0, #1 // if (*longitud > 0) return PATRON_ENCONTRADO;
	b patron_volteo_arm_c+192
	
	
	// else // <patron_volteo+188>
	mov r0, #0 // return NO_HAY_PATRON;
	
	// <patron_volteo_arm_c+192>
	pop {r1-r7}
	pop {r11, r12, lr}
	mov sp, r12
	bx lr
