1. 创建toolbar 
```swift

    private lazy var toolbar: NSToolbar = {
        let toolbar = NSToolbar.init(identifier: NSToolbar.Identifier.mainToolbar)
        toolbar.allowsUserCustomization = false
        toolbar.autosavesConfiguration = true
        toolbar.displayMode = .default
//        toolbar.centeredItemIdentifier = NSToolbarItem.Identifier.mainToolbarItem
        toolbar.sizeMode = .default
        toolbar.delegate = self
        toolbar.isVisible = true
        toolbar.showsBaselineSeparator = false
        
        return toolbar
    }()

```

2. 将toolbar 与 window 关联
```swift
 window?.toolbar = toolbar

```

3. 代理方法中处理tool item 显示问题 
```swift 


extension MainWindowController : NSToolbarDelegate, NSToolbarItemValidation {
    func validateToolbarItem(_ item: NSToolbarItem) -> Bool {
        return true
    }
    // 只有存在自定义 identifier 的时候才会调用，否则不调用
    func toolbar(_ toolbar: NSToolbar, itemForItemIdentifier itemIdentifier: NSToolbarItem.Identifier, willBeInsertedIntoToolbar flag: Bool) -> NSToolbarItem? {
      // 此处实现一个group，将多个item 编组进行处理
        let item = NSToolbarItemGroup(itemIdentifier: NSToolbarItem.Identifier.mainToolbarItem,
                                      titles: ["Listen Now", "Browse", "Radio"],
                                      selectionMode: .selectOne,
                                      labels: ["Listen Now", "Browse", "Radio"],
                                      target: self,
                                      action: #selector(didChangeSelection(_:)))
        item.label = "Navigator"
        item.paletteLabel = "Navigator"
        item.toolTip = "Switch between your source of music."
        item.isBordered = true
        item.visibilityPriority = .high
        item.selectionMode = .selectOne
        item.controlRepresentation = .automatic
        return item
    }
      // 决定默认显示的item
    func toolbarDefaultItemIdentifiers(_ toolbar: NSToolbar) -> [NSToolbarItem.Identifier] {
        return [
            NSToolbarItem.Identifier.flexibleSpace,
            NSToolbarItem.Identifier.layoutItem,
            NSToolbarItem.Identifier.exportItem
        ]
    }
    // 决定是否显示item 
    func toolbarAllowedItemIdentifiers(_ toolbar: NSToolbar) -> [NSToolbarItem.Identifier] {
        return [
            NSToolbarItem.Identifier.flexibleSpace,
            NSToolbarItem.Identifier.layoutItem,
            NSToolbarItem.Identifier.exportItem
        ]
    }
    

    @objc private func didChangeSelection(_ sender: NSToolbarItem) {
        
    }
    
}

```
