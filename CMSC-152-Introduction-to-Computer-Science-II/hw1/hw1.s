	.file	"hw1.c"
	.text
.globl _gcd
	.def	_gcd;	.scl	2;	.type	32;	.endef
_gcd:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$8, %esp
	movl	8(%ebp), %ebx
	movl	12(%ebp), %ecx
	movl	%ebx, %eax
	testl	%ecx, %ecx
	je	L1
	cmpl	%ecx, %ebx
	jb	L4
	movl	%ebx, %eax
	movl	$0, %edx
	divl	%ecx
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_gcd
	jmp	L1
L4:
	cmpl	%ebx, %ecx
	jb	L1
	movl	%ecx, %eax
	movl	$0, %edx
	divl	%ebx
	movl	%edx, 4(%esp)
	movl	%ebx, (%esp)
	call	_gcd
L1:
	addl	$8, %esp
	popl	%ebx
	popl	%ebp
	ret
	.section .rdata,"dr"
	.align 8
LC0:
	.long	1413754136
	.long	1074340347
	.text
.globl _area_pipe
	.def	_area_pipe;	.scl	2;	.type	32;	.endef
_area_pipe:
	pushl	%ebp
	movl	%esp, %ebp
	fldl	8(%ebp)
	fldl	16(%ebp)
	fld	%st(1)
	faddl	24(%ebp)
	fldl	LC0
	fld	%st(1)
	fmul	%st(1), %st
	fmul	%st(2), %st
	fld	%st(4)
	fmul	%st(2), %st
	fmul	%st(5), %st
	fsubrp	%st, %st(1)
	fadd	%st(0), %st
	fld	%st(3)
	fadd	%st(4), %st
	fmul	%st(2), %st
	fmulp	%st, %st(3)
	fxch	%st(3)
	fadd	%st(0), %st
	fmulp	%st, %st(1)
	fmulp	%st, %st(3)
	faddp	%st, %st(1)
	faddp	%st, %st(1)
	popl	%ebp
	ret
.globl _adj
	.def	_adj;	.scl	2;	.type	32;	.endef
_adj:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %ebx
	cmpl	$1, %eax
	jne	L9
	movl	$1374389535, %ecx
	movl	%ebx, %eax
	imull	%ecx
	movl	%eax, -16(%ebp)
	movl	%edx, -12(%ebp)
	movl	-12(%ebp), %eax
	sarl	$7, %eax
	movl	%ebx, %esi
	sarl	$31, %esi
	subl	%esi, %eax
	leal	(%eax,%eax,4), %eax
	leal	(%eax,%eax,4), %eax
	sall	$4, %eax
	cmpl	%eax, %ebx
	jne	L12
	movl	-12(%ebp), %eax
	sarl	$5, %eax
	subl	%esi, %eax
	leal	(%eax,%eax,4), %eax
	leal	(%eax,%eax,4), %eax
	sall	$2, %eax
	cmpl	%eax, %ebx
	jne	L11
L12:
	movl	$1374389535, %eax
	imull	%ebx
	movl	%edx, %ecx
	sarl	$7, %ecx
	movl	%ebx, %eax
	cltd
	movl	%ecx, %eax
	subl	%edx, %eax
	leal	(%eax,%eax,4), %eax
	leal	(%eax,%eax,4), %eax
	sall	$4, %eax
	movl	$1, %edx
	cmpl	%eax, %ebx
	jne	L8
L11:
	movl	$0, %edx
	jmp	L8
L9:
	cmpl	$2, %eax
	jne	L15
	movl	$1374389535, %ecx
	movl	%ebx, %eax
	imull	%ecx
	movl	%eax, -16(%ebp)
	movl	%edx, -12(%ebp)
	movl	-12(%ebp), %eax
	sarl	$7, %eax
	movl	%ebx, %esi
	sarl	$31, %esi
	subl	%esi, %eax
	leal	(%eax,%eax,4), %eax
	leal	(%eax,%eax,4), %eax
	sall	$4, %eax
	cmpl	%eax, %ebx
	jne	L18
	movl	-12(%ebp), %eax
	sarl	$5, %eax
	subl	%esi, %eax
	leal	(%eax,%eax,4), %eax
	leal	(%eax,%eax,4), %eax
	sall	$2, %eax
	cmpl	%eax, %ebx
	jne	L17
L18:
	movl	$1374389535, %eax
	imull	%ebx
	movl	%edx, %ecx
	sarl	$7, %ecx
	movl	%ebx, %eax
	cltd
	movl	%ecx, %eax
	subl	%edx, %eax
	leal	(%eax,%eax,4), %eax
	leal	(%eax,%eax,4), %eax
	sall	$4, %eax
	movl	$4, %edx
	cmpl	%eax, %ebx
	jne	L8
L17:
	movl	$3, %edx
	jmp	L8
L15:
	movl	$4, %edx
	cmpl	$3, %eax
	je	L8
	movl	$0, %edx
	cmpl	$4, %eax
	je	L8
	movl	$2, %edx
	cmpl	$5, %eax
	je	L8
	movl	$5, %edx
	cmpl	$6, %eax
	je	L8
	movl	$0, %edx
	cmpl	$7, %eax
	je	L8
	movl	$3, %edx
	cmpl	$8, %eax
	je	L8
	movl	$6, %edx
	cmpl	$9, %eax
	je	L8
	movl	$1, %edx
	cmpl	$10, %eax
	je	L8
	movl	$4, %edx
	cmpl	$11, %eax
	je	L8
	movl	$6, %edx
L8:
	movl	%edx, %eax
	addl	$8, %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
.globl _dow
	.def	_dow;	.scl	2;	.type	32;	.endef
_dow:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$32, %esp
	movl	16(%ebp), %ebx
	movl	%ebx, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_adj
	movl	%eax, %esi
	addl	%ebx, %esi
	addl	12(%ebp), %esi
	movl	%ebx, %eax
	testl	%ebx, %ebx
	jns	L41
	addl	$3, %eax
L41:
	sarl	$2, %eax
	pushl	%eax
	fildl	(%esp)
	leal	4(%esp), %esp
	fstpl	(%esp)
	call	_floor
	fnstcw	-10(%ebp)
	movzwl	-10(%ebp), %eax
	orw	$3072, %ax
	movw	%ax, -12(%ebp)
	fldcw	-12(%ebp)
	fistpl	-16(%ebp)
	fldcw	-10(%ebp)
	movl	-16(%ebp), %eax
	leal	-1900(%esi,%eax), %esi
	movl	$0, 8(%esp)
	movl	$1075576832, 12(%esp)
	pushl	%esi
	fildl	(%esp)
	leal	4(%esp), %esp
	fstpl	(%esp)
	call	_fmod
	fnstcw	-10(%ebp)
	movzwl	-10(%ebp), %eax
	orw	$3072, %ax
	movw	%ax, -12(%ebp)
	fldcw	-12(%ebp)
	fistpl	-16(%ebp)
	fldcw	-10(%ebp)
	movl	-16(%ebp), %eax
	addl	$32, %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC7:
	.ascii "%d %d %d %lf\12\0"
LC8:
	.ascii "%d\0"
	.align 4
LC5:
	.long	1077936128
	.align 4
LC6:
	.long	1073741824
	.text
.globl _main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$32, %esp
	andl	$-16, %esp
	movl	$16, %eax
	call	__alloca
	call	___main
	movl	$0, 16(%esp)
	movl	$1074790400, 20(%esp)
	flds	LC5
	fstpl	8(%esp)
	flds	LC6
	fstpl	(%esp)
	call	_area_pipe
	fstpl	-16(%ebp)
	movl	$8, 4(%esp)
	movl	$2, (%esp)
	call	_gcd
	movl	%eax, %esi
	movl	$2, 4(%esp)
	movl	$8, (%esp)
	call	_gcd
	movl	%eax, %ebx
	movl	$2, 4(%esp)
	movl	$2, (%esp)
	call	_gcd
	fldl	-16(%ebp)
	fstpl	16(%esp)
	movl	%esi, 12(%esp)
	movl	%ebx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$LC7, (%esp)
	call	_printf
	movl	$1999, 8(%esp)
	movl	$9, 4(%esp)
	movl	$9, (%esp)
	call	_dow
	movl	%eax, 4(%esp)
	movl	$LC8, (%esp)
	call	_printf
	leal	-8(%ebp), %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
	.def	_printf;	.scl	3;	.type	32;	.endef
	.def	_fmod;	.scl	3;	.type	32;	.endef
	.def	_floor;	.scl	3;	.type	32;	.endef
