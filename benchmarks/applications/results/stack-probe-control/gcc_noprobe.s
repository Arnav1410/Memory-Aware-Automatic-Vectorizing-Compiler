	.file	"reference.c"
	.text
	.p2align 4
	.globl	twoStage
	.type	twoStage, @function
twoStage:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	andq	$-32, %rsp
	subq	$4194336, %rsp
	movq	%fs:40, %rax
	movq	%rax, 4194328(%rsp)
	xorl	%eax, %eax
	testl	%r9d, %r9d
	jle	.L16
	leal	-1(%r9), %r10d
	movq	%rdx, %rcx
	cmpl	$6, %r10d
	jbe	.L17
	movl	%r9d, %r8d
	xorl	%edx, %edx
	movq	%rsp, %rax
	shrl	$3, %r8d
	salq	$5, %r8
	.p2align 4,,10
	.p2align 3
.L4:
	vmovdqu	(%rdi,%rdx), %ymm1
	vpaddd	(%rsi,%rdx), %ymm1, %ymm0
	vmovdqa	%ymm0, (%rax,%rdx)
	addq	$32, %rdx
	cmpq	%rdx, %r8
	jne	.L4
	movl	%r9d, %edx
	andl	$-8, %edx
	cmpl	%edx, %r9d
	movl	%edx, %r8d
	je	.L5
.L3:
	movl	%r9d, %eax
	subl	%r8d, %eax
	leal	-1(%rax), %r11d
	cmpl	$2, %r11d
	jbe	.L6
	vmovdqu	(%rdi,%r8,4), %xmm3
	vpaddd	(%rsi,%r8,4), %xmm3, %xmm0
	vmovdqa	%xmm0, (%rsp,%r8,4)
	movl	%eax, %r8d
	andl	$-4, %r8d
	addl	%r8d, %edx
	andl	$3, %eax
	je	.L29
.L6:
	movslq	%edx, %rax
	movl	(%rdi,%rax,4), %r8d
	addl	(%rsi,%rax,4), %r8d
	movl	%r8d, (%rsp,%rax,4)
	leal	1(%rdx), %eax
	cmpl	%eax, %r9d
	jle	.L8
	cltq
	addl	$2, %edx
	movl	(%rsi,%rax,4), %r8d
	addl	(%rdi,%rax,4), %r8d
	cmpl	%edx, %r9d
	movl	%r8d, (%rsp,%rax,4)
	jle	.L8
	movslq	%edx, %rdx
	movl	(%rsi,%rdx,4), %eax
	addl	(%rdi,%rdx,4), %eax
	movl	%eax, (%rsp,%rdx,4)
.L8:
	cmpl	$6, %r10d
	jbe	.L19
.L27:
	movq	%rsp, %rax
.L5:
	movl	%r9d, %esi
	xorl	%edx, %edx
	shrl	$3, %esi
	salq	$5, %rsi
	.p2align 4,,10
	.p2align 3
.L10:
	vmovdqa	(%rax,%rdx), %ymm2
	vpslld	$1, %ymm2, %ymm0
	vpaddd	%ymm2, %ymm0, %ymm0
	vmovdqu	%ymm0, (%rcx,%rdx)
	addq	$32, %rdx
	cmpq	%rsi, %rdx
	jne	.L10
	movl	%r9d, %edx
	andl	$-8, %edx
	cmpl	%r9d, %edx
	movl	%edx, %eax
	je	.L12
.L9:
	movl	%r9d, %esi
	subl	%eax, %esi
	leal	-1(%rsi), %edi
	cmpl	$2, %edi
	jbe	.L13
.L7:
	movl	%eax, %eax
	vmovdqa	(%rsp,%rax,4), %xmm4
	vpslld	$1, %xmm4, %xmm0
	vpaddd	%xmm4, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rcx,%rax,4)
	movl	%esi, %eax
	andl	$-4, %eax
	addl	%eax, %edx
	andl	$3, %esi
	je	.L12
.L13:
	movslq	%edx, %rax
	movl	(%rsp,%rax,4), %esi
	leal	(%rsi,%rsi,2), %esi
	movl	%esi, (%rcx,%rax,4)
	leal	1(%rdx), %eax
	cmpl	%eax, %r9d
	jle	.L12
	cltq
	addl	$2, %edx
	movl	(%rsp,%rax,4), %esi
	cmpl	%edx, %r9d
	leal	(%rsi,%rsi,2), %esi
	movl	%esi, (%rcx,%rax,4)
	jle	.L12
	movslq	%edx, %rdx
	movl	(%rsp,%rdx,4), %eax
	leal	(%rax,%rax,2), %eax
	movl	%eax, (%rcx,%rdx,4)
.L12:
	movl	%r9d, %eax
	vzeroupper
.L1:
	movq	4194328(%rsp), %rdx
	subq	%fs:40, %rdx
	jne	.L30
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L29:
	.cfi_restore_state
	cmpl	$6, %r10d
	ja	.L27
	movl	$4, %esi
	xorl	%edx, %edx
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L16:
	xorl	%eax, %eax
	jmp	.L1
.L17:
	xorl	%r8d, %r8d
	xorl	%edx, %edx
	jmp	.L3
.L19:
	xorl	%eax, %eax
	xorl	%edx, %edx
	jmp	.L9
.L30:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE0:
	.size	twoStage, .-twoStage
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
