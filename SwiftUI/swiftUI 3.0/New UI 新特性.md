
1. refreshable 下拉刷新功能 
```swift 
        List {

        }.refreshable {
                  // 可以写异步任务，等任务结束，就会隐藏
        }
      
```

2. search Bar(仅适用于 navigation bar)
```swift 
.searchable()


```

3. AsyncImage (异步加载图片)
4. 蒙板(.ultraThinMaterial) 
```swift 
Image().overlay(.ultraThinMaterial)

```
5. badge 
```swift 
TabView {
  Color.red.tabItem {
    Image("")
  }.badge(10)
}


```

6. onSubmit 
   用于监听TextField / searchable 输入完成
  ```swift 
        VStack {
            TextField("Test", text: $test)
        }.onSubmit(of: [.text]) {
            
        }

  ```
  
7. safeAreaInset 
```swift 
ScrollView {

}
.safeAreaInset(edge: .top) {
            Capsule()
                .fill(.red)
                .frame(height: 55)
                .padding(.horizontal)
        }

```
 

8. sheet 
```swift 
List {
   ForEach(0...20, id:\.self) { index in
       Text("User Task \(index)")
           .swipeActions(edge: .trailing, allowsFullSwipe: false) {
               Button {} label: {
                   Image(systemName: "house.fill")
               }
           }
   }
}
```

9. tint 可以更改背景色
```swift 
Button {} label: {
    Image(systemName: "house.fill")
}.tint(.blue)


```

10. AppStorage 


11. Keyboard Done Button 
```swift 
TextField("Test", text: $test)
   .textFieldStyle(.roundedBorder)
   .submitLabel(.join) // 此处设置

``` 


12. Convas 
通过 core graphics 进行绘制 

13. keyboard Accessory View 
```swift 
TextField("Test", text: $test)
   .textFieldStyle(.roundedBorder)
   .submitLabel(.join)
   .toolbar {
       // TODO add Toolbar Item to keyboard accessory view
   }

```

14. focus 
https://www.kodeco.com/31569019-focus-management-in-swiftui-getting-started

15. collection 中快速访问 Binding 
```swift 
Foreach($datasources) {$item in 
  
}
```


15. 增加了 foregroundStyle 
```swift 
  VStack {
      Text("Justine")
          .foregroundStyle(.primary)
      Text("Justine")
          .foregroundStyle(.secondary)
      
      Text("Justine")
          .foregroundStyle(.tertiary)
      
      Text("Justine")
          .foregroundStyle(.quaternary)
      
  }.foregroundStyle(.red)
```

16. 多彩 SF 
```swift 
Image(systemName: "brain.head.profile")
   .font(.largeTitle)
   .symbolRenderingMode(.palette)
   .foregroundStyle(.orange, .purple, .red)
// .symbolVariant(.circle) 这个属性也是与SF 相关的

```


17. Timeline View 
```swift 
// 利用 timeline view 实现随机数
TimelineView(.animation) { timeLine in
   Text("\(CGFloat.random(in: 0...1000))")
}
``

18. Button 的新修饰器
```swift 
Button {} label:{} 
.buttonStyle(.bordered) 
.controlSize(.large)
.controlProminence(.increased)
.tint(.red)
```

19. Toggle 

20. 支持自定义 key  获取scroll 偏移量
```swift 
.coordinateSpace(name:"testt")

GeometryReader {proxy in 
  let minY = proxy.frame(in: .named("testt")).minY
}
```

21. ScrollView Offset Modifiery
```swift 

struct OffsetModifier: ViewModifier {
    func body(content: Content) -> some View {
        content.overlay {
            GeometryReader { proxy -> Color in
                let minY = proxy.frame(in: .named("Offset")).minY
                
                return .clear
            }
        }
    }
}

```
> 使用： ScrollView{}.coordinateSpace(name:"Offset")
