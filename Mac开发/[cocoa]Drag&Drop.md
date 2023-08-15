# Drag & Drop 

## 启用系统自带的 
```swift 

tableView.draggingDestinationFeedbackStyle = .Gap  
```




## Drag & Drop =====> NSTableView 

### 注册dragged types 
```swift 

let registeredTypes:[String] = [NSStringPboardType]  
tableView.registerForDraggedTypes(registeredTypes)  
```

### Drag 时,写入剪切板 
```swift 
func tableView(tableView: NSTableView, writeRowsWithIndexes rowIndexes: NSIndexSet, toPasteboard pboard: NSPasteboard) -> Bool {  
    let currencyCode = // Get Currency Code At Row
    let registeredTypes:[String] = [NSStringPboardType]
    pboard.declareTypes(registeredTypes, owner: self)
    pboard.setString(currencyCode, forType: NSStringPboardType)
    return true
}
```

### Drop 操作 
1. 首先根据用户拖拽 Row 的位置来返回适当的操作，在这里，我们希望仅当用户插入两行之间时执行 Move 操作：  
```swift  
func tableView(tableView: NSTableView, validateDrop info: NSDraggingInfo, proposedRow row: Int, proposedDropOperation dropOperation: NSTableViewDropOperation) -> NSDragOperation {  
    if (dropOperation == .Above) { return .Move }
    return .None;
}
```
2. 对于 Valid Drop 操作，通过实现下述方法获取剪贴板上的信息并依次进行数据排序，更新 Table View 
```swift 
func tableView(tableView: NSTableView, acceptDrop info: NSDraggingInfo, row: Int, dropOperation: NSTableViewDropOperation) -> Bool {  
    if let currencyCode = info.draggingPasteboard().stringForType(NSStringPboardType),
       let oldIndex = userCurrencyCodes.indexOf(currencyCode) {
        // Update Data Source
        // Update Table View
        return true
    } else {
        return false
    }
}


```





