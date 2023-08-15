# 转换 LLVM IR ,并对其代码进行多个优化  


## c 代码 

```c
int mult() {
    int a = 5;
    int b = 3;
    int c = a * b;
    return c;
}


```



## 将c 转换为 LLVM IR 

```c
// clang -emit-llvm -S multiply.c -o multiply.ll  



; ModuleID = 'multiply.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"
; Function Attrs: nounwind uwtable
define i32 @mult() #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 3, i32* %b, align 4
  %1 = load i32* %a, align 4
  %2 = load i32* %b, align 4
  %3 = mul nsw i32 %1, %2
  store i32 %3, i32* %c, align 4
  %4 = load i32* %c, align 4
ret i32 %4 }


```

## 使用opt 进行转换  
优化内存访问(将局部变量从内存提升到寄存器)  

`opt -mem2reg -S multiply.ll -o multiply1.ll`  



## 查看优化结果 

```c

; ModuleID = 'multiply.ll'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"
; Function Attrs: nounwind uwtable
define i32 @mult(i32 %a, i32 %b) #0 {
  %1 = mul nsw i32 %a, %b
ret i32 %1 }


```
