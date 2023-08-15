1. menu 
```swift 
 Menu {
     Button("New Window") {
         
     }
     Button("Help") {
         
     }
     Button("Close Window") {
         
     }
 }label: {
     Image(systemName: "ellipsis")
         .font(.title3)
         .fontWeight(.semibold)
         .foregroundColor(.white)
         .rotationEffect(.degrees(90))
         .frame(width: 20, height: 20)
         .contentShape(Rectangle())
 }
 .buttonStyle(.plain)
```

2. 多window 的实现方式 
```swift 
struct _4__multi_windowApp: App {
    var body: some Scene { 
      // TODO: 是主window
        WindowGroup {
            ContentView()
        }
        .windowResizability(.contentSize)
        .windowStyle(.hiddenTitleBar)
        // MARK: - multi-Tab Window Group 其它window 通过  @Environment(\.openWindow) var openWindow 进行打开
        // let newTab = Tab()
        // openWindow(value: newTab)
        WindowGroup(id: "New Tab", for: Tab.self) { $tab in
            NewTabView(tab:tab, isRootView: false)
        }
        .windowResizability(.contentSize)
        .windowStyle(.hiddenTitleBar)
    }
}
```

3. 关闭 window 
```swift 
if isRootView {
    NSApplication.shared.terminate(nil)
}else {
    NSApplication.shared.mainWindow?.close()
}


```

4. 实现一个 Menu Bar App  
__如果不使用swiftUI方式，需要在 appdelegate 中初始化 MenuBarItem 对象
```swift 
struct _3_MenuBarAppApp: App {
    var body: some Scene {
        MenuBarExtra {
            Image(systemName: "switch.2")
        } label: {
            Image(systemName: "switch.2")
        }
        // Menu bar style
        .menuBarExtraStyle(.window)
        // .menu List Type
        // .window View type
    }
}

```
5. 原始方式实现 menu bar app 
```swift 

class AppDelegate: NSObject, NSApplicationDelegate, ObservableObject {
    
    @Published var statusItem: NSStatusItem?
    @Published var popover = NSPopover()
    
    func applicationDidFinishLaunching(_ notification: Notification) {
        setupBarMenu()
    }
    
    private func setupBarMenu() {
        
        popover.animates = true
        popover.behavior = .transient
        
        popover.contentViewController = NSHostingController(rootView: ContentView())
        
        // mark
        popover.contentViewController?.view.window?.makeKey()
        
        
        statusItem = NSStatusBar.system.statusItem(withLength: NSStatusItem.variableLength)
        if let statusButton = statusItem?.button {
            statusButton.image = .init(systemSymbolName: "dollarsign.circle.fill", accessibilityDescription: nil)
            statusButton.action = #selector(statusAction(sender:))
        }
    }
}

extension AppDelegate {
    @objc private func statusAction(sender: AnyObject) {
        if popover.isShown {
            popover.performClose(sender)
        }else {
            if let menuButton = statusItem?.button {
                popover.show(relativeTo: menuButton.bounds, of: menuButton, preferredEdge: .minY)
            }
        }
    }
}

```
6. 目前，无法监听`menu`的展开和关闭 
7.  onTapGesture 事件无响应问题 
```swift 
//针对内容执行 
.contentShape(RoundedRectangle(cornerRadius: 10)) 
// 然后再添加  onTapGesture 事件
```
