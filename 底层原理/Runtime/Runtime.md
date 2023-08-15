# Runtime 


## 常见面试题  
1. OC 的消息机制 
2. 消息转发机制流程  
3. 什么是Runtime? 平时使用过？  


## isa 详解  
共用体(union)， 使用位域来存储更多数据     

### class_rw_t 
其中的 `methods`， `properties`, `protocols` 是 __二维数组__， 是可读可写的，包含了类的初始化内容、分类的内容  

注意： 二维数组的原因是需要动态添加方法  



### class_ro_t 
其中的`baseMethodList`,`baseProtocols`,`ivars`,`basProperties`是一维数组，只读的，包含了类的初始化内容


### method_t 
1. SEL: 代表方法名、函数名，底层结构跟 `char*`类似     
2. IMP: 就是函数实现的首地址   
3. `type`:  
    默认情况下，OC 中每个方法都会传入两个参数`id 类型 self`和 `SEL 类型的 _cmd`
    
    ```
    // i24@0:8i16f20
    -(int)test:(int) age height:(float)height;  
    i   : 是返回值 int 类型  
    24  : 总共占用字节  
    @   :id 类型的self   
    0   : id 类型开始地址(占8 字节)
    :   : SEL cmd 类型
    8   : SEL 开始位置(占 8 字节)
    i   :  参数 int 类型
    16  :  int类型参数开始位置 
    f   :   参数 float 类型
    20  :   float 参数的开始位置   

    0  1  2  3  4  5  ..8 ....  16  ....20 ....24 
    id类型开始位置        SEL开始  int 参数 float参数 

    ```
#### Type Encoding 
iOS提供了叫`@encode` 的指令，可以将具体的类型标识成长字符串编码  
```





```



## 方法缓存  
* Class 内部结构中有一个方法缓存(`cache_t`), 用 __散列表__ 来缓存曾经调用过的方法， 可以挺高方法的查找速度   

```
struc cache_t {
    struct bucket_t *_buckets;  // 散列表
    mask_t _mask;   // 散列表的长度 - 1 (散列表的长度是大于缓存方法数的)
    mask_t _occupied; // 已经缓存的方法数量

}
```
__注意： 当缓存不足时，会直接将缓存清空， 重新来一遍__  


#### 散列表原理 
原理： 就是牺牲内存空间，换取效率  
 快速获取值： key & _mask =  索引  


#### 遍历 bucket_t
```

cache_t cache = goodStudentClass->cache;
bucket_t * buckets = cache._buckets; 

for(int i = 0; i < cache._mask + 1; i++) {
    bucket_t b = buckets[i]; 

}
```


## objc_msgSend 
> OC 的所有方法调用都是转换为 `objc_msgSend` 函数调用  


#### 三大阶段  
1. 消息发送  
2. 动态方法解析  
    * 类方法添加 
    * 对象方法添加    

3. 消息转发  
    1. 首先调用 `forwardingTargetForSelector`方法 
        此方法返回如果一个对象， 那么久调用`objc_msgSend`给这个对象  
    2. 如果上边这个方法返回nil, 调用`methodSignatureForSelector`方法 
        此方法返回一个方法签名，并且包装成下面这个方法的参数 
    3. 上边这个方法返回不为nil,调用`forwardInvocation` 
        在此处更改参数的target属性，实现调用   


#### super  
`[super run]` 
1. super 消息的接受者仍然是子类对象
2. 从父类开始查找方法的实现  

`object_getClass(xxx)` 方法，只和`xxx`对象有关，所以`[self class] 和【super class】`是相同的  

