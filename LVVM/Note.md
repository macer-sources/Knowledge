# LVVM Cookbook  

## 第一章 LVVM 设计与使用  


### 1. 模块化设计   
LVVM  的设计目标是成为一系列的库。LVVM的代码有三种表现形式： __内存编译器中的IR__, __存于磁盘的bitcode__, __用户刻度的汇编码__  LVVM IR 是基于静态单赋值(是一种特殊的中间码----每个变量仅被赋值一次)，并且提供了类型安全性、底层操作性、灵活性，能够清晰表达绝大数高级语言。     


#### 优化示例  
1. 示例代码    

```
cat testfile.ll 

define i32 @test1(i32 %A) {
    %B = add i32 %A, 0
    ret i32 %B
}

define internal i32 @test(i32 %X, i32 %dead) {
    ret i32 %X
}
define i32 @caller() {
    %A = call i32 @test(i32 123, i32 456) 
    ret i32 %A
}

```
2. 优化指令(指令合并)     
`opt -S -instcombine testfile.ll -o output.ll`   

3. __指令合并__ 优化结果   

```
cat output.ll 
; ModuleID = 'testfile.ll'

define i32 @test1(i32 %A) {
    ret i32 %A
}
define internal i32 @test(i32 %X, i32  %dead) {
    ret i32 %X
}
define i32 @caller() {
    %A = call i32 @test(i32 123, i32 456)  
    ret i32 %A
}

```

4. opt 进行无用参数消除(dead-argument-elimination)优化  
`opt -S -deadargelim testfile.ll -o output2.ll`  

5. __消除无用参数优化__结果  

```
; ModuleID = testfile.ll  

define i32 @test1(i32 %A)  {
    %B = add i32 %A , 0
    ret  i32 %B
}

define  internal i32 @test(i32 %X) {
    ret i32  %X
}

define i32 @caller() {
    %A = call i32 @test(i32 123) 
    ret i32 %A
}

```







