1. Chart 图表 
```swift 
// 更改颜色 
.foregroundStyle(by: . value("Key", item.day))

// 颜色区间对应 
.chartForegroundStyleScale(
  domain: datas.compactMap({
        key -> String? in 
         
  }, range: barColors)
)


```

2. Custom Presented
```swift 
Button("Show Sheet") {
  showView.toggle()
}.sheet(isPresented: $showView) {
  Text("Hello From Sheet") 
  .presentationDetents([.small, .medium, .large]) 
  .presentationDragIndicator(.hidden)
}

// 自定义 detents 
static let small = Self.height(100)

```

4. Share Link 
```swift
// 1. 分享基本的链接 
ShareLink(item: "xxxxxx")

// 分享图片 
struct ImageFile: Transferable {
  var image: Image 
  static var transferRepresentation: some TransferRepresentation {
     ProxyRepresentation(exporting: \.image)
  }
}


var imageFile = ImageFile(image: Image("xxxxx")) 
ShareLink(item: imageFile, preview: SharePreview("ssss", image: imageFile.image)) {
  Text("Share Image")
}

```

5. ImageRenderer 从 View 生成图片  
```swift 
let renderer = ImageRenderer(content: View()) 
if let image = renderer.uiImage {
  // 生成了 image
}
```

6. viewThatFits 
```swift 
ViewThatFits(in: .horizontal) {

}
```
7. Grid 
```swift 
Grid {
  GridRow {
      Color.red
      Color.red
      Color.red
  }
  GridRow {
    Color.red.gridCellColumns(1) // decalre custom Cell Size
  }
}


```

8. Custom Layout 
```swift



```
9. NavigationStack/ NavigationSplitView
```swift 


```

10. Table 
```swift
Table(dataSource) {
  TableColumn("Day", \.day)
  TableColumn("Downloads", \.downloads)
}
```

11. Gauge 
```swift 


```
12. tap gesture location. 
```swift 
Rectangle() 
.onTapGesture {location in 
// location 点击的位置
}
// 通过设置 coordinateSpace 确定点击点针对全局还是当前位置以及其他
```

13. scrollview Updates
```swift 
// 是否可以滚动 
.disabled(true)
// 滚动以隐藏键盘
.scrollDismissesKeyboard(.immediately)

```

14. MultiDatePicker 
```swift 
MultiDatePicker("", selection: $selectedDates)
   .frame(height: 400)
   .padding(20)
   .background {
       RoundedRectangle(cornerRadius: 10)
           .fill(.white.shadow(.drop(radius: 4)))
   }
   .padding()
```

15. TextField 支持多行 





16. `refreshing` 
  下拉刷新  
17. animation transiton push 
```swift 
VSTask {
  if show {
    Image("")
    ... 
    .transition(.push(from: .top))
  }else {

  }

}

```

18. dynamicLayout
```swift 
var body: some View {
  let dynamicLayout = show? AnyLayout(VStackLayout()) : AnyLayout(HStackLayout()) 

  dynamicLayout {
    sampleView

  }
  .onTapGesture {
    withAnimation {
      show.toggle()
    }

  }

var sampleView: some View {
  Group {
    Image("image1")
    Image("image2")
  }
}


}


```


19 __contentTransition__ 
```swift 
var body: some View {
  Text("Hello World")
  .font(.largeTitle)
  .fontWeight(show? .black: .thin)
  .foregroundColor(show? .yellow : .red) 
  .contentTransition(.interpolate)
  .onTapGesture {
    withAnimation {
      show.toggle()
    }

  }

}
```


20. navigation background color & hidden  
```swift 
NavigationStack {
  Text("Hello")
  .navigationTitle("Title")
  .navigationBackground(.yellow, for: .navigationBar)
  .navigationBackground(.visiable, for: .navigationBar)

  .toolbar(.hidden , for:.navigationBar)// 隐藏 navigation bar

}



```



21. ViewThatFits 
> 通过考虑所用空间，bing获取完全适合可用空间的第一个视图 

#### 原理： 
1. ViewThatFits测量特定轴或两者的可用空间。 
2. ViewThatFits测量特定轴或两者的可用空间。 
3. 如果第一个视图不适合可用空间，它会测量第二个视图的大小，如果适合则放置第二个视图。以此类推。
4. 如果没有视图适合可用空间，它将把最后一个视图放置在ViewBuilder闭包中。
```swift 





```
