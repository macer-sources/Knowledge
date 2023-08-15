
## NSSplitViewController 
1. 隐藏item
      ```swift
        // 方式1 
        self.splitViewItems.first?.isCollapsed.toggle()
        
        //方式2
      toggleSidebar(_ sender: Any?)
      
      ```


## NSSplitView 
1. 隐藏 item
   ```swift
     public extension NSSplitView {
        func collapseOrExpand(subviewAt subview: NSView, animated: Bool) {
            if animated {
                subview.animator().isHidden = !isSubviewCollapsed(subview)
            }else {
                subview.isHidden = !isSubviewCollapsed(subview)
            }
        }
        func collapseOrExpand(viewIndex subViewIndex: UInt, animated: Bool) {
            if subViewIndex > self.arrangedSubviews.count {
                return
            }
            collapseOrExpand(subviewAt: self.arrangedSubviews[Int(subViewIndex)], animated: animated)
        }
  }
   ```
 > 需要设置 delegate 方法， canCollapseSubview 为true，否则只是隐藏，其他item 无法联动


2. 代理方法之间的互斥关系 
   如果实现了 proposedMinimumPosition / proposedMaximumPosition 代理方法， 则constrainSplitPosition将不会掉用


