# NSViewController 问题 


## 调用顺序
#### storyboard模式下 
```swift
override func loadView() {
    super.loadView()
    /// ....
}
override func viewDidLoad() {
    super.viewDidLoad()
    //....
}
```
1. loadView 
2. super.loadView()方法 
3. viewDidLoad() 
4. super.viewDidLoad() 
5. viewDidiLoad other 内容 
6. loadView 中的other 内容

#### 纯代码模式 
```swift 
override func loadView() {
    self.view = NSView()
    // ... 
}
override func viewDidLoad() {
    super.viewDidLoad()
    //....
}
```
> 正常的执行顺序 先执行完 `loadView` 方法 ，再执行`viewDidLoad` 方法

