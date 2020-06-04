## Assembly Language

**Some Notes For CS:APP Chapter3 by Jiawei Wang**

### Floating Point Architecture

#### 1.x86-64's Development of FPA

* **In 1997: Pentium/MMX(Matrix Math eXtensions)<br>-- Introduced SIMD(Single Instruction Multiple Data) and %mm Registers(64-bit)<br>**
> What is SIMD: Perform the same operation on multiple different data in parallel<br>![SIMD](https://github.com/Angold-4/Sources/raw/master/SIMD.png)

* **In 1999: Pentium 3 -- Add SSE(Streaming SIMD Extention) and %xmm Registers(128-bit)**

* **In 2011: Intel Core i7 Sandy Birdge -- Add AVX(Advanced Vector Extension) and %ymm Registers(256-bit)**

* **In 2013: Intel Core i7 Haswell -- AVX -> AVX2 added more instructions and instruction formats**


#### 2.AVX
**AVX floating point architecture allows data to be stored in 16 YMM registers<br>**
**When do Scalar data manipulation These 16 %ymm only store floating point**

```
256     128     64    
%ymm0   %xmm0   %mm0
%ymm1   %xmm1   %mm1
%ymm2   %xmm2   %mm2
%ymm3   %xmm3   %mm3
%ymm4   %xmm4   %mm4
%ymm5   %xmm5   %mm5
%ymm6   %xmm6   %mm6
%ymm7   %xmm7   %mm7
%ymm8   %xmm8   %mm8
%7mm9   %xmm9   %mm9
%ymm10  %xmm10  %mm10
%ymm11  %xmm11  %mm11
%ymm12  %xmm12  %mm12
%ymm13  %xmm13  %mm13
%ymm14  %xmm14  %mm14
%ymm15  %xmm15  %mm15
```


### Command In AVX2
**Let's see an example:**
```c
/*float.c*/
float float_mov(float v1, float *src, float *dest){
    float v2 = *src;
    *dest = v1;
    return v2;
}
```

**```➜  Chapter1 (master) ✗ gcc -Og -S -mavx2 float.c```**

```assembly
/*float.s*/
v1 in %xmm0, src in %rdi, dst in %rsi
float_mov:
    vmovaps %xmm0, %xmm1
    vmovss (%rdi), %xmm0
    vmovss  %xmm1, (%rsi)
    ret
```
