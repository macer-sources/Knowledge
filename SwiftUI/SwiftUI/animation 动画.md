
## Animation  
* `animation()` 方法 
* `withAnimation()` 方法
* `matchedGeometryEffect` 在 view 之间专场，会讲动画状态进行传递，公用一个状态， 还是需要结合 `animation()`执行动画



1. `animation 修饰符`  
这会导致值在其当前值和新值之间进行动画处理
```swift 
.animation(.easeOut, value: animationAmount)

struct ContentView: View {
    @State private var animationAmount = 1.0

    var body: some View {
        Button("Tap Me") {
            animationAmount += 1
        }
        .padding(50)
        .background(.red)
        .foregroundColor(.white)
        .clipShape(Circle())
        .scaleEffect(animationAmount)
        .animation(.easeInOut(duration: 2), value: animationAmount) // 此方法之前的操作， 能动画的部分都会进行动画执行
    }
}
```

```swift 
// 针对部分值的变化， 比如： true/false， 是瞬间变化的，无法直接绑定动画，所以此时，可以直接在值上榜绑定动画
@State private var animationAmount = 1.0
Stepper("Scale amount", value: $animationAmount.animation(
    .easeInOut(duration: 1)
        .repeatCount(3, autoreverses: true)
), in: 1...10)

```


2. 显示动画(`withAnimation`) 
```swift 
withAnimation(.interpolatingSpring(stiffness: 5, damping: 1)) {
    animationAmount += 360
}
```

> 有时候，动画无法执行， 其一定和创建时机和动画值更改时机有关系，需要灵活调整动画 

3. `transition`  可以执行部分类似动画操作
```swift 
Rectangle()
    .fill(.red)
    .frame(width: 200, height: 200)
    .transition(.scale)

.transition(.asymmetric(insertion: .scale, removal: .opacity))
```



## Lottie 动画
需要通过 `UIViewRepresentable`进行封装 
```swift 

struct AnimationView: UIViewRepresentable {

    func makeUIView(context: UIViewRepresentableContext<AnimationView>) -> AnimationView {
        let view = AnimationView()
        let animation = Animation.named("", subdirectory: "") 
        view.animation = animation 
        view.loopMode = .repeat(10.0) 
        view.play() 
        return view
    }

}
```



