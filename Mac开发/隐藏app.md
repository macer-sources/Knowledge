[通过家庭控制实现](https://developer.apple.com/documentation/bundleresources/entitlements/com_apple_developer_family-controls)



# 隐藏app 
```swift 
let instagram = Application(bundleIdentifier: "com.burbn.instagram")
let store = ManagedSettingsStore()
store.application.blockedApplications = [instagram]
```


以下是测试代码：
```
let mIg = WebDomain.init(domain: "www.instagram.com") let ingredients: Set = [mIg] let webContents = WebContentSettings.FilterPolicy.all(except:ingredients) store.webContent.blockedByFilter = webContents
```