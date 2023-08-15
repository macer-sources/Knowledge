# 将 bitcode 转换为特定目标架构的汇编码   


### 转换 

将bitcode 代码转换为 汇编码有两种方式： 

1. `llc test.bc -o test.s`  
2. `clang -S test.bc -o test.s -formit-frame-pointer`   
    使用 clang 前端  





### 结果示例  


```s  

.text
.file "test.bc"
.globl mult
.align 16, 0x90
.type mult,@function
mult:
.cfi_startproc
# BB#0:
Pushq %rbp
.Ltmp0:
# @mult
.cfi_def_cfa_offset 16
.Ltmp1:
.cfi_offset %rbp, -16
movq %rsp, %rbp
.Ltmp2:
.cfi_def_cfa_register %rbp
imull %esi, %edi
movl %edi, %eax
popq %rbp
retq
.Ltmp3:
.size mult, .Ltmp3-mult
.cfi_endproc



```