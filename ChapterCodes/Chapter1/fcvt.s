	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	_fcvt                   ## -- Begin function fcvt
	.p2align	4, 0x90
_fcvt:                                  ## @fcvt
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	cvttsd2si	(%rdx), %rax
	cvtsi2sdq	(%rcx), %xmm0
	movss	(%rsi), %xmm1           ## xmm1 = mem[0],zero,zero,zero
	cvtsi2sdl	%edi, %xmm2
	cvtsd2ss	%xmm2, %xmm2
	movq	%rax, (%rcx)
	movss	%xmm2, (%rsi)
	movsd	%xmm0, (%rdx)
	xorps	%xmm0, %xmm0
	cvtss2sd	%xmm1, %xmm0
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
