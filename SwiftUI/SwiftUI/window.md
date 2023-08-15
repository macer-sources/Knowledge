
## 参考 
https://github.com/lukakerr/NSWindowStyles 
https://cindori.com/developer/floating-panel
https://github.com/onmyway133/blog/issues/789


1. 多窗口支持 
> 使用supportsMultipleWindows环境值检查您运行的平台何时支持多窗口环境。
```swift 

struct ContentView: View {
    @Environment(\.supportsMultipleWindows)
    private var supportsMultipleWindows
    
    var body: some View {
        if supportsMultipleWindows {
            Text("Supports multiple windows")
        } else {
            Text("Doesn't support multiple windows")
        }
    }
}

```

2. 单一窗口
```swift 
@main struct MyApp: App {
    var body: some Scene {
        #if os(macOS)
        Window("Statistics", id: "stats") {
            StatisticsView()
        }
        #endif
          // TODO: 此处不会被创建出来，只会创建上层的window
        WindowGroup {
            ContentView()
        }
    }
}

```

3. 打开window 
```swift 
@Environment(\.openWindow) private var openWindow
Button("Open statistics") {
            openWindow(id: "stats")
        }


```

4. 从view 获取window 
```swift 
@available(OSX 11.0, *)
public extension View {
    func wndAccessor(_ act: @escaping (NSWindow?) -> () ) -> some View {
        self.modifier(WndTitleConfigurer(act: act))
    }
}

@available(OSX 11.0, *)
struct WndTitleConfigurer: ViewModifier {
    let act: (NSWindow?) -> ()
    
    @State var window: NSWindow? = nil
    
    func body(content: Content) -> some View {
        content
            .getWindow($window)
            .onChange(of: window, perform: act )
    }
}

//////////////////////////////
///HELPERS
/////////////////////////////

// Don't use this:
// Usage:
//.getWindow($window)
//.onChange(of: window) { _ in
//    if let wnd = window {
//        wnd.level = .floating
//    }
//}

@available(OSX 11.0, *)
private extension View {
    func getWindow(_ wnd: Binding<NSWindow?>) -> some View {
        self.background(WindowAccessor(window: wnd))
    }
}

@available(OSX 11.0, *)
private struct WindowAccessor: NSViewRepresentable {
    @Binding var window: NSWindow?
    
    public func makeNSView(context: Context) -> NSView {
        let view = NSView()
        DispatchQueue.main.async {
            self.window = view.window
        }
        return view
    }
    
    public func updateNSView(_ nsView: NSView, context: Context) {}
}
```


4. 关闭window， swift UI与原始window结合使用时 
```swift 
typealias SceneID = String
extension NSApplication {

    private func openWindowPerform() {
        let item = NSApp.windowsMenu?.items.first { $0.title == "OpenWindowAction" }
        if let item, let action = item.action {
            NSApp.sendAction(action, to: item.representedObject, from: nil)
        }
    }

    func closeWindow(_ id: SceneID) {
        windows.first { $0.identifier?.rawValue == id }?.close()
    }

    func closeWindow(_ ids: SceneID...) {
        ids.forEach { id in
            windows.first { $0.identifier?.rawValue == id }?.close()
        }
    }

    func findWindow(_ id: SceneID) -> NSWindow? {
        windows.first { $0.identifier?.rawValue == id }
    }

    var openSwiftUIWindows: Int {
        NSApp
            .windows
            .compactMap(\.identifier?.rawValue)
            .compactMap { $0 }
            .count
    }
}



// 创建window 
  Window("", id: "about") {
            Text("test")
  }

// 关闭window 
NSApp.closeWindow("about")

```



5. window 的属性设置 
```swift 
        WindowGroup {
            Rectangle().fill(.yellow)
//                .frame(width: 90, height: 90)// 只在这里设置frame，而不设置window的frame，整个window显示出来的会大于设置的farmer大小，带有个title bar 的高度28， 并且view 显示可能会有问题
                .frame(maxWidth: .infinity, maxHeight: .infinity)
                .ignoresSafeArea()
        }
        .defaultSize(CGSize.init(width: 90, height: 90)) // 直接在这里约定死window的大小，才是实际想要的大小，比如这里的90 ，绘制出来整个window的大小就是90
        .windowResizability(.contentSize)
        .windowStyle(.hiddenTitleBar)
```

6. window remove close/minimize button 
```swift 
        .onReceive(NotificationCenter.default.publisher(for:
                        NSWindow.didBecomeKeyNotification)) { notification in
                        if let window = notification.object as? NSWindow {
                            window.standardWindowButton(.closeButton)?.isHidden = true
                            window.standardWindowButton(.miniaturizeButton)?.isHidden = true
                            window.standardWindowButton(.zoomButton)?.isHidden = true
                        }
                    }
```
> 实例 
```swift 
struct CleanKeyboardFolatingWindow: Scene {
    var body: some Scene {
        Window("", id: "floating") {
            Rectangle().fill(.red)
                .frame(maxWidth: .infinity, maxHeight: .infinity)
                .labelsHidden()
                .ignoresSafeArea()
                .onReceive(NotificationCenter.default.publisher(for: NSWindow.didBecomeKeyNotification)) { notification in
                    if let window = notification.object as? NSWindow, window.identifier?.rawValue == "floating" {
                        window.standardWindowButton(.closeButton)?.isHidden = true
                        window.standardWindowButton(.miniaturizeButton)?.isHidden = true
                        window.standardWindowButton(.zoomButton)?.isHidden = true
                    }
                }
        }
        .defaultSize(CGSize.init(width: 90, height: 90))
        .windowResizability(.contentSize)
        .windowStyle(.hiddenTitleBar)
    }
}
```




7. windowGroup / Window 区别 
   * id 
    如果设置window ID 那么，将等于NSWindow.identifier.rawValue, 如果设置的 windowGroup id ， 那么实际 NSWindow 的 idenfitifer 是 `floating-AppWindow-1` 这种形式的



8. window 加载顺序 
```swift 
    var body: some Scene {
        Window("", id: "test") {
            ContentView()
        }
        CleanKeyboardFolatingWindow()
    }

// 首次进入只会显示第一个window， 可以通过open window 打开其他的window 
```


9. open window的几种方式
* `openWindow` 
```swift
WindowGroup("Plan Chooser", id: Window.paywall.id) { ... }
 self.openWindow(id: Window.paywall.id)
```
* `openURL` 通过深度链接进行打开,

