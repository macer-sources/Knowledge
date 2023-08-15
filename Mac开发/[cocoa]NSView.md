
# NSView 相关



## 为 NSView 添加手势返回  

### 基础手势 
* `NSPanGestureRecognizer`   
```swift 
var startPoint: NSPoint? = nil
var endPoint: NSPoint? = nil

var delta: CGPoint {
    guard let start = startPoint, let end = endPoint else { return CGPointZero }
    return CGPointMake(end.x - start.x, end.y - start.y)
}

let panGestureRecognizer = NSPanGestureRecognizer.init(target: self, action: #selector(panGestureRecognized(_:)))
panGestureRecognizer.delegate = self
addGestureRecognizer(panGestureRecognizer)

func panGestureRecognized(sender: NSPanGestureRecognizer) {
    if sender.state == .Began {
        startPoint = sender.translationInView(self)
    }
    if sender.state == .Recognized {
        endPoint = sender.translationInView(self)
        delegate?.handlePanGestureRecognized(delta)
    }
}
```

* SwipeTracking ScrollView 
```swift 
override func wantsScrollEventsForSwipeTrackingOnAxis(axis: NSEventGestureAxis) -> Bool {
    return axis == .Horizontal
}

override func scrollWheel(theEvent: NSEvent) {

    // Not a gesture scroll event.
    if theEvent.phase == .None { return }
    // Not horizontal
    if abs(theEvent.scrollingDeltaX) <= abs(theEvent.scrollingDeltaY) { return }

    var animationCancelled = false

    theEvent.trackSwipeEventWithOptions(
        .LockDirection,
        dampenAmountThresholdMin: 1,
        max: 1) { (gestureAmount, phase, complete, stop) in

        if animationCancelled {
            stop.initialize(true)
        }

        if (phase == .Began) { }
        if (phase == .Ended || phase == .Cancelled) {
            self.delegate?.hanldeSwipeTrackingFromScrollEvent(CGPointMake(theEvent.scrollingDeltaX, theEvent.scrollingDeltaY))
            animationCancelled = true
        }
    }
}

```

* ThreeFingersSwipeGesture 
```swift 

// http://stackoverflow.com/questions/7433120/nsresponder-swipewithevent-not-called
//
// To receive swipeWithEvent: messages, you have to ensure that the 3 finger swipe gesture is not mapped to anything that might cause a conflict. Go to System preferences -> Trackpad -> More Gestures, and set these preferences to one of the following:
//
// Swipe between pages:
// - Swipe with two or three fingers, or
// - Swipe with three fingers
// Swipe between full-screen apps:
// - Swipe left or right with four fingers

override func swipeWithEvent(event: NSEvent) {
    let x = event.deltaX
    let y = event.deltaY

    if abs(x) > 0 || abs(y) > 0 {
        delegate?.hanldeSwipeWithThreeFingers(CGPointMake(x, y))
    }
}

```

* TwoFingersSwipeGesture 
```swift 
protocol TwoFingersSwipeGestureRecognizeViewDelegate {
    func hanldeSwipeWithTwoFingers(delta: CGPoint)
}

class TwoFingersSwipeGestureRecognizeView: CustomView {

    var delegate: TwoFingersSwipeGestureRecognizeViewDelegate? = nil

    // Basically it's from
    // https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/EventOverview/HandlingTouchEvents/HandlingTouchEvents.html#//apple_ref/doc/uid/10000060i-CH13-SW21

    var initialTouches = Array<NSTouch?>(count: 2, repeatedValue: nil)
    var currentTouches = Array<NSTouch?>(count: 2, repeatedValue: nil)

    var delta: CGPoint {
        guard let fingerAInitial = initialTouches[0],
            let fingerBInitial = initialTouches[1],
            let fingerACurrent = currentTouches[0],
            let fingerBCurrent = currentTouches[1] else {
                return CGPointZero
        }

        let fingerADeltaX = fingerACurrent.normalizedPosition.x - fingerAInitial.normalizedPosition.x
        let fingerADeltaY = fingerACurrent.normalizedPosition.y - fingerAInitial.normalizedPosition.y
        let fingerBDeltaX = fingerBCurrent.normalizedPosition.x - fingerBInitial.normalizedPosition.x
        let fingerBDeltaY = fingerBCurrent.normalizedPosition.y - fingerBInitial.normalizedPosition.y

        // We'll use sameSign(:,y:) to make sure we are dealing with two finger scroll
        // rather than zoom-in or zoom-out.
        func sameSign(x: CGFloat, y: CGFloat) -> Bool { return (((x >= 0) ? 1 : 0) ^ ((y < 0) ? 1 : 0)) != 0 }

        let deltaX = sameSign(fingerADeltaX, y: fingerBDeltaX) ? (abs(fingerADeltaX) > abs(fingerBDeltaX) ? fingerADeltaX : fingerBDeltaX) : 0
        let deltaY = sameSign(fingerADeltaY, y: fingerBDeltaY) ? (abs(fingerADeltaY) > abs(fingerBDeltaY) ? fingerADeltaY : fingerBDeltaY) : 0

        // In NSEvent, a non-0 deltaX will represent a horizontal swipe, 
        // -1 for swipe right and 1 for swipe left.
        // So we just using the negtive value of deltaX anyway.
        return CGPointMake(deltaX, deltaY)
    }

    let threshold: CGFloat = 0
    var isTracking = false

    override func awakeFromNib() {
        super.awakeFromNib()
        acceptsTouchEvents = true
    }

    override func touchesBeganWithEvent(event: NSEvent) {
        // Should use '.Began' or '.Touching' rather than '.Any',
        // but sometimes the phase is '.Unknown', so.. I just use '.Any' here anyway.
        let touches = event.touchesMatchingPhase(.Any, inView: self)
        if touches.count == 2 {
            let array = Array(touches)
            initialTouches[0] = array[0]
            initialTouches[1] = array[1]
            currentTouches[0] = initialTouches[0]
            currentTouches[1] = initialTouches[1]
        } else if touches.count == 2 {
            // More than 2 touches. Only track 2.
            if isTracking {
                cancelTracking()
            }
        }
    }

    override func touchesMovedWithEvent(event: NSEvent) {
        let touches = event.touchesMatchingPhase(.Touching, inView: self)
        if let fingerAInitial = initialTouches[0],
            let fingerBInitial = initialTouches[1]
            where touches.count == 2 {

            touches.forEach { touch in
                if touch.identity.isEqual(fingerAInitial.identity) {
                    currentTouches[0] = touch
                } else if touch.identity.isEqual(fingerBInitial.identity) {
                    currentTouches[1] = touch
                }
            }

            if !isTracking {
                isTracking = true
            }
        }
    }

    override func touchesEndedWithEvent(event: NSEvent) {
        if isTracking {
            if (abs(delta.x) > threshold || abs(delta.y) > threshold) {
                delegate?.hanldeSwipeWithTwoFingers(delta)
            }
            cancelTracking()
        }
    }

    override func touchesCancelledWithEvent(event: NSEvent) {
        if isTracking {
            cancelTracking()
        }
    }

    func cancelTracking() {
        initialTouches = initialTouches.map { _ in nil as NSTouch? }
        currentTouches = currentTouches.map { _ in nil as NSTouch? }
        isTracking = false
    }
}

```






### Trackpad Events 

当用户手指在 Trackpad 上移动或点按时，系统会生成 `Multi-touch Events`，`Gesture Events` 或 `Mouse Events`。
Trackpad 内置支持将某些手势等价为鼠标操作（具体在 SystemPreference - Trackpad 中设置）；对于某些手势 Event， NSWindow 将直接调用 NSResponder 中相对应的方法：
* `Pinch`：两指捏近或松开，对应缩小或者放大，将调用 `magnifyWithEvent`: 
* `Rotate`: 两指沿着相对半圆移动，对应旋转，调用 `rotateWithEvent:`；
* `Swipe` : 三指沿着同一方向扫过（Brushing across the trackpad in a common direction），对应 Swipe，调用 `swipeWithEvent:` ；
* `Scroll`: 两指沿着同一水平或垂直方向移动，对应 Scroll，调用相应鼠标事件（例如： `scrollWheel:` ）  

当光标悬浮在某 View 上操控 Trackpad 时，View 将接收到 Event，View 将处理 Touch 事件或者沿着 Responder Chain 向上传递直到事件被处理或者 Discarded（更多有关 Responder Chain 可以参见这篇文章）。


### Swipe Gesture  
大多数情况下， Trackpad 的设置并不支持 `swipeWithEvent:` 事件。 为了使得系统支持，我们需要在 System Preferences - Trackpad - More Gestures 中进行如下设置：
* 在 Swipe between pages 中选择：Swipe with two or three fingers 或 Swipe with three fingers； 
* 在 Swipe between full-screen apps 中选择：Swipe left or right with four fingers。  

这样才能确保 NSWindow 接收 Three Finger Swipe 事件后向相应的 View（即 First Responder）发送 swipeWithEvent: 消息。  
在 swipeWithEvent: 中根据 NSEvent 的 deltaX 和 deltaY 属性即可获取水平、垂直方向的偏移。  

```swift 
override func swipeWithEvent(event: NSEvent) {      
    // Handler here.        
    let x = event.deltaX    
    let y = event.deltaY   
    // Do sth...
 }
```



### Scroll Gesture 






