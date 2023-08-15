# 将 c 语言编译为 llvm ir   



## c 源码  

```c

int mult() {
    int a = 5;
    int b = 3;
    int c = a * b;
    return c;
}


```



## 编译 
#### 方式1 

`clang -emit-llvm  -S testfile.c -o testfile.ll`

#### 方式2  


`clang -cc1 -emit-llvm testfile.c -o testfile.ll`





## 编译结果-- IR 

```c
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
    ret i32 %4 
}




```