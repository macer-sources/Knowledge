# NSButton 相关




## 1. NSButton 设置高亮图片 
```swift 

let button = NSButton() 
button.image = NSImage.init(named: "") 

// 设置高亮 
(button.cell as? NSButtonCell).highlightBy: NSContentsCellMask
button.alternateImage = NSImage.init(named: "") 

// 设置图片铺满 button 
button.cell.setimageScaling: NSImageScaleAxesIndependently

```