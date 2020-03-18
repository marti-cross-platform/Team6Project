	.file	"Ccode.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB79:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$48, %esp
	call	___main
	movl	$0, 44(%esp)
	movl	$0, 32(%esp)
	movl	$0, 44(%esp)
	jmp	L2
L7:
	movzbl	_gPasswordHash, %eax
	movzbl	%al, %eax
	sall	$8, %eax
	movl	%eax, %edx
	movzbl	_gPasswordHash+1, %eax
	movzbl	%al, %eax
	addl	%edx, %eax
	movl	%eax, 28(%esp)
	movl	44(%esp), %eax
	sall	$2, %eax
	addl	$2, %eax
	movzbl	_gPasswordHash(%eax), %eax
	movzbl	%al, %eax
	sall	$8, %eax
	movl	%eax, %edx
	movl	44(%esp), %eax
	sall	$2, %eax
	addl	$3, %eax
	movzbl	_gPasswordHash(%eax), %eax
	movzbl	%al, %eax
	addl	%edx, %eax
	movl	%eax, 36(%esp)
	cmpl	$0, 36(%esp)
	jne	L3
	movl	$65535, 36(%esp)
L3:
	movl	28(%esp), %eax
	movl	%eax, 32(%esp)
	movl	$0, 40(%esp)
	jmp	L4
L6:
	movl	40(%esp), %eax
	addl	$_gOutFileName, %eax
	movzbl	(%eax), %eax
	movl	32(%esp), %edx
	addl	$_gkey, %edx
	movzbl	(%edx), %edx
	xorl	%eax, %edx
	movl	40(%esp), %eax
	addl	$_gOutFileName, %eax
	movb	%dl, (%eax)
	movl	36(%esp), %eax
	addl	%eax, 32(%esp)
	cmpl	$65536, 32(%esp)
	jle	L5
	subl	$65537, 32(%esp)
L5:
	addl	$1, 40(%esp)
L4:
	movl	$_gInFileName, (%esp)
	call	_fileLength
	cmpl	40(%esp), %eax
	jg	L6
	addl	$1, 44(%esp)
L2:
	cmpl	$0, 44(%esp)
	jle	L7
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE79:
	.ident	"GCC: (GNU) 4.9.2"
	.def	_fileLength;	.scl	2;	.type	32;	.endef
