# 无用参数消除(dead-argument-elimination)优化  



## pass 调用

`opt -S -deadargelim testfile.ll -o -output2.ll`  

## 优化结果  

```c
// cat output2.ll
; ModuleID = 'testfile.ll' 

define i32 @test1(i32 %A) {
    %B = add i32 %A, 0
    ret i32 %B
}

define internal i32 @test(i32 %x) {
    ret i32 %x
}

define i32 @caller() {
    %A = call i32 @test(i32 123)
    ret i32 %A
}

```

## 工作原理详解  
本 pass 是针对参数进行优化，去除无用的参数。

