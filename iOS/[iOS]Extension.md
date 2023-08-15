# Extension 相关 


## Group Container
> 启用 App Group 默认创建 
> 在运行时，Group Container 将自动创建，保存在 ~/Library/Group Containers/ 目录下，并能够被访问到。  

### 获取 App Group 地址 
```swift 
func containerURLForSecurityApplicationGroupIdentifier(_ groupIdentifier: String) -> URL?  
```
> 只有与 entitlement 中 com.apple.security.application-groups 匹配的 Group Identifier 才被视作 Valid  


### 将数据储存至 App Group 
> 一般来说 Container 和 Extension 共享的数据分为用户设置和数据文件两类.   
* NSUserDefaults 
```swift 
struct ApplicationGroups {  
    static let primary = "\(DevelopmentTeam.ID).\(ArbitraryStringYouLike)"
}

private var applicationUserDefaults: NSUserDefaults {  
    return NSUserDefaults(suiteName: ApplicationGroups.primary)!
}
```
* 其它文件  
Core Data，SQLite 或者图片、音频，任何文件都可以放到共享目录下供 Container 和 Extension App 共同访问。   

## Responsive  
> 当 Container 和 Extension 共享数据时，很有可能他们也会同时修改数据。为了保持数据一致，在一个进程对数据操作时我们可能会为数据加上 Exclusive Lock，此时如果 Extension 突然退出，我们希望能够告诉系统程序正在进行一些不可以被打断的工作，如果一定要马上退出请给我最后一次机会结束工作
```swift  

let pi = NSProcessInfo.processInfo()  
pi.performExpiringActivityWithReason("clean-up")  { expired in  
    if (!expired) {
        self.serializeData()
    } else {
        self.cleanupSerializingData()
    }
}

// TODO: 针对需要在 Main Queue 中完成的工作，使用 async 而应该使用 sync
let pi = NSProcessInfo.processInfo()  
pi.performExpiringActivityWithReason("clean-up") { expired in  
    if (!expired) {
        dispatch_sync(dispatch_get_main_queue(), {
            self.performInterruptibleWork()
        })
    } else {
        self.canceled = true
    }
}


```


#### 数据同步通知 
> 在 Container 和 Extension App 同时运行时，Container 中某些修改用户设置项或数据库的操作将导致 Extension 数据不同步, 通过通知实现数据同步 
```swift 
// Container 中 
// Get an instance of the Darwin Notification Center
let nc = CFNotificationCenterGetDarwinNotifyCenter()

// Post notification
CFNotificationCenterPostNotification(  
    nc,
    "com.example.app-model-updated", // Represent your notification
    nil,
    nil,
    CFBooleanGetValue(true)
)



// TODO: Extension 中 
let nc = CFNotificationCenterGetDarwinNotifyCenter()

CFNotificationCenterAddObserver(  
    nc,
    nil,
    { _ in self.reloadModel() }, // Callback block.
    "com.example.app-model-updated", // Represent the notification
    nil,
    .DeliverImmediately
)

```
__需要 Remove Observer__  


## Background Refresh  
> NCWidgetProviding 中响应系统刷新事件的方法  
```swift 
func widgetPerformUpdateWithCompletionHandler(completionHandler:  
((NCUpdateResult) -> Void)) {
  let updated = refreshTheModel()
  if (updated) { view.setNeedsDisplay() }
  completionHandler(updated ? .NewData : .NoData)
}

```




