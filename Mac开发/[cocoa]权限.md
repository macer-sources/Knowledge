# cocoa 隐私权限获取  


## 1. 文件鱼文件夹  





## 2. 完全磁盘访问权限 





## 3. 辅助功能 
```swift 
func requestAccessibilityAuthorization() -> Bool {
    let options = [kAXTrustedCheckOptionPrompt.takeUnretainedValue() as String: true]
    let accessibilityEnabled = AXIsProcessTrustedWithOptions(options as CFDictionary)
    
//        if !accessibilityEnabled {
//            let alert = NSAlert()
//            alert.messageText = "需要辅助功能权限"
//            alert.informativeText = "这个应用需要你授予辅助功能权限以进行键盘模拟。请在弹出的对话框中点击“打开系统偏好设置”。"
//            alert.addButton(withTitle: "确定")
//            alert.runModal()
//            print("未授权，去授权")
//        }
    
    return accessibilityEnabled
}
    
```




## 4. 输入监控 

