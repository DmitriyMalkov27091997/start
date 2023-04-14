	.syntax unified
	.cpu cortex-m3
	.fpu softvfp
	.thumb
	.align 1
	.thumb
	.text
	.thumb_func
	.global	HardFaultException
	.extern print_fault

HardFaultException:
	mov		r0, sp
	push	{lr}
	bl		print_fault
	pop		{pc}
.dloop:
	B       .dloop
	.size	HardFaultException, .-HardFaultException

	.thumb
	.thumb_func
	.global	rbit8
	.type rbit8, %function

rbit8:
	.cfi_startproc
	rbit	r0, r0
	rev		r0, r0
	bx		lr
	.size	rbit8, .-rbit8
	.cfi_endproc

	.thumb
	.thumb_func
	.global	NVIC_SystemReset
	.type NVIC_SystemReset, %function

NVIC_SystemReset:
	.cfi_startproc
	ldr		r0, =0xE000ED0C
	ldr		r1, =0x05FA0004
	str		r1, [r0]
	dsb
deadloop:
	b			deadloop
	.size	NVIC_SystemReset, .-NVIC_SystemReset
	.cfi_endproc
