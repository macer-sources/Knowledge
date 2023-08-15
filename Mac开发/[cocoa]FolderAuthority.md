# Folder 权限获取 

[PermissionsKit](https://github.com/MacPaw/PermissionsKit)

### 授予访问文件夹的权限 
> 没有 API 可以表示“请提示用户访问此文件夹”。(没有API 直接获取权限， 但可以通过一下方式获取权限提示)  
* 全磁盘访问
* 提示用户打开文件/目录
* 将文件夹拖放到应用程序上



### 全磁盘访问 
> 并不适合大多数应用程序
此种方式需要用户主动操作才能完成， 需要打开“安全”首选项窗格，进入“完全磁盘访问”





## 临时权限获取  

### NSOpenPanel 的细粒度权限  
__目的__ : 读取（和写入）应用程序沙箱之外的单个文件夹的权限  
> File Access - User Selected File ===== `Read/Write`


获取文件目录
```swift 

private func promptForWorkingDirectoryPermission() -> URL? {
    let openPanel = NSOpenPanel()
    openPanel.message = "Choose your directory"
    openPanel.prompt = "Choose"
    openPanel.allowedFileTypes = ["none"]
    openPanel.allowsOtherFileTypes = false
    openPanel.canChooseFiles = false
    openPanel.canChooseDirectories = true

    let response = openPanel.runModal()
    print(openPanel.urls) // this contains the chosen folder
    return openPanel.urls.first
}

```

> 当用户退出应用程序时，此访问权限将过期  



### 拖放 
> 让用户将文件夹拖放到您的应用程序中,然后获取到权限

```swift  
protocol DragDelegate : class {
    func handleDrag(from view: DragHandlingView, dragInfo: NSDraggingInfo) -> Bool
}

class DragHandlingView: NSView {
    weak var delegate: DragDelegate?

    override func draggingEntered(_ sender: NSDraggingInfo) -> NSDragOperation {
        return .generic // this controls what the icon does ("copy", "move", "link", etc)
    }

    override func performDragOperation(_ sender: NSDraggingInfo) -> Bool {
        return delegate?.handleDrag(from: self, dragInfo: sender) ?? false
    }
}


class WorkingDirectoryPromptViewController: NSViewController, DragDelegate {
    override func viewDidLoad() {
        super.viewDidLoad()
        registerForDragDrop()
    }

    private func registerForDragDrop() {
        (view as! DragHandlingView).delegate = self
  
        // we don't need file contents, just the location
        view.registerForDraggedTypes([.fileURL])
    }
    
    // ...
}

func handleDrag(from view: DragHandlingView, dragInfo: NSDraggingInfo) -> Bool {
    // the dragInfo contains a "pasteboard" with our data
    if let fileURL = NSURL(from: dragInfo.draggingPasteboard) as URL? {
        print("FILE: \(fileURL)")
        var isDir: ObjCBool = false
        _ = FileManager.default.fileExists(atPath: fileURL.path, isDirectory: &isDir)

        if !isDir.boolValue {
            let alert = NSAlert()
            alert.alertStyle = .warning
            alert.messageText = "Drag a folder instead."
            alert.runModal()
            return false
        } else {
            workingDirectoryChosen(fileURL)
        }
    }
    return true
}

```


## 持久权限 
[文档](https://developer.apple.com/library/archive/documentation/Security/Conceptual/AppSandboxDesignGuide/AppSandboxInDepth/AppSandboxInDepth.html#//apple_ref/doc/uid/TP40011183-CH3-SW16)  
1. 添加权限(可以不需要添加了)
```xml
<key>com.apple.security.files.bookmarks.app-scope</key>
<true/>

```

2. 使用以上的方式获取权限后，保存 Bookmark 
```swift 
private func saveBookmarkData(for workDir: URL) {
    do {
        let bookmarkData = try workDir.bookmarkData(options: .withSecurityScope, includingResourceValuesForKeys: nil, relativeTo: nil)

        // save in UserDefaults
        Preferences.workingDirectoryBookmark = bookmarkData
    } catch {
        print("Failed to save bookmark data for \(workDir)", error)
    }
}

```

3. 重启时， 重新加载权限
```swift 

private func restoreFileAccess(with bookmarkData: Data) -> URL? {
    do {
        var isStale = false
        let url = try URL(resolvingBookmarkData: bookmarkData, options: .withSecurityScope, relativeTo: nil, bookmarkDataIsStale: &isStale)
        if isStale {
            // bookmarks could become stale as the OS changes
            print("Bookmark is stale, need to save a new one... ")
            saveBookmarkData(for: url)
        }
        return url
    } catch {
        print("Error resolving bookmark:", error)
        return nil
    }
}

```

3. 在进行访问时， 必须告诉系统何时要使用“资源”以及何时完成 
> 当准备好读取文件时，必须将其包装在`ScopedResource`中以表明我们想要访问资源：
```swift 

if !workingDir.startAccessingSecurityScopedResource() {
    print("startAccessingSecurityScopedResource returned false. This directory might not need it, or this URL might not be a security scoped URL, or maybe something's wrong?")
}

paths = try FileManager.default.contentsOfDirectory(at: workingDir, includingPropertiesForKeys: nil, options: .skipsHiddenFiles)
            .map {
                $0.relativePath.replacingOccurrences(of: workingDir.path, with: "")
            }

workingDir.stopAccessingSecurityScopedResource()
```
* startAccessingSecurityScopedResource 返回false 的情况： 
    * 用户确实无权访问此内容 
    * 该 URL 不是安全范围的 URL
    * 该目录不需要它（~/Downloads 等）  


## 问题 
* 注意符号链接  
    无法直接访问 链接的文件夹， 必须授予对真实文件夹的权限  
* 留意已经具有访问权限的文件夹  
* 留意 Dropbox 和其他文件系统插件  




## 代码参考 
* 权限
```xml
<key>com.apple.security.files.user-selected.read-write</key>
<true/>
<key>com.apple.security.files.bookmarks.app-scope</key>
<true/>
```


```swift 

import Foundation
import Cocoa

var bookmarks = [URL: Data]()

func openFolderSelection() -> URL?
{
    let openPanel = NSOpenPanel()
    openPanel.allowsMultipleSelection = false
    openPanel.canChooseDirectories = true
    openPanel.canCreateDirectories = true
    openPanel.canChooseFiles = false
    openPanel.begin
        { (result) -> Void in
            if result.rawValue == NSApplication.ModalResponse.OK.rawValue
            {
                let url = openPanel.url
                storeFolderInBookmark(url: url!)
            }
    }
    return openPanel.url
}

func saveBookmarksData()
{
    let path = getBookmarkPath()
    NSKeyedArchiver.archiveRootObject(bookmarks, toFile: path)
}

func storeFolderInBookmark(url: URL)
{
    do
    {
        let data = try url.bookmarkData(options: NSURL.BookmarkCreationOptions.withSecurityScope, includingResourceValuesForKeys: nil, relativeTo: nil)
        bookmarks[url] = data
    }
    catch
    {
        Swift.print ("Error storing bookmarks")
    }

}

func getBookmarkPath() -> String
{
    var url = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0] as URL
    url = url.appendingPathComponent("Bookmarks.dict")
    return url.path
}

func loadBookmarks()
{
    let path = getBookmarkPath()
    bookmarks = NSKeyedUnarchiver.unarchiveObject(withFile: path) as! [URL: Data]
    for bookmark in bookmarks
    {
        restoreBookmark(bookmark)
    }
}



func restoreBookmark(_ bookmark: (key: URL, value: Data))
{
    let restoredUrl: URL?
    var isStale = false

    Swift.print ("Restoring \(bookmark.key)")
    do
    {
        restoredUrl = try URL.init(resolvingBookmarkData: bookmark.value, options: NSURL.BookmarkResolutionOptions.withSecurityScope, relativeTo: nil, bookmarkDataIsStale: &isStale)
    }
    catch
    {
        Swift.print ("Error restoring bookmarks")
        restoredUrl = nil
    }

    if let url = restoredUrl
    {
        if isStale
        {
            Swift.print ("URL is stale")
        }
        else
        {
            if !url.startAccessingSecurityScopedResource()
            {
                Swift.print ("Couldn't access: \(url.path)")
            }
        }
    }

}

```

* 代码调用 
```swift 
// 打开NSOpenPanel：
let selectedURL = openFolderSelection()
saveBookmarksData()

// 并在应用程序关闭后加载现有书签：
loadBookmarks()
```






## 参考 
* [AppSandboxFileAccess](https://github.com/leighmcculloch/AppSandboxFileAccess)

