## 隐藏app

> 在iOS 中，可以使用家长控制来隐藏app 启动。本文将介绍在Mac中隐藏app 的方式

```swift 
// familyActivityPicker 


let instagram = Application(bundleIdentifier: "com.burbn.instagram")
let store = ManagedSettingsStore()
store.application.blockedApplications = [instagram]

 let mIg = WebDomain.init(domain: "www.instagram.com") let ingredients: Set = [mIg] let webContents = WebContentSettings.FilterPolicy.all(except:ingredients) store.webContent.blockedByFilter = webContents

```

### 参考 
https://www.folio3.com/mobile/blog/screentime-api-ios/
https://hacknicity.medium.com/hide-sensitive-information-in-the-ios-app-switcher-snapshot-image-25ddc9b8ef5f





## Mac 中隐藏app
### 方式1 
```shell 
# 通过此种方式隐藏app 
 sudo chflags hidden iBooks.app
# 显示app 
 sudo chflags hidden iBooks.app 
 # 刷新启动台缓存 
 defaults write com.apple.dock ResetLaunchPad -bool true; killall Dock

```

> 此种方式有个缺点： 就是知识隐藏了app，如果使用`显示隐藏文件`的方式，还是能看到app的，并且能正确打开 




### 方式2 
> 直接修改`launchpad` 数据库 
```shell
# 旧的命令
sqlite3 ~/Library/Application Support/Dock/*.db "DELETE from apps WHERE title='APPNAME';" && killall Dock

#  Sierra上不起作用了
sqlite3 $(sudo find /private/var/folders -name com.apple.dock.launchpad)/db/db "DELETE FROM apps WHERE title='APPNAME';" && killall Dock

sqlite3 $(sudo find /private/var/folders ( -name com.apple.dock.launchpad -a -user $USER ) 2> /dev/null)/db/db "DELETE FROM apps WHERE title='AppNameHere';" && killall Dock

```
