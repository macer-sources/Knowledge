# RunLoop  

### 面试题  
1. runloop 项目中的使用？ 
2. runloop 内部实现逻辑  
3. runloop和现成的关系？ 
4. timer 和 runloop的关系？  
5. 程序中添加每3秒响应一次的NSTimer, 当拖动tbaleview 的时候 ，timer 无法响应要怎么处理 ？  
6. runloop 是怎么响应用户操作的，具体流程是怎样的？ 
7. runloop的集中状态？ 
8. runloop的mode 作用是什么？ 

#### runloop应用范围  
1. 定时器  
2. GCD aync main  queue 
3. 网络处理  
4. autoreleasePool 
5. 事件响应、手势识别，界面刷新   



#### Runloop 和线程关系   
1. 每条线程都有一个与之对应的runloop对象  
2. runloop保存咋一个全局的Dictionary 中， 线程作为 key ， runloop作为value  
3. 线程刚创建时并没有runloop对象，runloop会在 __第一次获取时创建__   
4. runloop 会在线程结束时销毁  
5. 主线程runloop已经自动获取， 子线程默认没有开启runloop  

#### runloop 相关的类  
1. `CFRunLoopRef`
2. `CFRunLoopModeRef`
3. `CFRunLoopSourceRef`
4. `CFRunLoopTimerRef`
5. `CFRunLoopObserverRef`  


#### runloop 运行逻辑  
1. Source 0     
    * 触摸事件处理  
    2. performSelector:onThread  
2. Source 1 
    * 基于Port 的线程间通信  
    * 系统事件捕捉   
3. Timer 
    * NSTimer  
    * performSelector:withObject:afterDelay:  
4. Observer 
    * 监听runloop 状态    
    * UI刷新(BeforeWaiting)  
    * autoreleasePool   


__注意:__  
    UIScrollView 滚动时，会先推出当前的`DefaultMode`,切换到 `UITracking`  

#### Runloop 流程
1. 通知Observer: 进入Loop  
2. 通知Observer: 即将处理 Timers  
3. 通知Observer: 即将处理Sources  

4. 处理Blocks  
5. 处理Source0(可能会再次处理Blocks)  
6. 如果存在Source1, 就跳转到第 8 步   
7. 通知Observer:开始休眠   

8. 通知Observer: 结束休眠(被某个消息唤醒)  
    * 处理Timers  
    * 处理 GCD Async Main Queue 
    * 处理Source 1 
9. 处理Blocks  
10. 根据前面的执行结束，决定如何操作  
    * 回到第2步  

11. 通知Observers: 推出Loop  



#### NSTimer 滚动时停止  
```
NSTimer timer; 

[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];;

NSRunLoopCommonModes 并不是一个真的模式， 包含了两个模式，只是一个标记  

timer 能在 _commonModes 数组中存放的模式下工作  

```


#### 线程保活 
##### 内存泄露问题 
1. 线程创建时，容易强引用   
```
// 通过以下方式实现  
self.thread = [[NSThread alloc]initWithBlock:^{

    // 此种方式是专门用来开启永不停止的runloop  
    [[NSRunLoop currentRunLoop]run];  

    // 正确方式 
    while(weakSelf && weakSelf.stop) {
        [[NSRunLoop currentRunLoop]runMode:.Default beforeDate:[NSDate distantFuture]];  
    }
}];


```
2. 对象释放时， runloop导致线程没有释放  
    ```
     
    -(void)stop {
        self.stop = yes;
        CFRunLoopStop(CFRunLoopGetCurrent());

    }
    -(void)test {
        [self performSelector:@selector(stop) ontThread:self.thread withObject:nil waitUntilDone:YES]; 
        waitUntilDone: 如果是No， 不等待子线程执行完毕 
    }
     
    ```

__注意__  
通过`run` 方法启动的runloop时停止不掉了，内部是无限循环的调用`runModel:beforeDate`
 
#### 封装  
```Objective-C  
@interface LoopThread: NSObject
-(void)run; 
-(void)stop;
-(void)executeBlock:(void (^(void)))block;
-(void)executeTaskWithTarget:(id)target action:(SEL)action object:(id)object;
@end 

@implementation  LoopThread 
-(instancetype)init { 

    self.stop = NO; 
    __weak typeof(self) weakSelf = self;
    self.thread = [[NSThread alloc]initWithBlock:^{
        [[NSRunLoop currentRunLoop]addPort:[[NSPort alloc]init] forMode:NSDefaultRunLoopMode]; 
        while(weakSelf && weakSelf.stop) {
            [[NSRunLoop currentRunLoop]runMode:NSDefaultRunLoopMode beforeDate:[NSData distantFuture]]; 
        }
    }];
}
- (void)run {
    if(!self.thread) return;
    [self.thread start];
}

-(void) stop { 
    if(!self.thread) return;
    [self performSelector:@selector(threadStop) onThread:self.thread withObject:nil waitUntilDone:YES];
}

-(void) threadStop {
    self.stop = YES; 
    CFRunLoopStop(CFRunLoopGetCurrent()); 
    self.thread = nil;
}

-(void)executeBlock:(void (^(void)))block {
    if(!self.thread || !block) return;
    [self performSelector:@selector(__executeTask) onThread:self.thread withObject:nil waitUntilDone:NO]; 
}

-(void)__executeTask()block {
    block();
} 

-(void)dealloc {
    [self stop];
}

@end
```


__C语言实现方式更加简单__  

```
self.thread = [[NSThread alloc]initWithBlock:^{ 
    // 创建上下文，并初始化
    CFRunLoopSourceContext context = {0}; 


    // 创建source 
    CFRunLoopSourceRef source = CFRunLoopSourceCreate(,0,&context);

    // 添加source  
    CFRunLoopAddSource(CFRunLoopGetCurrent(),source, kCFRunLoopDefaultMode);
    
    // 销毁source 
    CFRelease(source);

    // 启动 
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1.0e10, false);
    // 最后一次参数，代表是否执行完source就直接返回
}



```
