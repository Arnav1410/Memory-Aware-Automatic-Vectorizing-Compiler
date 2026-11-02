	.file	"reference.c"
# GNU C11 (Ubuntu 13.3.0-6ubuntu2~24.04.1) version 13.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -march=alderlake -mmmx -mpopcnt -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2 -mno-sse4a -mno-fma4 -mno-xop -mfma -mno-avx512f -mbmi -mbmi2 -maes -mpclmul -mno-avx512vl -mno-avx512bw -mno-avx512dq -mno-avx512cd -mno-avx512er -mno-avx512pf -mno-avx512vbmi -mno-avx512ifma -mno-avx5124vnniw -mno-avx5124fmaps -mno-avx512vpopcntdq -mno-avx512vbmi2 -mgfni -mvpclmulqdq -mno-avx512vnni -mno-avx512bitalg -mno-avx512bf16 -mno-avx512vp2intersect -mno-3dnow -madx -mabm -mno-cldemote -mclflushopt -mclwb -mno-clzero -mcx16 -mno-enqcmd -mf16c -mfsgsbase -mfxsr -mno-hle -msahf -mno-lwp -mlzcnt -mmovbe -mmovdir64b -mmovdiri -mno-mwaitx -mno-pconfig -mno-pku -mno-prefetchwt1 -mprfchw -mno-ptwrite -mrdpid -mrdrnd -mrdseed -mno-rtm -mserialize -mno-sgx -msha -mshstk -mno-tbm -mno-tsxldtrk -mvaes -mwaitpkg -mno-wbnoinvd -mxsave -mxsavec -mxsaveopt -mxsaves -mno-amx-tile -mno-amx-int8 -mno-amx-bf16 -mno-uintr -mno-hreset -mno-kl -mno-widekl -mavxvnni -mno-avx512fp16 -mno-avxifma -mno-avxvnniint8 -mno-avxneconvert -mno-cmpccxadd -mno-amx-fp16 -mno-prefetchi -mno-raoint -mno-amx-complex --param=l1-cache-size=48 --param=l1-cache-line-size=64 --param=l2-cache-size=12288 -mtune=alderlake -O3 -std=c11 -fwrapv -fPIC -fopt-info-vec-all=/mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/gcc.vectorization.txt -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.p2align 4
	.globl	twoStage
	.type	twoStage, @function
twoStage:
.LFB0:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	andq	$-32, %rsp	#,
	leaq	-4194304(%rsp), %r11	#,
.LPSRL0:
	subq	$4096, %rsp	#,
	orq	$0, (%rsp)	#,
	cmpq	%r11, %rsp	#,
	jne	.LPSRL0
	subq	$32, %rsp	#,
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:1: int twoStage(int *a, int *b, int *c, int *d, int *e, int n) {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp248
	movq	%rax, 4194328(%rsp)	# tmp248, D.2799
	xorl	%eax, %eax	# tmp248
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	testl	%r9d, %r9d	# n
	jle	.L16	#,
	leal	-1(%r9), %r10d	#, _127
	movq	%rdx, %rcx	# tmp246, c
	cmpl	$6, %r10d	#, _127
	jbe	.L17	#,
	movl	%r9d, %r8d	# n, bnd.26
	xorl	%edx, %edx	# ivtmp.68
	movq	%rsp, %rax	#, tmp243
	shrl	$3, %r8d	#,
	salq	$5, %r8	#, _71
	.p2align 4,,10
	.p2align 3
.L4:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	vmovdqu	(%rdi,%rdx), %ymm1	# MEM <vector(8) int> [(int *)a_24(D) + ivtmp.68_192 * 1], tmp252
	vpaddd	(%rsi,%rdx), %ymm1, %ymm0	# MEM <vector(8) int> [(int *)b_25(D) + ivtmp.68_192 * 1], tmp252, vect__8.35
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	vmovdqa	%ymm0, (%rax,%rdx)	# vect__8.35, MEM <vector(8) int> [(int *)&temporary + ivtmp.68_192 * 1]
	addq	$32, %rdx	#, ivtmp.68
	cmpq	%rdx, %r8	# ivtmp.68, _71
	jne	.L4	#,
	movl	%r9d, %edx	# n, tmp.41
	andl	$-8, %edx	#, tmp.41
	cmpl	%edx, %r9d	# tmp.41, n
	movl	%edx, %r8d	# tmp.41,
	je	.L5	#,
.L3:
	movl	%r9d, %eax	# n, niters.38
	subl	%r8d, %eax	# niters_vector_mult_vf.27, niters.38
	leal	-1(%rax), %r11d	#, tmp178
	cmpl	$2, %r11d	#, tmp178
	jbe	.L6	#,
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	vmovdqu	(%rdi,%r8,4), %xmm3	# MEM <vector(4) int> [(int *)vectp_a.43_173], tmp255
	vpaddd	(%rsi,%r8,4), %xmm3, %xmm0	# MEM <vector(4) int> [(int *)vectp_b.46_179], tmp255, vect__122.48
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	vmovdqa	%xmm0, (%rsp,%r8,4)	# vect__122.48, MEM <vector(4) int> [(int *)vectp_temporary.50_186]
	movl	%eax, %r8d	# niters.38, niters_vector_mult_vf.40
	andl	$-4, %r8d	#, niters_vector_mult_vf.40
	addl	%r8d, %edx	# niters_vector_mult_vf.40, tmp.41
	andl	$3, %eax	#, niters_vector_mult_vf.7
	je	.L29	#,
.L6:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movslq	%edx, %rax	# tmp.41, tmp.41
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movl	(%rdi,%rax,4), %r8d	# *_121, *_121
	addl	(%rsi,%rax,4), %r8d	# *_124, tmp189
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movl	%r8d, (%rsp,%rax,4)	# tmp189, temporary[i_166]
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	leal	1(%rdx), %eax	#, i
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	cmpl	%eax, %r9d	# i, n
	jle	.L8	#,
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	cltq
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	addl	$2, %edx	#, i
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movl	(%rsi,%rax,4), %r8d	# *_50, *_50
	addl	(%rdi,%rax,4), %r8d	# *_51, tmp197
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	cmpl	%edx, %r9d	# i, n
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movl	%r8d, (%rsp,%rax,4)	# tmp197, temporary[i_38]
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	jle	.L8	#,
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movslq	%edx, %rdx	# i, i
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movl	(%rsi,%rdx,4), %eax	# *_157, *_157
	addl	(%rdi,%rdx,4), %eax	# *_155, tmp205
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movl	%eax, (%rsp,%rdx,4)	# tmp205, temporary[i_188]
.L8:
	cmpl	$6, %r10d	#, _127
	jbe	.L19	#,
.L27:
	movq	%rsp, %rax	#, tmp243
.L5:
	movl	%r9d, %esi	# n, bnd.6
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	xorl	%edx, %edx	# ivtmp.59
	shrl	$3, %esi	#,
	salq	$5, %rsi	#, _189
	.p2align 4,,10
	.p2align 3
.L10:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	vmovdqa	(%rax,%rdx), %ymm2	# MEM <vector(8) int> [(int *)&temporary + ivtmp.59_115 * 1], tmp258
	vpslld	$1, %ymm2, %ymm0	#, tmp258, tmp213
	vpaddd	%ymm2, %ymm0, %ymm0	#, tmp213, vect__13.12
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	vmovdqu	%ymm0, (%rcx,%rdx)	# vect__13.12, MEM <vector(8) int> [(int *)c_21(D) + ivtmp.59_115 * 1]
	addq	$32, %rdx	#, ivtmp.59
	cmpq	%rsi, %rdx	# _189, ivtmp.59
	jne	.L10	#,
	movl	%r9d, %edx	# n, tmp.18
	andl	$-8, %edx	#, tmp.18
	cmpl	%r9d, %edx	# n, tmp.18
	movl	%edx, %eax	# tmp.18, niters_vector_mult_vf.7
	je	.L12	#,
.L9:
	movl	%r9d, %esi	# n, niters.15
	subl	%eax, %esi	# niters_vector_mult_vf.7, niters.15
	leal	-1(%rsi), %edi	#, tmp215
	cmpl	$2, %edi	#, tmp215
	jbe	.L13	#,
.L7:
	movl	%eax, %eax	# niters_vector_mult_vf.7, niters_vector_mult_vf.7
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	vmovdqa	(%rsp,%rax,4), %xmm4	# MEM <vector(4) int> [(int *)vectp_temporary.20_91], tmp261
	vpslld	$1, %xmm4, %xmm0	#, tmp261, tmp219
	vpaddd	%xmm4, %xmm0, %xmm0	#, tmp219, vect__30.22
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	vmovdqu	%xmm0, (%rcx,%rax,4)	# vect__30.22, MEM <vector(4) int> [(int *)vectp_c.24_99]
	movl	%esi, %eax	# niters.15, niters_vector_mult_vf.17
	andl	$-4, %eax	#, niters_vector_mult_vf.17
	addl	%eax, %edx	# niters_vector_mult_vf.17, tmp.18
	andl	$3, %esi	#, niters.15
	je	.L12	#,
.L13:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movslq	%edx, %rax	# tmp.18, tmp.18
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movl	(%rsp,%rax,4), %esi	# temporary[i_84], temporary[i_84]
	leal	(%rsi,%rsi,2), %esi	#, tmp228
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movl	%esi, (%rcx,%rax,4)	# tmp228, *_6
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	leal	1(%rdx), %eax	#, i
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	cmpl	%eax, %r9d	# i, n
	jle	.L12	#,
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	cltq
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	addl	$2, %edx	#, i
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movl	(%rsp,%rax,4), %esi	# temporary[i_27], temporary[i_27]
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	cmpl	%edx, %r9d	# i, n
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	leal	(%rsi,%rsi,2), %esi	#, tmp234
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movl	%esi, (%rcx,%rax,4)	# tmp234, *_13
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	jle	.L12	#,
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movslq	%edx, %rdx	# i, i
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movl	(%rsp,%rdx,4), %eax	# temporary[i_116], temporary[i_116]
	leal	(%rax,%rax,2), %eax	#, tmp240
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movl	%eax, (%rcx,%rdx,4)	# tmp240, *_76
.L12:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	movl	%r9d, %eax	# n, <retval>
	vzeroupper
.L1:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:7: }
	movq	4194328(%rsp), %rdx	# D.2799, tmp249
	subq	%fs:40, %rdx	# MEM[(<address-space-1> long unsigned int *)40B], tmp249
	jne	.L30	#,
	leave	
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret	
	.p2align 4,,10
	.p2align 3
.L29:
	.cfi_restore_state
	cmpl	$6, %r10d	#, _127
	ja	.L27	#,
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	movl	$4, %esi	#, niters.15
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	xorl	%edx, %edx	# tmp.18
	jmp	.L7	#
	.p2align 4,,10
	.p2align 3
.L16:
	xorl	%eax, %eax	# <retval>
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:6:     return i;
	jmp	.L1	#
.L17:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	xorl	%r8d, %r8d	#
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:4:     for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
	xorl	%edx, %edx	# tmp.41
	jmp	.L3	#
.L19:
	xorl	%eax, %eax	# niters_vector_mult_vf.7
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:5:     for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
	xorl	%edx, %edx	# tmp.18
	jmp	.L9	#
.L30:
# /mnt/c/study materials/semester 7/__btp/simulation part/btp_automatedvectorizedcompilers/build/applications/dynamic/shared/two_stage/kernel.c:7: }
	call	__stack_chk_fail@PLT	#
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
