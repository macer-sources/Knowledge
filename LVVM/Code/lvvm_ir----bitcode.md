# LVVM IR ------> bitcode 


### IR code 

```c
// cat test.ll

define i32 @mult(i32 %a, i32 %b) #0 {
    %1 = mul nsw i32 %a, %b 
    ret i32 %1
}

```


### 转换 

`llvm-as test.ll -o teset.bc`  




### 结果 
__由于 bitcode 是二进制形式的， 所以直接打开是乱码，需要通过 hexdump 等工具进行查看__  
