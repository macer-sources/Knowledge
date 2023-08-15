## 原理 
使用 `XXX_Preview:PreviewProvider`预览，然后通过`UIViewControllerRepresentable / UIViewRepresentable`封装 UIkit

```swift 
import SwiftUI
struct ViewControllerPreview: UIViewControllerRepresentable {
    
    typealias UIViewControllerType = UIViewController
    
    let viewControllerBuilder: () -> UIViewControllerType
 
    init(_ viewControllerBuilder: @escaping () -> UIViewControllerType) {
        self.viewControllerBuilder = viewControllerBuilder
    }
    
    @available(iOS 13.0.0, *)
    func makeUIViewController(context: Context) -> UIViewController {
        viewControllerBuilder()
    }
 
    func updateUIViewController(_ uiViewController: UIViewController, context: Context) {
        // Do nothing
    }
}
 
struct ViewPreview: UIViewRepresentable {
    
    let viewBuilder: () -> UIViewType
 
    init(_ viewBuilder: @escaping () -> UIViewType) {
        self.viewBuilder = viewBuilder
    }
    
    func makeUIView(context: Context) -> UIView {
        viewBuilder()
    }
    
    func updateUIView(_ uiView: UIView, context: Context) {
        // Do nothing
    }
    
    typealias UIViewType = UIView
    
}
```

## 预览 
```swift 
#if canImport(SwiftUI) && DEBUG
import SwiftUI
 
@available(iOS 13.0, *)
struct Login_Preview: PreviewProvider {
    static var previews: some View {
        ViewControllerPreview {
            UINavigationController(rootViewController: LoginPage())
        }
    }
}
#endif
```


## 支持版本自增 
```swift 
if [ $ENABLE_PREVIEWS == "NO" ]
then
  # your code to execute here
  version=[crayon-645aad091b0b7752596513 inline="true" ]/usr/libexec/PlistBuddy -c "Print CFBundleVersion" $PRODUCT_SETTINGS_PATH
  version= expr $version + 1
/usr/libexec/PlistBuddy -c “Set :CFBundleVersion $version” $PRODUCT_SETTINGS_PATH
else
echo “Skipping the script because of preview mode”
fi
```

