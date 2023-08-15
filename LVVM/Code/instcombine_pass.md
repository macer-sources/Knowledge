# instcombine 优化器(指令合并)


## 1. test code 

```c
// cat testfile.ll 

define i32 @test1(i32 %A) {
    %B = add i32 %A, 0
    ret i32 %B
}

define internal i32 @test(i32 %x, i32 %dead) {
    ret i32 %x
}

define i32 @caller() {
    %A = call i32 @test(i32 123, i32 456)
    ret i32 %A
}

```

## 调用 优化 
`opt -S -instcombine testfile.ll -o output1.ll`  


## 优化结果  

```c
// cat output1.ll

; ModuleID = 'testfile.ll' 

define i32 @test1(i32 %A) {
    ret i32 %A
}

define internal i32 @test(i32 %x, i32 %dead) {
    ret i32 %x
}

define i32 @caller() {
    %A = call i32 @test(i32 123, i32 456)
    ret i32 %A
}
```


## 工作原理详解  
运行`instcombine`优化器pass， 会将指令进行合并. `%B = add i32 %A, 0; ret i32 %B` 被优化为了 `ret i32 %A`. 并没有改变原来的代码。而是产生了新的代码   

