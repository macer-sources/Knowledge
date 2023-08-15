# Block
`void(^block)(void)`
`xrun -sdk iphoneos clang -arch arm64 -rewrite-objc main.m`

### 面试题 
1. block的原理是怎样的？ 本质是什么？  
2. __block 的作用是什么？ 有什么使用注意点？  
3. block 的修饰词为什么是 copy? 使用block有哪些注意点？ 
4. block在修饰 NSMutableArray ,需不需要添加 __block?  


### block 本质  
1. block 本质是一个 OC 对象，有 ISA指针  
2. block 封装了函数调用和函数调用环境(引用的变量等都会包含在内)  

### 值捕获问题 
1. 全局变量  
    不会捕获  
2. 局部变量  
    * auto 的 
        会捕获，值传递的方式，block结构体内部保存了值  
    * static 的  
        会捕获，引用传递的方式，block内部保存了内存地址。 
        原因： 因为block内部代码会转换为一个函数，所以涉及到垮还是调用，需要保存，由于static是静态的， 出了作用域还存在，所以只需要保存内存地址就行了  
3. self  
    会捕获，`self`是个局部变量， 
    ```
    -(void)test {
        void (^block)(void) = ^ {
            nslog(self);
        }
        block();
    }
    ```
    以上代码转换为C 的时候是这样的`test(Object *self, SEL _cmd)` self是个参数，所以是个局部变量，所以会被捕获  

### Block 类型  
Block 是个OC 对象  , 都继承自 `NSBlock`， `NSBlock` 又继承自 `NSObject`   

```
[block class];
[[block class]superclass];

```

* __`__NSGlobalBlock__(_NSConcreteGlobalBlock)`__   
    没有访问 auto 变量的 , 
    执行 copy 不做任何事情   

* __`__NSStackBlock__(_NSConcreteStackBlock)`__ 
    访问 auto  变量 (ARC下回进行优化，变为 MallocBlock)   
    栈上会自动销毁，所以有可能调用的不是想要的值     
    原理： 
        1. block结构体创建的内存是在栈上(虽然将局部变量传入了结构体)但整体还是在栈上     
        2. 栈上的可能会被销毁，所以整个结构体可能会被销毁    
        3. 虽然可以将block保存到全局变量，但是，全局变量只能保存block的内存地址，而其具体是否被释放，其是不知道的    
    * 栈上block ，调用 `copy`方法， 就会将其复制到 `堆`上    

* __`__NSMallocBlock__(_SNConcreteMallocBlock)`__  
    `__NSStackBlock__` 调用 `copy` 方法  `[block copy]`, 此种类型调用 copy 引用计数 + 1  




#### 内存分布  

```
程序区域(代码段)   


数据区域(数据段)
                            <---------- GlobalBlock

堆(动态申请内存，开发者申请和释放内存)  
                            <---------- MallocBlock


栈(存放局部变量)
                            <---------- StackBlock  


```   

#### Block 的 copy  
1. block 作为函数返回值， 自动进行copy操作  
2. block 赋值给`__strong`自动进行 copy操作  
3. block作为 Cocoa API 中方法名含有 `usingBlock` 的方法参数时   
4. block作为 GCD 参数  


__Block 只要是在栈上的， 无论强指针还是弱指针引用外部对象，都不会__   




### 循环引用解决方案  
1. __block  
    需要在内部将 __block 修饰的变量 = nil  
2. __weak 
3. __unsafe_unretained: 不安全  

