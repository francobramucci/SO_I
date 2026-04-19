	.file	"jardin_ornamental.c"
	.text
	.globl	molinete
	.type	molinete, @function
molinete:
.LFB12:
	.cfi_startproc
	movl	visitantes(%rip), %edx
	leal	1(%rdx), %eax
	addl	$2000001, %edx
	.p2align 4
.L2:
	movl	%eax, %ecx
	addl	$1, %eax
	cmpl	%edx, %eax
	jne	.L2
	movl	%ecx, visitantes(%rip)
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE12:
	.size	molinete, .-molinete
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hoy hubo %d visitantes!\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	leaq	molinete(%rip), %rbx
	leaq	8(%rsp), %rdi
	movl	$0, %ecx
	movq	%rbx, %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	leaq	16(%rsp), %rdi
	movl	$0, %ecx
	movq	%rbx, %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	movl	$0, %esi
	movq	8(%rsp), %rdi
	call	pthread_join@PLT
	movl	$0, %esi
	movq	16(%rsp), %rdi
	call	pthread_join@PLT
	movl	visitantes(%rip), %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L7
	movl	$0, %eax
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L7:
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
