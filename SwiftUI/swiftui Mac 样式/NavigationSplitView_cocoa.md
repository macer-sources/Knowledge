# NavigationSplitView 在 cocoa 上的应用 

## 简单用法 
```swift 
struct ContentView: View {
    var body: some View {
        NavigationSplitView {
            Text("Sidebar")
        } content: {
            Text("Primary View")
        } detail: {
            Text("Detail View")
        }
    }
}
```

* `NavigationSplitViewVisibility` 显示逻辑 
    * `.detailOnly`模式下，详细视图将占用您应用程序的所有可用屏幕空间。
    * `.doubleColumn`模式下，您将同时看到内容和详细信息视图。
    * `.all`模式下，系统将尝试显示所有三个视图（如果存在）。如果您没有内容视图（中间视图），它只会显示两个。
    * `.automatic`模式下，系统将根据当前设备和方向做它认为最好的事情。


* 显示样式 
    * `.prominentDetail`
    * `.balanced`
    * `.automatic`: 在 iPhone 上变为prominentDetail，而在 iPad 上变为.balanced
    ```swift 
    struct ContentView: View {
        var body: some View {
            NavigationSplitView {
                Text("Sidebar")
            } content: {
                Text("Content")
            } detail: {
                Text("Detail")
            }
            .navigationSplitViewStyle(.balanced)
        }
    }
    ```



> 关于隐藏 SideBar button 的方式 
```swift 
.toolbar(.hidden, for: .windowToolbar)
```
> mac 上隐藏顶部栏区域 
```swift 
.ignoresSafeArea()
.navigationBarBackButtonHidden()

```


* 自定义试图宽度 
```swift 
.navigationSplitViewColumnWidth(100)// 此种方式只是设置初始宽度， 宽度还是可变的 
.navigationSplitViewColumnWidth(min: 300, ideal: 300, max: 300) // 此种方式可以将宽度设为不可变的
.navigationSplitViewColumnWidth(min: 0, ideal: 0, max: 0) // 通过这种方式，可以彻底隐藏一栏
```





## 总结 
1. `SideBar`部分在默认情况下，可以实现`NavigationStack`的功能 
    ```swift 
    NavigationSplitView {
        List(1..<50) { i in
            NavigationLink("Row \(i)", value: i)
        }
         <!-- .listStyle(.sidebar)  将其变为 side bar-->
        .navigationDestination(for: Int.self) {
            Text("Selected row \($0)")
        }
        .navigationTitle("Split View")
    } detail: {
        Text("Please select a row")
    }
    ```