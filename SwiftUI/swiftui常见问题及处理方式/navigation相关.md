
# Navigation 相关 


## 参考 
https://gist.github.com/mbrandonw/f8b94957031160336cac6898a919cbb7


## 问题1 
#### 问题描述 
```swift 
@State private var mainNavigationPath: NavigationPath = NavigationPath.init()
@State private var detailNavigationPath:NavigationPath = NavigationPath.init()

NavigationStack(path: $mainNavigationPath) {
        HStack(spacing: 0) {
            ContentView()
            DetailViewNavigation()
        }
        .navigationDestination(for: String.self) { path in
            Rectangle()
                .fill(.yellow)
                .onAppear {
                    debugPrint("\(path)")
                }
        }
    }

    @ViewBuilder
    func DetailViewNavigation() -> some View {
        NavigationStack(path: $detailNavigationPath) {
            StorageDetailView()
        }
        .frame(width: 300)
        .background(Color.white)
    }
```
#### 问题表现
```
A NavigationLink is presenting a value of type “String” but there is no matching navigationDestination declaration visible from the location of the link. The link cannot be activated.

Note: Links search for destinations in any surrounding NavigationStack, then within the same column of a NavigationSplitView.

```


#### 解决方案
```swift 
struct ContentView: View {
    @State private var mainNavigationPath: NavigationPath = NavigationPath.init()
    @State private var detailNavigationPath:NavigationPath = NavigationPath.init()

    
    
    var body: some View {
        NavigationStack(path: $mainNavigationPath) {
                HStack(spacing: 0) {
                    MainContentView()
                    DetailViewNavigation()
                }
                .navigationDestination(for: String.self) { path in
                    Rectangle()
                        .fill(.yellow)
                        .onAppear {
                            debugPrint("\(path)")
                        }
                }
            }
    }
    
    @ViewBuilder
    func MainContentView() -> some View {
        VStack {
            NavigationLink("MainContentView", value: "detail")
//            Text("MainContentView")
//                .onTapGesture {
//                    mainNavigationPath.append("detail")
//                }
        }
    }
    
    @ViewBuilder
    func DetailViewNavigation() -> some View {
        NavigationStack(path: $detailNavigationPath) {
            VStack {
                Text("DetailViewNavigation")
            }
        }
        .frame(width: 300)
        .background(Color.white)
    }
}

```
> navigationDestination 必须写在 `NavigationStack`的 content 内容view 上，不能直接写在 `NavigationStack` 上
> 直接修改`mainNavigationPath.append("detail")` pop的时候会出问题， 除非手动remove，利用系统的pop 按钮就会出问题



#### 示例代码 
> 此处实例的是问题存在
```swift 

struct NavigationDemo: View {
    @State private var path:[String] = []
    var body: some View {
        HStack {
            Rectangle()
                .fill(.red)
                .frame(width: 200)
                .frame(maxHeight: .infinity)
            NavigationStack(path: $path) {
                VStack {
                    Text("Main")
                        .onTapGesture {
                            withAnimation {
                                // TODO: 如果没有正确使用navigation，还是能push，只是push 的是一个不知道哪里来的东西
                                path.append("1")
                            }
                        }
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
                .navigationDestination(for: String.self) { path in
                    DetailView(path: $path, name: "Detail \(path)")
                }
            }
        }
    }
}

struct DetailView: View {
    @Environment(\.dismiss) private var dismiss
    @Binding var path: [String]
    let name: String
    var body: some View {
        VStack {
            Text("pop")
                .onTapGesture {
                    dismiss()
                }
                .padding(.bottom, 50)
            Text(name)
                .onTapGesture {
                    path.append("Detail \(path.count)")
                }
        }
        .background(.white)// TODO: mac back 是透明的，不是白色的
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .navigationBarBackButtonHidden() // TODO: 此处会隐藏navigtiaon bar 和back button，
    }
}



struct NavigationDemo_Previews: PreviewProvider {
    static var previews: some View {
        NavigationDemo()
    }
}


```


> 左右两个`NavigationStack` 的实现方式 
```swift 
struct NavigationDemo: View {
    @State private var path:[String] = []
    @State private var detailPath:[String] = []
    var body: some View {
        HStack {
            Rectangle()
                .fill(.red)
                .frame(width: 200)
                .frame(maxHeight: .infinity)
            NavigationSplitView {
                NavigationStack(path: $path) {
                    ZStack {
                        Rectangle()
                            .fill(.yellow)
                            .frame(maxWidth: .infinity, maxHeight:.infinity)
                        Text("Main")
                            .onTapGesture {
                                withAnimation {
                                    path.append("1")
                                }
                            }
                    }
                }
                .ignoresSafeArea()
                .navigationBarBackButtonHidden()
                .navigationDestination(for: String.self) { path in
                    DetailView(path: $path, name: "Detail \(path)")
                }
            } detail: {
                NavigationStack(path: $detailPath) {
                    ZStack {
                        Rectangle()
                            .fill(.blue)
                            .frame(maxWidth: .infinity, maxHeight:.infinity)
                        Text("Detail")
                            .onTapGesture {
                                withAnimation {
                                    detailPath.append("1")
                                }
                            }
                    }
                }
                .ignoresSafeArea()
                .navigationBarBackButtonHidden()
                .navigationDestination(for: String.self) { path in
                    DetailView(path: $detailPath, name: "Detail View \(path)")
                }
            }
            .navigationSplitViewColumnWidth(400)
            .navigationBarBackButtonHidden()
            .navigationSplitViewStyle(.prominentDetail)
            .toolbar(.hidden, for: .windowToolbar)
            .ignoresSafeArea()
        }
    }
}

struct DetailView: View {
    @Environment(\.dismiss) private var dismiss
    @Binding var path: [String]
    let name: String
    var body: some View {
        ZStack {
            Rectangle()
                .fill(.black)
                .frame(maxWidth: .infinity, maxHeight: .infinity)
            VStack {
                Text("pop")
                    .foregroundColor(.white)
                    .onTapGesture {
                        dismiss()
                    }
                    .padding(.bottom, 50)
                Text(name)
                    .foregroundColor(.white)
                    .onTapGesture {
                        withAnimation {
                            path.append("Detail \(path.count)")
                        }
                    }
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)

        }
        .background(.white)// TODO: mac back 是透明的，不是白色的
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .navigationBarBackButtonHidden() // TODO: 此处会隐藏navigtiaon bar 和back button，
    }
}


```

> 多层 `NavigationStack` 的实现 
> `NavigationStack`--->`NavigationSplitView`----> (detail: `NavigationStack`)
```swift 
struct NavigationDemo: View {
    @State private var path:[String] = []
    @State private var detailPath:[String] = []
    var body: some View {
        HStack {
            Rectangle()
                .fill(.red)
                .frame(width: 200)
                .frame(maxHeight: .infinity)
            NavigationStack(path: $path) {
                NavigationSplitView {
                    ZStack {
                        Rectangle()
                            .fill(.yellow)
                            .frame(maxWidth: .infinity, maxHeight:.infinity)
                        Text("Main")
                            .onTapGesture {
                                withAnimation {
                                    path.append("1")
                                }
                            }
                    }
                    .ignoresSafeArea()
                } detail: {
                    NavigationStack(path: $detailPath) {
                        ZStack {
                            Rectangle()
                                .fill(.blue)
                                .frame(maxWidth: .infinity, maxHeight:.infinity)
                            Text("Detail")
                                .onTapGesture {
                                    withAnimation {
                                        detailPath.append("1")
                                    }
                                }
                        }
                        .navigationDestination(for: String.self) { path in
                            DetailView(path: $detailPath, name: " Detail \(path)")
                        }
                    }
                    .ignoresSafeArea()
                }
                .ignoresSafeArea()
                .navigationDestination(for: String.self) { path in
                    DetailView(path: $path, name: "Main Detail \(path)")
                }
            }
            .navigationBarBackButtonHidden()
            .toolbar(.hidden, for: .windowToolbar)
            .ignoresSafeArea()
        }
    }
}

```


## 总结 
1. push 的方式 
    * NavigationLink
    * append
        如果没有正确使用navigation，还是能push，只是push 的是一个不知道哪里来的东西
    ```swift 
    path.append("1")

    ```

2. navigationDestination 的位置 
    必须是在`NavigationStack` root view 上 
3. pop 
    * dismiss 
    ```swift 
    @Environment(\.dismiss) private var dismiss
    dismiss()
    ```
    * path remove 
    ```swift 
    @Binding var path: [String]
    path.removeLast()
    ```

4. ~~目前不能在 `NavigationStack` 中包含 `NavigationStack`~~ (TODO: 此种形式已经通过上边的 示例代码实现了)
    ```swift
        NavigationStack(path: $path) {
            HStack {
                NavigationStack(path: $detailPath) {
                    VStack {
                        Rectangle()
                            .fill(.blue)
                            .frame(maxWidth: .infinity, maxHeight:.infinity)
                    }
                }
            }
            .navigationDestination(for: String.self) { path in
                DetailView(path: $path, name: "Detail \(path)")
            }

        }
    ```