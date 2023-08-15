# 参考 
https://www.macdev.app/ebook/event.html




# 鼠标相关 


## 相关函数 
* `hoverEffect` 悬停效果 
    __Mac不支持__ 
* `onHover` 是否悬停在视图上 
    `func onHover(perform action: @escaping (Bool) -> Void) -> some View` 返回的是`bool`, 智能检测是否悬停在 view 之上 

* `onContinuousHover`
    > 添加指针进入视图边界、在视图边界内移动和退出视图边界时要执行的操作。

```swift  
// 最好在外部包一层 GeometryReader ，否则，可能视图显示没问题，但是鼠标定位有问题
GeometryReader.init { proxy in
    Color.clear.onContinuousHover { hoverPhase in
        switch hoverPhase {
        case .active(let value):
            let origin = geometry[proxy.plotAreaFrame].origin
            let location = CGPoint(x: value.x - origin.x,y: value.y - origin.y)
            let originValue = proxy.value(at: location, as: (String, Int).self)
            debugPrint("====\(String(describing: originValue))")
            self.currentPoint = originValue
        case .ended:
            self.currentPoint = nil
            break
        }
    }
}
```

## 鼠标事件双击
> 此处包含了鼠标事件的双击、单击、同时处理单击和双击
```swift 
struct MyView: View {
    var body: some View {
        Rectangle()
            .fill(Color.blue)
            .frame(width: 200, height: 200)
            .gesture(TapGesture(count: 2).onEnded {
                print("double clicked")
            })
            .simultaneousGesture(TapGesture().onEnded {
                print("single clicked")
            })
    }
}
```


> 右键单击 
`contextMenu` 是目前右键事件监测的唯一方式




===============
## 键盘事件处理
* `addLocalMonitorForEvents`
    只能监听应用内事件
* `addGlobalMonitorForEvents`
    全局事件监听

> 移除监听`[NSEvent removeMonitor:eventMonitor];` 

* 更底层的监听方式
```swift 
 let event = CGEvent.tapCreate(tap: .cghidEventTap, place: .headInsertEventTap, options: .defaultTap, eventsOfInterest: CGEventMask(eventMask), callback: eventCallback, userInfo: nil)
 let source = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event, 0)
CFRunLoopAddSource(CFRunLoopGetCurrent(), source, .commonModes)
CGEvent.tapEnable(tap: event!, enable: true)

// 移除监听
if let source = source, CFRunLoopContainsSource(CFRunLoopGetCurrent(), source, .commonModes) {
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), source, .commonModes)
}
if let event = event, CGEvent.tapIsEnabled(tap: event) {
    CGEvent.tapEnable(tap: event, enable: false)
}

// 处理回调
func eventCallback(proxy: CGEventTapProxy, type: CGEventType, event: CGEvent, refcon: UnsafeMutableRawPointer?) -> Unmanaged<CGEvent>? { 
    // TODO: 这里会掉用两次 因为 keyUp/keyDown的存在
    guard let event = NSEvent.init(cgEvent: event)else {
        return nil
    }
    if event.modifierFlags.contains(.control) {
        
    }
    if event.charactersIgnoringModifiers == "A" {
    
    }    

    return nil // return nil 事件将不会再传播下去
}

// 直接处理 CGEvent, 不推荐这种方式
if [.keyUp].contains(type) {
    let flags = event.flags
    let keyCode = event.getIntegerValueField(.keyboardEventKeycode)
    if keyCode == 12 && ((flags.rawValue & CGEventFlags.maskCommand.rawValue) == CGEventFlags.maskCommand.rawValue) { 
        exit(0)
    }
}


```

#### // 此处通过event 获取更多信息 
```swift
CGPoint location = CGEventGetLocation(event);
let windowNumber = [NSWindow windowNumberAtPoint:location belowWindowWithWindowNumber:0];
CGWidnowID windowID = (CGWindowID)windowNumber; 

CFArrayRef array = CFArrayCreate(NULL, (const void **)&windowID, 1, NULL);
NSArray *windowInfos = (NSArray *)CGWindowListCrateDescriptionFormArray(array);

if (windowInfos.count> 0) {
    NSDictionary *info = [windowInfos objectAtIndex: 0];
}

```

#### 常见的一些操作 
1. 将左右键进行对掉
```swift 

if (type == .rightMouseDown) {
    CGEventSetType(event, .leftMouseDown)
}
return event
```

2. 更改鼠标事件为键盘return事件 
```swift 
CGEventRef newEvent = CGEventCreateKeyboardEvent(NULL, kVK_Return, type == .rightMouseDown);
return newEvent;

```

3. 重复发送事件
```swift 
// 两种方式 
CGEventPost(kCGSessionEventTap, newEvent); // 最好使用此方式， 避免再次被 kCGHIDEventTap 铺获到
CGEventTapPostEvent(proxy, newEvent);


```
