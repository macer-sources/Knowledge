
## 简介  


[github](https://github.com/gbammc/Thor.git)  



## 内容  


1. 获取任意app的icon  


```swift

    // bundle
    let appBundleURL: URL
    // app name
    let appName: String
    // app display name
    let appDisplayName: String
    // app icon name
    let appIconName: String

var icon: NSImage? { 

        guard let bundle = Bundle(url: appBundleURL), let bundleIdentifier = bundle.bundleIdentifier else {
            return nil
        }

        let compositeName = "\(bundleIdentifier):\(appIconName)"

        guard let file = bundle.pathForImageResource(appIconName),
            let bundleImage = NSImage(contentsOfFile: file) else {
                return nil
        }

        bundleImage.setName(compositeName)
        bundleImage.size = NSSize(width: 36, height: 36)

        return bundleImage
    }

```

2. 选择文件或文件夹，并获取 metadata  

```swift  

        let openPanel = NSOpenPanel()
        openPanel.allowsMultipleSelection = false
        openPanel.canChooseDirectories = true
        openPanel.canChooseFiles = true
        openPanel.allowedFileTypes = [kUTTypeApplicationFile as String, kUTTypeApplicationBundle as String]
        if let appDir = NSSearchPathForDirectoriesInDomains(.applicationDirectory, .localDomainMask, true).first {
            openPanel.directoryURL = URL(fileURLWithPath: appDir)
        }
        openPanel.beginSheetModal(for: view.window!, completionHandler: { (result) in
            if result == NSApplication.ModalResponse.OK, let metaDataItem = NSMetadataItem(url: openPanel.urls.first!) {
                let app = AppModel(item: metaDataItem)

                AppsManager.manager.save(app, shortcut: nil)

                self.tableView.reloadData()
                self.tableView.scrollRowToVisible(self.apps.count - 1)
            }
        })

       guard let path = item.value(forAttribute: kMDItemPath as String) as? String,
            let displayName = item.value(forAttribute: kMDItemDisplayName as String) as? String,
            let name = item.value(forKey: kMDItemFSName as String) as? String,
            let appBundle = Bundle(path: path),
            let iconName = (appBundle.infoDictionary?["CFBundleIconFile"]) as? String else {
                return nil
        }
```


3. 简单数据的归档  

```swift 
    // 归档路径
    private var selectedAppsFilePath: String {
        let dir = NSSearchPathForDirectoriesInDomains(.applicationSupportDirectory, .userDomainMask, true).first
        let key = kCFBundleNameKey as String

        guard let appSupportDir = dir, let appName = Bundle.main.infoDictionary![key] as? String else {
            return ""
        }

        let appDir = appSupportDir.appendingPathComponent(appName)
        if !FileManager.default.fileExists(atPath: appDir) {
            _ = try? FileManager.default.createDirectory(atPath: appDir, withIntermediateDirectories: true)
        }

        return appDir.appendingPathComponent("apps")
    }


    // 归档  
    NSKeyedArchiver.archiveRootObject(apps, toFile: path)
    // 反归档 
    NSKeyedUnarchiver.unarchiveObject(with: data) as? // 转为具体的数据类型，一般采用的是 dic  
```











## 扩展  
1. NSApplication 

```swift 

extension NSApplication {

    // 版本号
    class func shortVersionString() -> String {
        guard let infoDict = Bundle.main.infoDictionary else {
            return "Unknown"
        }

        return (infoDict["CFBundleShortVersionString"] as? String) ?? ""
    }

    // 构建号
    class func buildVersionString() -> String {
        guard let infoDict = Bundle.main.infoDictionary else {
            return "?"
        }

        return (infoDict["CFBundleVersion"] as? String) ?? ""
    }

    // 显示版本
    class func formattedVersion() -> String {
        return "Version \(shortVersionString()) (\(buildVersionString()))"
    }
}


```

```swift   
    var startAtLogin: Bool {
        get {
            if defaults.value(forKey: DefaultsKeys.LaunchAtLoginKey.key) == nil {
                defaults[.LaunchAtLoginKey] = itemRefInLoginItems().thisRef != nil
            }
            return defaults[.LaunchAtLoginKey]
        }
        set {
            defaults[.LaunchAtLoginKey] = newValue
            // 通过 ServiceManagement 实现开机自启功能
            SMLoginItemSetEnabled(launcherAppId as CFString, newValue)
        }
    }
    // LSSharedFileList： 文件系统的持久列表
    private var loginItemsRef: LSSharedFileList? {
        let items = kLSSharedFileListSessionLoginItems.takeRetainedValue()
        return LSSharedFileListCreate(nil, items, nil).takeRetainedValue()
    }

    // MARK: Methods

    func toggleStartAtLogin() {
        startAtLogin = !startAtLogin
    }

    private func itemRefInLoginItems() -> (thisRef: LSSharedFileListItem?, lastRef: LSSharedFileListItem?) {
        
        guard let ref = loginItemsRef,
            let items = LSSharedFileListCopySnapshot(ref, nil).takeRetainedValue() as? [LSSharedFileListItem] else {
                return (nil, nil)
        }

        guard items.count > 0, let lastItem = items.last else {
            return (nil, kLSSharedFileListItemBeforeFirst.takeRetainedValue())
        }

        let bundleURL = Bundle.main.bundleURL
        for item in items {
            let itemURL = LSSharedFileListItemCopyResolvedURL(item, 0, nil)
            if let itemURL = itemURL, bundleURL == URL(string: String(describing: itemURL.takeRetainedValue())) {
                return (item, lastItem)
            }
        }

        return (nil, lastItem)
    }


```
