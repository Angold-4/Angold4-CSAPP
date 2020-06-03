### Fight Against Buffer Overflow Attacks In C

#### What Is Buffer Overflow Attacks:

**Let's see a very low-quality code to understand that:**
```c
/*getline.c*/
/*This is a very low-quality code*/
#include<string.h>
#include<stdlib.h>

char *get_line()
{
    char buf[4];
    char *result;
    gets(buf);
    result = malloc(strlen(buf)+1);
    strcpy(result, buf);
    return result;
}
```
**Then we use ```gcc -c``` to compile it and do```objdump```:<br>**
**We can see function get_line()**
```assembly
0000000000400720 <get_line>:
  400720: 53                push  %rbx
  400721: 48 83 ec 10       sub   $0x10,%rsp
  400425: 48 89 e7          mov   %rsp,%rdi
  400728: e8 73 ff ff ff    callq 4006a0<gets>
```
**When we run that program. If we input ```0123456789012345678901234```<br>We will find a segmentation fault(in early linux)**
**Then We use GDB to check, Here is the result:**
```
The return addr == 0x400076
Before we call func<gets> the value in %rbx == 0x400100
So at this time the stack is like:
00 00 00 00 00 40 00 76   --> return addr
00 00 00 00 00 40 01 00   --> push %rbx
```

**After complete everything when ```return result``` The stack is like that:**
```
00 00 00 00 00 40 00 34  --> return addr(+4)
33 32 31 30 39 38 37 36  --> 67890123
35 34 33 32 31 30 39 38  --> 89012345
37 36 35 34 33 32 31 30  --> 01234567
```
**We can see that when ```get_line``` func completed because we set the buffer only four bytes<br>**
**It cause buffer overflow -- The value in %rbx and the return addr were all changed<br>**


In real life If a malicious programmer make a -buffer-flow program and change the return addr to a function<br>that You don't want to call it
It will cause damages


**There are many ways to avoid that and Let's check that:**

#### 1. Stack Randomization
**In order to insert the attack code into the system<br>the attacker has to insert both the code and the pointer to this code and push it onto the stack<br>**
**So the Attacker need to know that where is the stack**

We use **Stack Randomization** to make every time we run a program. The Stack Position will be changed<br>

To achieve it. Every time A program beginning. We use func```alloca()``` to distribute a random number position<br>
The program did not use that area. But it will cause every time when we run the program.<br>The Position of stack were always Unpredictable<br>


**We can use a easy func to Prove that:**


```c
# include<stdio.h>

int main()
{
    long local;
    printf("local at %p\n", local);
    return 0;
}
```
> From CS:APP<br>
> When we run that func in 32-bit linux machine about 10000 times<br>
> The range of this addr is from ```0xff7fc59c``` to ```0xffffd09c``` about ```2**23```<br>
> When we run that func in 64-bit linux machine about 10000 times<br> 
> The range of this addr is from ```0x7fff0001b98``` to ```0x7ffffffaa4a8``` about ```2**32```<br>


#### 2.Stack Destruction Detection

**Although The Stack Randomization(ASLR) Can counter some forms of attack**<br>
**Random loss to brute force** You will never know what the power of attacker

The second line of defense for computers is **Stack Destuction Detection**<br>


**Let's see a normal stack:**
```
-------------------------------------
|                                   |
|___________________________________| --> Caller's shallow frame
|          Return address           |
-------------------------------------
|                                   |
|                                   | --> Buffer
|___________________________________|
|             Canary                |
------------------------------------- --> buf = %rsp
```
**Store a special Canary Value between any local buffer and the Caller's shallow frame**<br>

Let's see an example to figure out that:
```c
/*echo.c*/
#include <stdio.h>

void echo(){
    char buf[8];
    gets(buf);
    puts(buf);
}
```
**After we compile it:**
```assembly
## echo.s
_echo:                                  ## @echo
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset %rbx, -24
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	leaq	-24(%rbp), %rbx
	movq	%rbx, %rdi
	callq	_gets
	movq	%rbx, %rdi
	callq	_puts
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-16(%rbp), %rax
	jne	LBB0_2
## %bb.1:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	retq
LBB0_2:
	callq	___stack_chk_fail
                                        ## -- End function

```

**We can find that in line 11 at echo.s: ```movq  ___stack_chk_guard@GOTPCREL(%rip), %rax```<br>**
**It is like we create a special position pointer and mov it to %rax(This Special area is Marked as Read-Only)<br>**
**In the next line We move the value into %rax and this value is Canary<br>**


**Before the function ```puts()``` return. The gcc will check the Value of Canary's position whether it is as same as the Value store in The Special position pointer(In Line 20)<br>**

**If not equal(```jne```). Jump to LBB0_2(last line) and cause ```___stack_chk_fail```**



#### 3. Limit Executable Code Area

**Only the part of the memory that holds the code generated by the compiler needs to be executable.<br>The rest is limited to read and write only**
