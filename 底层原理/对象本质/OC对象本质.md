# OC 代码本质  

1. 将OC代码转换为C++ 
    ```
    clang -rewrite-objc main.m -o main.cpp
    // 指定平台 
    scrun -sdk iphoneos clang -arch arm64 -rewrite-objc main.m -o main.cpp 
    ```

2. NSObject 第层实现
    ```
    @interface NSObject {                           struct NSObject_IMPL {
        Class isa;              --------------->            Class isa;
    }                                                }

    ```
    占用8个字节(一个指针)， 但内存对齐后，占用 16 个字节  
    ```
    // 获取NSObject 类的实例对象的大小  
    // 具体返回的是成员变量所占用的大小   
    class_getInstanceSize([NSObject class]);

    获得 obj 指针所指向内存的大小
    malloc_size(obj);


    ```


### 内存占用分析(可以将OC转为CPP代码分析)  
__具体也可以查看libmalloc库__   

```
// 定义OC 对象
@interface Person: NSObject 
@end 

@impermemation Person 
{
    int _age;
    int _height;
}
@end
 
// 转成 C 结构体(底层runtime是转为C 的)  
struct Person_IMP {
    Class isa;      // 8 字节
    int _age;       // 4
    int _height;    // 4
    int _no;        // 4
}                   // 20

// 获取内存大小的方法   

Person *p = [[Person alloc]init];

1. sizeof(struct Personal_IMP);             // 此种方式在编译器编译时就已经确定大小，编译完就是个常数
2. class_getInstanceSize([Person class])    // 此种方式获取的是对象内存
3. malloc_size(p)                           //此种方式回去的实际分配的内存


中间有个内存对齐问题 
通过以上方式获取内存大小： 
1. 24    此处由于内存对齐，此处是按照结构体内存对齐，会按照 8 字节对齐，所以  20 ----> 24   
2. 24   
3. 32    此处也是由于iOS系统是按照 16字节对齐(系统层内存对齐)， 20 -------> 32(16 * 2)

```


## OC 对象分类  
### instance 对象(实例对象）  
> 通过 alloc 出来的对象，每次调用 alloc 都会产生一个新的对象   

##### 包含内容 
1. isa 指针  
2. 其他成员变量(具体值信息)  



### class 对象(类对象)  
__每个类的类对象时唯一的__   

##### 获取类对象  
```
Class class1 = [obj class];
Class class2 = object_getClass(obj);
Class class3 = [NSObject class];
```

##### 包含内容 
1. isa 指针  
2. superclass 指针  
3. 类的属性信息(@property)，类的对象方法信息 (instance method)
4. 类的协议信息(protocol )，类的成员变量信息(ivar，成员变量的类型和名字等描述信息)
5. ....   



### meta-class(元类对象)  
> 描述类对象的信息  
__每个类只有一个__   



##### 获取元类对象 
```
// 传入的是类对象  
Class * metaClass = object_getClass([NSObject class]);

```

##### 包含内容   
1. isa 指针  
2. superclass 指针  
1. 类方法 (class-method) 




### 方法解析  
1. `objc_getClass(const char *ClassName)`  
    * 传入字符串类名  
    * 返回对应的类对象   
2. `objc_getClass(id obj)`
    * 传入obj 可能是 instance 对象， class 对象， meta-class 对象  
    * 返回值    
        * instance 对象， 返回class 对象  
        * class 对象， 返回 meta-class 对象 
        * meta-class对象， 返回 NSObject 的 meta-class对象  
3. `-class, + class`  
    * 返回类对象  


### 总结 
1. instance 的 isa 指向 class 
2. class 的 ISA 指向 meta-class 
3. meta-class 的ISA 指向  基类的 meta-class  
4. class 的 superclass 指向 父类的 class   
5. meta-class 的superclass 指向父类的 meta-class 
    * 基类的 meta-class的superclass 指向的时基类的 class  



#### instance 调用流程  






#### 属性参数 
* atomic  
    对属性加锁，多线程下线程安全，对方问速度有影响，默认值
* nonatomic 
    不加锁，非线程安全，不影响速度
* readwrite 
* readonly
    
* assign 
    直接赋值，默认值。
* retain  
    先release 原来的值，再retain新值   
* copy  
    先release 原来的值，再copy 新值   


#### ARC 对象饮用类型  
* __strong 
* __weak  
* __unsafe_unretained  
* __autoreleasing  
