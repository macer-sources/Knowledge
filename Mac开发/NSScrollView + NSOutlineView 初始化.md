
```
  private lazy var scrollableOutlineView: (scrollView: NSScrollView, outlineView: NSOutlineView) = {

    let outlineColumn = NSTableColumn()
    outlineColumn.resizingMask = .autoresizingMask
    outlineColumn.isEditable = false

    let outlineView = NSOutlineView()
    outlineView.headerView = nil
    outlineView.columnAutoresizingStyle = .uniformColumnAutoresizingStyle
    outlineView.addTableColumn(outlineColumn)
    outlineView.outlineTableColumn = outlineColumn
    outlineView.columnAutoresizingStyle = .firstColumnOnlyAutoresizingStyle
    outlineView.focusRingType = .none
    outlineView.allowsMultipleSelection = true
    outlineView.usesAutomaticRowHeights = true
    outlineView.selectionHighlightStyle = .sourceList
    outlineView.floatsGroupRows = false

    let scrollView = NSScrollView()
    scrollView.documentView = outlineView
    scrollView.hasVerticalScroller = true
    scrollView.autohidesScrollers = true
    scrollView.drawsBackground = false
    return (scrollView, outlineView)
  }()


```