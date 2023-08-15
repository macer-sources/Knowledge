### 启用/禁用滚动视图 
```
class MyScrollView: NSScrollView {
    /// スクロール有効/無効
    var isScrollEnabled = true

    /// scrollWheel
    /// - Parameter event:
    override func scrollWheel(with event: NSEvent) {
        if self.isScrollEnabled {
            super.scrollWheel(with: event)
        }
    }
}

```

### 使用带有快捷键的 ESC 时

```
@IBAction override func cancelOperation(_ sender: Any?) {
}

```

### presentAsSheet 调用的 View 的固定大小


```
class MyViewController: NSViewController {
    override func viewDidLoad() {
        super.viewDidLoad()
        // サイズを固定
        self.preferredContentSize = self.view.frame.size
    }
}


```


### NSWindow 全屏检测 

```
extension NSWindow {    
    // フルスクリーン判定
    var isFullScreen: Bool {
        return self.styleMask.contains(.fullScreen)
    }    
}

```


### 启用/禁用菜单

```
extension ViewController: NSMenuItemValidation {
    func validateMenuItem(_ menuItem: NSMenuItem) -> Bool {
        switch (menuItem.action) {
        // toggleSidebarメニューを条件によって有効/無効にする
        case #selector(NSSplitViewController.toggleSidebar(_:)):
            if self.hoge {
                return false
            } else {
                return true
            }
        default:
            return true
        }
    } 
}


```

### 将对象保存到文件 

```
// NSConfig プロトコルを実装
class Entity: NSObject, NSCoding {
    var id: Int = 0
    var name: String = ""

    override init() {
        super.init()
    }

    required init?(coder: NSCoder) {
        self.id = coder.decodeInteger(forKey: "id")
        self.name = coder.decodeObject(forKey: "name") as! String
    }

    func encode(with coder: NSCoder) {
        coder.encode(self.id, forKey: "id")
        coder.encode(self.name, forKey: "name")
    }
}

// ファイルへ出力
var item: Entity(id: 1, name: "hoge")
NSKeyedArchiver.archiveRootObject(item, toFile: filePath)

// ファイルから読み込み
let readedItem = NSKeyedUnarchiver.unarchiveObject(withFile: filePath) as? Entity

```






## 更改 outlineview 选中背景色默认蓝色为灰色 
继承 `NSTableRowView` 并实现以下方法

```

        override var isEmphasized: Bool {
            set {
                super.isEmphasized = newValue
            }
            get {
                if(needSelectedColor) {
                    return true
                }
                return false
            }
    
        }

```

## 去除  outlineview 选中的额背景颜色 

继承 `NSTableRowView` 并实现以下方法 
```
extension NavigatorSidebarRowView {
  
    // TODO: 通过这种方法隐藏 蓝色背景
    override func didAddSubview(_ subview: NSView) {
        super.didAddSubview(subview)
        if !needSelectedColor , subview is NSVisualEffectView {
            subview.isHidden = true
        }
    }
}

```










