
# Text Field 相关

## 简介 
* NSTextField   
    Text Field 由 `NSTextFieldCell` 和 `NSTextField` 组成，__NSTextFieldCell 实现了大多数方法，NSTextField 继承自 NSControl，作为 NSTextFieldCell 的 Container 为其所有方法进行封装__ 。NSTextField 提供了一些类似 textDidBeginEditing: 的 delegate 方法。
* NSTextView  
    NSTextView 是 NSText 的子类。  
    Text View 通常用于多行带有样式的复杂文字编辑。用户可以控制 Text View 的文字内容、字体、颜色、样式和其他属性。    
* Field Editor
    __Field Editor 是一个 Window 中所有控件共享的一个 NSTextView。__
    这个被共享的 Text View 会自动的插入 View Hierarchy，为正在 Editing 的 Text Field 提供文字编辑的功能，处理键盘事件和显示文字
    [Working with the Field Editor](https://link.juejin.cn/?target=https%3A%2F%2Fdeveloper.apple.com%2Flibrary%2Fmac%2Fdocumentation%2FTextFonts%2FConceptual%2FCocoaTextArchitecture%2FTextEditing%2FTextEditing.html%23%2F%2Fapple_ref%2Fdoc%2Fuid%2FTP40009459-CH3-SW29)

### 键盘事件处理 
键盘事件最终会进入 `keyDown:` 方法中，在 NSTextField 与 NSTextView 中，会由 `interpretKeyEvents:` 并根据键盘事件是否有绑定的 Command，向调用者发送 `doCommandBySelector:` 或 `insertText: `消息。
```swift 
    func control(control: NSControl, textView: NSTextView, doCommandBySelector commandSelector: Selector) -> Bool {
        if commandSelector == #selector(insertNewline(_:)) {
            if let modifierFlags = NSApplication.sharedApplication().currentEvent?.modifierFlags
                where (modifierFlags.rawValue & NSEventModifierFlags.ShiftKeyMask.rawValue) != 0 {
                print("Shift-Enter detected.")
            } else {
                print("Enter detected.")
            }
            textView.insertNewlineIgnoringFieldEditor(self)

            return true
        }

        return false
    }

// return true 。说明delegate处理了事件， 系统将不再执行 commandSelector ， 返回 false 系统则会进行默认的处理   
// enter ======> insertNewline:  
// 通过 NSApplication 中 currentEvent?.modifierFlags 判断是否有组合键
```





#### NSTextField & NSTextView 区别 
对于 Enter 和 Tab 键的行为不同。  NSTextField 类似其他非文本编辑的 Cocoa 控件：Enter 键触发终止编辑，Tab 键令焦点移到相邻下一控件；NSTextView 则给编辑内容添加换行或者 tab 字符。







## 1. 点击按钮，复制粘贴到文本框
> 复制粘贴的菜单操作和快捷键操作, 他们属于“nil-targeted action”, __他们的receive是在运行时被指定的。__ 在Interface Builder中，-copy:，-paste:方法都是连接到一个占位对象“First Responder”上的。而First Responder到底是谁是在程序运行过程中动态变化着的。 

> 在NSTextField这个例子中，-copy:和-paste:的Receive是一个NSText对象  

> 原理： 在NSWindow中，有一个方法能够返回当前处于First Responder的NSText对象: -fieldEditor:forObject:  

```c++

- (IBAction)copyText:(id)sender {
    [self.sourceField becomeFirstResponder];  //让Source文本框变成First Responder
    NSText *sourceText = [self.window fieldEditor:YES forObject:self.sourceField];  //返回sourceField的NSText对象
    [sourceText selectAll:nil];  // 全选
    [sourceText copy:nil];  // 复制
}

- (IBAction)pasteText:(id)sender {
    [self.targetField becomeFirstResponder];  //让Target文本框变成First Responder
    NSText *targetText = [self.window fieldEditor:YES forObject:self.targetField];  //返回targetField的NSText对象
    self.targetField.stringValue = @"";  //清空原来文本框中的值
    [targetText paste:nil];  //粘贴
}

```


### 2. 超链接 
```swift 
let label = NSTextField(labelWithString: "")
let mutableString = NSMutableAttributedString()
let hyperString = NSAttributedString.init(string: "wwww.apple.com", attributes: [NSAttributedString.Key.link: "www.apple.com"])
mutableString.append(hyperString)

// TODO: Textfield 设置
label.attributedStringValue = mutableString
label.backgroundColor = .clear
label.isEditable = false// 取消编辑
label.isBordered = false // 取消边框
label.isSelectable = true // 设置可选中， 必须设置，否则选不中超链接
label.allowsEditingTextAttributes = true // 必须设置， 否则点击超链接不会有反应

```