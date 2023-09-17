

# HostingView 获取内在 SwiftUI 的实际size 的方式 

## 定义 swiftUI  
```swift 


struct TextView: View {
    var text: String
    var body: some View {
        Text(text)
    }
}



```


## 通过 swiftUI 创建 hosting view 
```swift 

    override func viewDidLoad() {
        super.viewDidLoad()
        let textViewUI = TextView.init(text: text).frame(maxWidth: 400).fixedSize(horizontal: false, vertical: true)
        let textView = NSHostingView(rootView: textViewUI)
        textView.translatesAutoresizingMaskIntoConstraints = false
        textView.layoutSubtreeIfNeeded()

        // 通过以上方式，可以获取  fitting size  就是swiftUI 具体的大小，
        // 计算方式： 依据设置的最大宽度来计算 高度的 这里设置的是  frame(maxWidth: 400) 最大宽度为 400， 通过 fixedSize(horizontal: false, vertical: true) 来计算，宽度部分不考虑变更。只变更高度部分 
        debugPrint("textView:\(textView.frame)======\(textView.fittingSize)") 
        // 这个时候frame 中size 和origin 都是 0， 但是  fittingSize 部分是有值的
        // 注意： intrinsicContentSize 这个值是不对的 
    }

```
