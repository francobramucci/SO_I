	.file	"jardin_ornamental.c"
	.text
	.p2align 4
	.globl	molinete
	.type	molinete, @function
molinete:
.LFB12:
	.cfi_startproc
	addl	$2000000, visitantes(%rip)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE12:
	.size	molinete, .-molinete
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hoy hubo %d visitantes!\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	xorl	%ecx, %ecx
	leaq	molinete(%rip), %rdx
	xorl	%esi, %esi
	movq	%fs:40, %rdi
	movq	%rdi, 24(%rsp)
	leaq	8(%rsp), %rdi
	call	pthread_create@PLT
	xorl	%ecx, %ecx
	leaq	molinete(%rip), %rdx
	xorl	%esi, %esi
	leaq	16(%rsp), %rdi
	call	pthread_create@PLT
	movq	8(%rsp), %rdi
	xorl	%esi, %esi
	call	pthread_join@PLT
	movq	16(%rsp), %rdi
	xorl	%esi, %esi
	call	pthread_join@PLT
	movl	visitantes(%rip), %esi
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rdi
	call	printf@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L6
	xorl	%eax, %eax
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.globl	visitantes
	.bss
	.align 4
	.type	visitantes, @object
	.size	visitantes, 4
visitantes:
	.zero	4
	.ident	"GCC: (GNU) 15.2.1 20260209"
	.section	.note.GNU-stack,"",@progbits
