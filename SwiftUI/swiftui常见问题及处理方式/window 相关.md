## window 相关的 


## 1. onOpenURL window 下无法使用问题， 只能在 windowGroup 下使用 
```swift 

 WindowGroup {
      ContentView()
        .onOpenURL { url in
          // handle the URL that must be opened
        }
    }

```

### 解决方案 
1. 使用 appdelegate 去进行open url 处理 
2. 主窗口用 `Window`,然后创建一个 空 `WindowGroup` 专门进行处理 `onOpenURL`  








## 2. 如何防止 `windowGroup`重复创建window
* 针对 `openWindow`的防重复
```swift 
extension WindowGroup {
   init<W: Identifiable, C: View>(_ titleKey: LocalizedStringKey, uniqueWindow: W, @ViewBuilder content: @escaping () -> C)
   where W.ID == String, Content == PresentedWindowContent<String, C> {
      self.init(titleKey, id: uniqueWindow.id, for: String.self) { _ in
         content()
      } defaultValue: {
         uniqueWindow.id
      }
   }
}


WindowGroup("Plan Chooser", uniqueWindow: Window.paywall) {
   // ...
}


extension OpenWindowAction {
   func callAsFunction<W: Identifiable>(_ window: W) where W.ID == String {
      self.callAsFunction(id: window.id, value: window.id)
   }
}

self.openWindow(Window.paywall)
```



* 针对`onOpenURL` 的防重复 
```swift 

    var body: some Scene {
     WindowGroup {
       ContentView()
         .handlesExternalEvents(preferring: ["myScene"], allowing: ["*"]) // activate existing window if exists 激活现有窗口(如果存在)
         .onOpenURL(perform: { url in
             print("\(url)") // This is just debug code
          })
       }.commands {
          CommandGroup(replacing: .newItem, addition: { })
       }
//       .handlesExternalEvents(matching: Set(arrayLiteral: "*")) // create new window if doesn't exist  // 如果不存在,创建新窗口,如果是这一个，将创建多个window

    }


```



## 3. 多 window 问题  
> 默认情况下， `Scene` 中创建多个 `window`时， 只会显示第一个 
```swift 

  var body: some Scene {
    WindowGroup {
      MyView()
        .frame(width: 200, height: 200)
        .background(Color.red)
    }
    WindowGroup {
      MyView()
        .frame(width: 200, height: 200)
        .background(Color.yellow)
    }
  }
// 默认只会显示红色的
```

#### ` handlesexternalevents(matching:)`: 指定一个修饰符以指示在为接收到的外部事件创建新场景时是否可以使用此场景
> 外部事件 
* `universal links`
* siriKit
* Spotlight
* Handoff
* Other


#### handlesexternalevents 
* `handlesExternalEvents(matching:)` 
    __仅支持在`WindowGroup`上使用__ 
    * `.handlesExternalEvents(matching: ["myScene"])`: 给定一个条件，如果该条件符合， 将调用适当的窗口  
    ```swift 
    @main
    struct testWindowGroupApp: App {
    
    @Environment(\.openURL) var openURL
    
    var body: some Scene {
        
        WindowGroup {
        ContentView()
            .frame(width: 200, height: 200)
        }
        .commands {
            CommandMenu("MyMenu") {
                Button("Show my Scene") {
                    openURL(URL(string: "myApp://myScene")!)
                }
            }
        }
        .handlesExternalEvents(matching: ["main"])
        
        MyScene()
        .handlesExternalEvents(matching: ["myScene"])
    }
    ```

* `handlesExternalEvents(preferring:, allowing:)`



============================== 
