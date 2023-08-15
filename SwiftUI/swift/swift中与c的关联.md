
# swift 中定义及掉用 c 函数
```swift 
public typealias `func` = @convention(c) (Int32, UnsafeMutablePointer<Void>) -> Void


//实现一个c 函数 
func myFunc(event: Int32, context: UnsafeMutablePointer<Void>) {
    //...
    // TODO: 拿到掉用者对象
    let obj: MyController = unsafeBitCast(context, MyController.self)
}
var callback: `func` = myFunc
// 如何在swift中掉用c 并且在 c 函数中拿到掉用者对象呢 


// TODO: 掉用
let selfObj = unsafeBitCast(self, UnsafeMutablePointer<Void>.self)


```