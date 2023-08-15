# Category 


## 面试题  
1. Category 使用场景？ 
2. 实现原理   
3. 与 extension的区别？ 
    * extension 在编译时，数据已经包含在类信息中  
    * category是在运行时，合并到类信息中  
4. 有load方法？ load方法调用时机？ load方法能继承吗？ 
    * 存在继承的情况，主要是OC 是消息发送机制，消息发送的原理导致 (有可能调用的是分类的)    

5. load 与  initialize的区别？ 在category中的调用顺序？ 
6. 能否添加成员变量？


## 原理  
1. 代码编译时， 每个分类都会变为 `_category_t` 结构体(__注意：编译完成并没有合并到原类中__)
   

### 加载过程  
1. 通过runtime加载某个类的所有category数据  
2. 把所有category的方法啊， 属性，协议数据，合并到一个大数组中，后面参与编译的caegtory会在数组的前面  
3. 将合并后的分类数据， 插入到类原来数据之前  

#### 源码顺序  
1. objc-os.mm
    * _objc_init
    * map_images 
    * map_images_nolock  
2. objc-runtime-new 
    * _read_images 
    * remethodizeClass
    * attachCategoryies 
    * attachLists 
    * realloc, memmove, memcopy  



### Load 方法  
1. category分类中有 load 方法  
2. 在runtime 加载类和分类时调用  



### initilize  
> 在类第一次接收到消息调用   
1. 分类中如果有， 分类和 本类中，只会调用一个   
2. 此种方式调用采用的时 `objc_msgSend()`   
3. 先调用父类的， 再调用子类的  



### load 与 initlize 的区别 
1. 调用方式  
    * load 是根据函数地址调用  
    * initialize是通过 objc_msgSend 调用  

2. 调用时刻  
    * load 是在runtime加载类、分类的时候调用(只会调用一次)   
    * initialize 是类第一次接收到消息的时候调用， 每个类只会initialize一次(父类可能会调用多次)
3. 调用顺序  
    * load  
        * 先调用类的load  
        * 先笔译的类，优先调用  
        * 调用自雷的load之前，会先调用父类的load   
        * 再调用分类的  
        * 先编译的分类，优先调用  
    * initialize  
        * 先初始化父类的， 再初始化子类的(可能最终调用的还是父类的)  
        



