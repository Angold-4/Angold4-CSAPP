	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	_proc                   ## -- Begin function proc
	.p2align	4, 0x90
_proc:                                  ## @proc
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	addq	%rdi, (%rsi)
	addl	%edx, (%rcx)
	movb	16(%rbp), %al
	addw	%r8w, (%r9)
	movq	24(%rbp), %rcx
	addb	%al, (%rcx)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_call_proc              ## -- Begin function call_proc
	.p2align	4, 0x90
_call_proc:                             ## @call_proc
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	$1, -16(%rbp)
	movl	$2, -8(%rbp)
	movw	$3, -4(%rbp)
	movb	$4, -1(%rbp)
	leaq	-1(%rbp), %rax
	leaq	-16(%rbp), %rsi
	leaq	-8(%rbp), %rcx
	leaq	-4(%rbp), %r9
	movl	$1, %edi
	movl	$2, %edx
	movl	$3, %r8d
	pushq	%rax
	pushq	$4
	callq	_proc
	addq	$16, %rsp
	movslq	-8(%rbp), %rcx
	addq	-16(%rbp), %rcx
	movswq	-4(%rbp), %rax
	movsbq	-1(%rbp), %rdx
	subq	%rdx, %rax
	imulq	%rcx, %rax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
