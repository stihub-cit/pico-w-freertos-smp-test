
.syntax unified
.cpu cortex-m0plus
.thumb

.global vRegTest1Implementation
.global vRegTest2Implementation

/*-----------------------------------------------------------*/
vRegTest1Implementation:

	/* Fill the core registers with known values.  This is only done once. */
	movs r1, #101
	movs r2, #102
	movs r3, #103
	movs r4, #104
	movs r5, #105
	movs r6, #106
	movs r7, #107
	movs r0, #108
	mov	 r8, r0
	movs r0, #109
	mov  r9, r0
	movs r0, #110
	mov	 r10, r0
	movs r0, #111
	mov	 r11, r0
	movs r0, #112
	mov  r12, r0
	movs r0, #100

reg1_loop:
	/* Repeatedly check that each register still contains the value written to
	it when the task started. */
	cmp	r0, #100
	bne	reg1_error_loop
	cmp	r1, #101
	bne	reg1_error_loop
	cmp	r2, #102
	bne	reg1_error_loop
	cmp r3, #103
	bne	reg1_error_loop
	cmp	r4, #104
	bne	reg1_error_loop
	cmp	r5, #105
	bne	reg1_error_loop
	cmp	r6, #106
	bne	reg1_error_loop
	cmp	r7, #107
	bne	reg1_error_loop
	movs r0, #108
	cmp	r8, r0
	bne	reg1_error_loop
	movs r0, #109
	cmp	r9, r0
	bne	reg1_error_loop
	movs r0, #110
	cmp	r10, r0
	bne	reg1_error_loop
	movs r0, #111
	cmp	r11, r0
	bne	reg1_error_loop
	movs r0, #112
	cmp	r12, r0
	bne	reg1_error_loop

	/* Everything passed, increment the loop counter. */
	push { r1 }
	ldr	r0, =ulRegTest1LoopCounter
	ldr r1, [r0]
	adds r1, r1, #1
	str r1, [r0]
	pop { r1 }

	/* Start again. */
	movs r0, #100
	b reg1_loop

reg1_error_loop:
	/* If this line is hit then there was an error in a core register value.
	The loop ensures the loop counter stops incrementing. */
	b reg1_error_loop
	nop



vRegTest2Implementation:

	/* Fill the core registers with known values.  This is only done once. */
	movs r1, #1
	movs r2, #2
	movs r3, #3
	movs r4, #4
	movs r5, #5
	movs r6, #6
	movs r7, #7
	movs r0, #8
	mov	r8, r0
	movs r0, #9
	mov r9, r0
	movs r0, #10
	mov	r10, r0
	movs r0, #11
	mov	r11, r0
	movs r0, #12
	mov r12, r0
	movs r0, #10

reg2_loop:
	/* Repeatedly check that each register still contains the value written to
	it when the task started. */
	cmp	r0, #10
	bne	reg2_error_loop
	cmp	r1, #1
	bne	reg2_error_loop
	cmp	r2, #2
	bne	reg2_error_loop
	cmp r3, #3
	bne	reg2_error_loop
	cmp	r4, #4
	bne	reg2_error_loop
	cmp	r5, #5
	bne	reg2_error_loop
	cmp	r6, #6
	bne	reg2_error_loop
	cmp	r7, #7
	bne	reg2_error_loop
	movs r0, #8
	cmp	r8, r0
	bne	reg2_error_loop
	movs r0, #9
	cmp	r9, r0
	bne	reg2_error_loop
	movs r0, #10
	cmp	r10, r0
	bne	reg2_error_loop
	movs r0, #11
	cmp	r11, r0
	bne	reg2_error_loop
	movs r0, #12
	cmp	r12, r0
	bne	reg2_error_loop

	/* Everything passed, increment the loop counter. */
	push { r1 }
	ldr	r0, =ulRegTest2LoopCounter
	ldr r1, [r0]
	adds r1, r1, #1
	str r1, [r0]
	pop { r1 }

	/* Start again. */
	movs r0, #10
	b reg2_loop

reg2_error_loop:
	/* If this line is hit then there was an error in a core register value.
	The loop ensures the loop counter stops incrementing. */
	b reg2_error_loop
	nop

