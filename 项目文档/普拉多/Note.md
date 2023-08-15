1. 使用 swift package 管理的资源中加载storyboard
```
    public static let ModuleName = "PLMainModule"
    public static let WindowControllerName = "MainWindowController"
    public static let StoryboardName = "Main"
    
    let wc = NSStoryboard.cocoa.windowController(name: MainRouter.WindowControllerName, storyboard: NSStoryboard.Name.init(MainRouter.StoryboardName), bundle: Bundle.module)
    
    注意: 要关闭资源的 inherit Module for Target 但是，module name 要存在

```

2. 拖动滚动view 
```swift
class DraggableClipView: NSClipView {

    private var clickPoint: NSPoint!
    private var originalOrigin: NSPoint!

    override func mouseDown(with event: NSEvent) {
        clickPoint = event.locationInWindow
        originalOrigin = bounds.origin
    }

    override func mouseDragged(with event: NSEvent) {

        let scale = (superview as? NSScrollView)?.magnification ?? 1.0

        let newPoint = event.locationInWindow

        let newOrigin = NSPoint(x: originalOrigin.x + (clickPoint.x - newPoint.x) / scale,
                                y: originalOrigin.y - (clickPoint.y - newPoint.y) / scale)

        let constrainedRect = constrainBoundsRect(NSRect(origin: newOrigin, size: bounds.size))

        scroll(to: constrainedRect.origin)

        superview?.reflectScrolledClipView(self)
    }

    override func mouseUp(with _: NSEvent) {
        clickPoint = nil
    }
}

```