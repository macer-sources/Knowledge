
1. Menu  
```swift



```
2. Picker 
```swift 
    Picker("", selection: $day) {
      ForEach(1 ... maxDays, id: \.self) {
        Text("\($0)")
      }
    }
    .pickerStyle(.menu)
```

3. contentMenu 
```swift 
            Image(systemName: "ellipsis")
                .resizable()
                .aspectRatio( contentMode: .fit)
                .rotationEffect(.init(degrees: 90))
                .frame(width: 20)
                .frame(height: 10)
                .contentShape(Rectangle())
                .contextMenu {
                    Text("测试1")
                    Text("测试2")
                    Text("测试3")
            }
            // 鼠标右键才能显示
```


4. CommandMenu 
   // Mac 下，顶部菜单栏menu 
   
