1. async / await 
```swift 
// 方便调用异步任务的方式 
Task {
  await request()
}

async {
  await request()
}

``

2. 主线程运行 
```swift 
方式1: @MainActor 标记方法 
@MainActor func loadUI() {}

方式2: MainActor 运行 , 只能在异步任务中使用

await MainActor.run {

}
```
