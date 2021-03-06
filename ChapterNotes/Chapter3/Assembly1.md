## Assembly Language 

**Some Notes for Chapter 3  -- 20200521 by Jiawei Wang**

### C --> Assembly
**There are two ways to read Codes from C to Assembly:<br>**
**We Use This Test.c file to Test:**
```c
#include <stdio.h>

int main(){
    printf("Hello, World!");
    return 0;
}

```
* **gcc -S:**<br>
```✗ gcc -Og -S test.c```<br>**Then gcc will generate a file called test.s:(-Og means Optimization)**<br>

```assembly
## test.s
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L_.str(%rip), %rdi
	xorl	%eax, %eax
	callq	_printf
	xorl	%eax, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Hello, World!"


.subsections_via_symbols

```

* **objdump:**<br>

```
✗ gcc -Og -c test.c

test.o:	file format Mach-O 64-bit x86-64

Disassembly of section __TEXT,__text:
_main:
       0:	55 	pushq	%rbp
       1:	48 89 e5 	movq	%rsp, %rbp
       4:	48 8d 3d 0b 00 00 00 	leaq	11(%rip), %rdi
       b:	31 c0 	xorl	%eax, %eax
       d:	e8 00 00 00 00 	callq	0 <_main+0x12>
      12:	31 c0 	xorl	%eax, %eax
      14:	5d 	popq	%rbp
      15:	c3 	retq

```
### Access Information

**An x86-64 CPU contains a set of 16 general-purpose registers that store 64-bit values<br>**
```assembly
## This is a list for these 16 general-purpose reigsters:

64    32     16     8
%rax  %eax   %ax    %al
%rbx  %ebx   %bx    %bl
%rcx  %ecx   %cx    %cl
%rdx  %edx   %dx    %dl
%rsi  %esi   %si    %sil
%rdi  %edi   %di    %dil
%rbp  %ebp   %bp    %bpl
%rsp  %esp   %sp    %spl
%r8   %r8d   %r8w   %r8b
%r9   %r9d   %r9w   %r9b
%r10  %r10d  %r10w  %r10b
%r11  %r11d  %r11w  %r11b
%r12  %r12d  %r12w  %r12b
%r13  %r13d  %r13w  %r13b
%r14  %r14d  %r14w  %r14b
%r15  %r15d  %r15w  %r15b
```

#### Common Operands in Assembly Language
**3-Normal-Operands:**
```
1.immediate:
e.g: $-577 --> int -577, $0x1F --> A Hex 0x1F

2.regster:
e.g: %rax --> the Value in Regster %rax

3.storage:
e.g: 0x1F --> the Value in Storage 0x1F
```

**2-Addressing Mode**
```
For Example:

register:  value:
%rax       0x100
%rcx       0x1
%rdx       0x3

storage:   value:
0x100      0xFF
0x104      0xAB
0x108      0x13
0x10C      0x11

1.indexed addressing:
e.g: (%rax) --> the Value in Storage the Value in Regster %rax --> 0xFF
e.g: 4(%rax) --> the Value in Storage 4 + the Value in Regster %rax --> 0xAB
e.g: 260(%rcx,%rdx) --> the Value in Storage 260 + the Value in Regster %rcx, %rdx --> (0x104+0x1+0x3) = (0x108) = 0x13

2.proportion indexed addressing:
e.g: 0xFC(,%rcx,4) --> the Value in Storage 0xFC + 4*the Value in Regster %rcx --> (0xFC + 4x1) = (0x100) = 0xFF
e.g: (%rax,%rcx,4) --> the Value in Storage the Value in Regster %rax + 4*the Value in Regster %rcx --> (0x100 + 4*0x1) = 0x11 
```
#### 'mov' in Assembly Language
**The mov instruction copies the data to the specified location**

##### mov

```assembly
movl $0x4050,%eax        Immediate-->Register  4bytes
movw %bp,%sp             Register-->Register   2bytes
movb (%rdi,%rcx),%al     Memory-->Register     1byte
movb $-17,(%rsp)         Immediate-->Memory    1byte
movq %rax,-12(%rbp)      Register-->Memory     8bytes

```
##### movz
**Move from smaller storage units to larger storage units<br>Fill the remaining byte(s) in the destination with 0**
```assembly
movzbw $0xF0,%ax     In ax register: 0x00F0
```

##### movs
**Move from smaller storage units to larger storage units<br>Fill the remaining byte(s) in the destination with Sign bit(Just as mov)**
```assembly
movsbw $0xF0,%ax     In ax register: 0xFFF0
```

##### More Examples:

```assembly
movabsq $0x0011223344556677, %rax    ## %rax = 0011223344556677
movb    $-1, %al                     ## %rax = 00112233445566FF
movw    $-1, %ax                     ## %rax = 001122334455FFFF
movl    $-1, %eax                    ## %rax = 00000000FFFFFFFF
movq    $-1, %rax                    ## %rax = FFFFFFFFFFFFFFFF
```

```assembly
movabsq $0x0011223344556677, %rax    ## %rax = 0011223344556677
movb    $0xAA, %dl                   ## %dl = AA
movb    %dl, %al                     ## %rax = 00112233445566AA
movsbq  %dl, %rax                    ## %rax = FFFFFFFFFFFFFFAA
movzbq  %dl, %rax                    ## %rax = 00000000000000AA
```

##### Practice:
```c
src_t  *sp;
dest_t *dp;
/*
Assume that src_t and dest_t were typedef (Some Types of numbers)
Here We need to Use suitable Assembly language to Achieve:
(Supposed that the value of sp and dp are stored in %rdi and %rsi)
*/
*dp = (dest_t) *sp
```

```assembly
src_t          dest_t          Command               Note
long           long            movq(%rdi),%rax       NaN
                               movq %rax,(%rsi)

char           int             movsbl(%rdi),%eax     ## If We don't move a Unsigned number.
                               movl %eax, (%rsi)     ## We need to use 'movs'

char           unsigned        movsbl(%rdi),%eax     ## That is because In C programming Language
                               movl %eax, (%rsi)     ## If Cast type Change both the sign and the size
                                                     ## C Usually Change the Size first

unsigned char  long            movzbl(%rdi),%eax     ## If We need to move a Unsigned number.
                               movl %eax, (%rsi)     ## We need to use 'movz'

int            char            movl (%rdi),%eax      ## If We move a number from Large Size to Small Size
                               movb %al, (%rsi)      ## Move all to the register first, than move the least significant byte of the Small Size

unsigned       unsigned char   movl (%rdi),%eax      NaN
                               movb %al, (%rsi)

char           short           movzbw (%rdi),%ax     NaN
                               movw %ax, (%rsi)
```

#### Assembly with C Pointer
**Let's see this C Function exchange**

```c
// long.c
1 long exchange(long *xp, long y)
2 {
3     long x = *xp;
4     *xp = y;
5     return x;
6 }

```
**When We Compile it... ```gcc -Og -S long.c```**

```assembly
## long.s
_exchange:                             

1   pushq	%rbp
2   movq	%rsp, %rbp

3   movq	(%rdi), %rax
4   movq	%rsi, (%rdi)

5   popq	%rbp
6   retq

```
* **We Can See that in Assembly (long.s): Line3 and Line4 are correspond to Line3 and Line4 in long.c**
* **a pointer ```*xp``` in Assembly language is correspond to a register with ```()```Opreation**
