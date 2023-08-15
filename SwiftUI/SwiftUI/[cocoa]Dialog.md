
# Cocoa Dialog

## Alert 
```swift 

  Alert(title: Text("Alert"),
        message: Text("This is an alert!"),
        primaryButton: .default(Text("OK"), action: {
          self.dialogResult = "OK clicked in Alert"
        }), secondaryButton: .cancel({
          self.dialogResult = "Cancel clicked in Alert"
        }))
```
## Action 
__ActionSheet 在 macOS 中不可用__  
```swift 



```
## Sheet
```swift 

.sheet(isPresented: $sheetIsShowing) {
    SheetView(isVisible: self.$sheetIsShowing)
}


```
## File dialogs(save & open)
```swift 

let panel = NSSavePanel()
panel.nameFieldLabel = "Save cat image as:"
panel.nameFieldStringValue = "cat.jpg"
panel.canCreateDirectories = true
panel.begin { response in
    if response == NSApplication.ModalResponse.OK, let fileUrl = panel.url {
        print(fileUrl)
    }
}
```