	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	_get_line               ## -- Begin function get_line
	.p2align	4, 0x90
_get_line:                              ## @get_line
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	leaq	-20(%rbp), %r14
	movq	%r14, %rdi
	xorl	%eax, %eax
	callq	_gets
	movq	%r14, %rdi
	callq	_strlen
	leaq	1(%rax), %rdi
	callq	_malloc
	movq	%rax, %rbx
	movq	%rax, %rdi
	movq	%r14, %rsi
	callq	_strcpy
	movq	%rbx, %rax
	addq	$16, %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
