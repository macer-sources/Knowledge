1. 简单方式
```swift
Color.red.gradient
```

2. 线性渐变/径向渐变/角度渐变
```swift 
// 线性渐变
LinearGradient(gradient: Gradient(colors: [.blue, .white, .pink]), startPoint: .topLeading, endPoint: .bottomTrailing)

// 径向渐变
RadialGradient(gradient: Gradient(colors: [.blue, .black]), center: .center, startRadius: 2, endRadius: 650)


// 角度渐变
AngularGradient(gradient: Gradient(colors: [.green, .blue, .black, .green, .blue, .black, .green]), center: .center)
```

3. 应用 
```swift 
ZStack {
            RadialGradient(gradient: Gradient(colors: [.orange, .red]), center: .center, startRadius: 100, endRadius: 470)
            Text("SwiftUI").font(.system(size: 83)).fontWeight(.thin).foregroundColor(.white)
}.edgesIgnoringSafeArea(.all)

// 或者 
.background {
  RadialGradient(gradient: Gradient(colors: [.orange, .red]), center: .center, startRadius: 100, endRadius: 470)
}

```
