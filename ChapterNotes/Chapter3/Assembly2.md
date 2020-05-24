## Assembly Language

**Some Notes For CS:APP Chapter3 by Jiawei Wang**

### Arithmetic and logical operations

#### LEA
**lea means "Load Effective Addr"**
```assembly
## Diff Between lea and mov

leaq S, D   -->  &S -> D
movq S, D   -->   S -> D

## for example:

leaq (%rdi, %rsi), %rax  --> That means %rax = %rdi + %rsi
movq (%rdim %rsi), %rax  --> That means %rax = (%rdi + %rsi) (In Storage)

lea:
## Although the form of the instruction is to read data from the memory to the register
## It does not actually access the memory but returns the pointer to the target register
```
**Another important Usage of lea is that it can Briefly describe common arithmetic operations:<br>**
**let's see this C program:**
```c
//scale.c

long scale(long x, long y, long z){
    long t = x + 4 * y + 12 * z;
    return t;
}
```
**Compile it:**
```assembly
## scale.s(brief)
##  x in %rdi, y in %rsi, z in %rdx 

scale:
    leaq (%rdi,%rsi,4), %rax   --> x + 4*y
    leaq (%rdx,%rdx,2), %rdx   --> z + 2*z
    leaq (%rax,%rdx,4), %rax   --> x + 4*y + 4*(3*z) = x + 4*y + 12*z
    ret
```

#### INC DEC NEG NOT
**All of These Were One-Parameter Oprand**

```assembly
inc D   --> D += 1
dec D   --> D -= 1
neg D   --> D = -D
not D   --> D = ~D
```

#### ADD SUB IMUL XOR OR AND
**All of These Were Two-Parameter Oprand but little diff with MOV and LEA**

```assembly
add  S,D  --> D = D + S
sub  S,D  --> D = D - S
imul S,D  --> D = D * S
xor  S,D  --> D = D ^ S
or   S,D  --> D = D | S
and  S,D  --> D = D & S

```

#### Shift Operation

```assembly
sal k,D   --> D = D << k
shl k,D   --> D = D >> k
sar k,D   --> D = D >> k(Algorithms)
shr k,D   --> D = D << k(Logic)

```
**Let's See an example to understand some details there:**

```c
//shift.c

long shift_left4_rightn(long x, long n)
{
    x <<= 4;
    x >>= n;
    return x;
}

```

```assembly
## shift.s(brief)
## x in %rdi, n in %rsi

arith:
    movq  %rdi, %rax  ## Get the Value of x
    salq  $4, %rax    ## x <<= 4
    movl  %esi, %ecx  ## Get the Value of n
    sarq  %cl, %rax   ## x >>= n(Only the Last byte in register(%cl) represent the value to shift)

```
**In Shift Operation: Only immediate and value in register %cl Can be used as "Shift Number"**


#### Another Example
**Mix Program to understand these oprand below**
```c
//arith.c

long arith(long x, long y, long z)
{
    long t1 = x ^ y;
    long t2 = z * 48;
    long t3 = t1 & 0x0F0F0F0F;
    long t4 = t2 - t3;
    return t4;
}

```
**Compile it  ```✗ gcc -Og -S arith.c```**
```assembly
1 _arith:                                 ## @arith
2   pushq	%rbp
3   movq	%rsp, %rbp
4   xorl	%edi, %esi
5   shlq	$4, %rdx
6   leaq	(%rdx,%rdx,2), %rax
7   andl	$252645135, %esi        ## imm = 0xF0F0F0F
8   subq	%rsi, %rax
9   popq	%rbp
10  retq

                                        ## -- End function

```
```
Line 2 3 and Line 9 push the Program register into stack
Because this program is not runable(without main())

Line 4:  t1 = x ^ y
Line 5:  2**4 * z(16*z)
Line 6:  3 *(16*z)
Line 7:  t3 = t1 & 0xF0F0F0F
Line 8:  t4 = t2 - t3

We can notice that the Compiler divide the Opration "z * 48" into two steps:
1)z = z >> 4 --> 2)z * 3
```

#### Special Oprations
**In x86-64. We can caculate two 64-bits numbers.<br>But as we know that when two 64-bits numbers do some caculation(mul div)<br>
We need almost 128-bits to store the answer. But in x86-64. We don't have 128-bits register**

**Let's see How x86-64 assembly achieve it:**
```c
// uint.c
typedef unsigned __int128 uint128_t;

void store_uprod(uint128_t *dest, unsigned long x, unsigned long y)
{
    *dest = x * (uint128_t) y
}

/*
*Let me do a brief introduction to this uint.c. 
*Then we can check the assembly form to see how the compiler to achieve that
*/
```
* **We use the 128-bits type that GCC provided to apply the 128-bits number**
* **in function store_uprod: We store the value of x ```*``` y(Forcibly convert the result to a 128bit number) in the storage of pointer ```*dest```**


```assembly
## dest in %rdi, x in %rsi, y in %rdx
_store_uprod:                           ## @store_uprod
1   pushq	%rbp
2   movq	%rsp, %rbp
3   movq	%rdx, %rax
4   mulq	%rsi
5   movq	%rdx, 8(%rdi)
6   movq	%rax, (%rdi)
7   popq	%rbp
8   retq
                                        ## -- End function

```
* **The way gcc compiler handles 128bit results is that to use two register(%rax and %rdx) to store separately the lower 8 bytes and the upper 8 bytes(In Line 5 and 6)**
* **Another interesting thing is that the way to caculate two 64bit number:<br>
In Line 3 we directly move y into %rax register and In Line 4 we have a one-parameter ```mul``` command<br>
The ```mul``` instruction at this time requires that one of the two 64bit parameters must be in register %rax and another need to be the only parameter of mul**


**It is a little bit difference between multiply and div in 64-bit opration. Let's see an example of Div:**

```c
// div64.c
void remdiv(long x, long y, long *qp, long *rp)
{
    long q = x / y;
    long r = x % y;
    *qp = q;
    *rp = r;
}
```
```assembly
## div64.s
## x in %rdi, y in %rsi, qp in %rdx, rp in %rcx
_remdiv:                                ## @remdiv
1   pushq	%rbp
2   movq	%rsp, %rbp                 
3   movq	%rdx, %r8                   ## Copy qp 
4   movq	%rdi, %rax                  ## Move x to lower 8 bytes of dividend
5   cqto                                ## Sign-extend to upper 8 bytes of dividend
6   idivq	%rsi                        ## divide by y
7   movq	%rax, (%r8)                 ## Store quotient at qp
8   movq	%rdx, (%rcx)                ## Store remainder at rp
9   popq	%rbp
10  retq
                                        ## -- End function

```
* **The signed division instruction idivl takes registers```%rax```(upper 64 bits) and```%rdx```(lower 64 bits) as the dividend, and the divisor is given as the operand of the instruction.**
* **The instruction stores the quotient in register```%rax``` and the remainder in register```%rdx```**

* **We can see that in line 4 we move the x into register ```%rax```**
* **In line 5 the ```cqto``` command means that to extand the 64bit sign bit of ```%rax``` to ```%rdx```**


**In Unsigned long div: We don't need to use ```cqto```**

```c
// div64.c
void remdiv(unsigned long x, unsigned long y, unsigned long *qp, unsigned long *rp)
{
    unsigned long q = x / y;
    unsigned long r = x % y;
    *qp = q;
    *rp = r;
}
```
```assembly
## div64.s
## x in %rdi, y in %rsi, qp in %rdx, rp in %rcx
_remdiv:                                ## @remdiv
1   pushq	%rbp
2   movq	%rsp, %rbp                 
3   movq	%rdx, %r8                   ## Copy qp 
4   movq	%rdi, %rax                  ## Move x to lower 8 bytes of dividend
5   movl    $0, %rbx                    ## Unsigned division we don't need cqto
6   divq	%rsi                        ## divide by y
7   movq	%rax, (%r8)                 ## Store quotient at qp
8   movq	%rdx, (%rcx)                ## Store remainder at rp
9   popq	%rbp
10  retq
                                        ## -- End function

```
