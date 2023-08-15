
## 临时权限
### 方式1 
```
   let openPanel = NSOpenPanel()
    openPanel.message = &quot;Choose your directory&quot;
    openPanel.prompt = &quot;Choose&quot;
    openPanel.allowedFileTypes = [&quot;none&quot;]
    openPanel.allowsOtherFileTypes = false
    openPanel.canChooseFiles = false
    openPanel.canChooseDirectories = true

    let response = openPanel.runModal()
    print(openPanel.urls) // this contains the chosen folder
    return openPanel.urls.first
```
> 此种在app 推出时，就会失效 


### 方式2  拖拽实现
```
protocol DragDelegate : class {
    func handleDrag(from view: DragHandlingView, dragInfo: NSDraggingInfo) -&gt; Bool
}

class DragHandlingView: NSView {
    weak var delegate: DragDelegate?

    override func draggingEntered(_ sender: NSDraggingInfo) -&gt; NSDragOperation {
        return .generic // this controls what the icon does (&quot;copy&quot;, &quot;move&quot;, &quot;link&quot;, etc)
    }

    override func performDragOperation(_ sender: NSDraggingInfo) -&gt; Bool {
        return delegate?.handleDrag(from: self, dragInfo: sender) ?? false
    }
}
```

```
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
```

```
func handleDrag(from view: DragHandlingView, dragInfo: NSDraggingInfo) -> Bool {
    // the dragInfo contains a &quot;pasteboard&quot; with our data
    if let fileURL = NSURL(from: dragInfo.draggingPasteboard) as URL? {
        print("FILE: \(fileURL)")
        var isDir: ObjCBool = false
        _ = FileManager.default.fileExists(atPath: fileURL.path, isDirectory: &amp;isDir)

        if !isDir.boolValue {
            let alert = NSAlert()
            alert.alertStyle = .warning
            alert.messageText = "Drag a folder instead"
            alert.runModal()
            return false
        } else {
            workingDirectoryChosen(fileURL)
        }
    }
    return true
}
```

> 默认情况下，上述方法会在应用保持打开状态时授予您访问权限。当您退出应用程序时，您拥有的任何文件夹访问权限都将丢失。


## 持久访问 

1. 添加权限(可以不添加) 

   ```
     <key>com.apple.security.files.bookmarks.app-scope</key>
     <true/>
   ```
   
2. 使用临时授权后，给文件夹添加 bookmark  

   ```  
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
3. 在随后的启动中，我们可以检查此数据是否存在并尝试加载 URL（保留其权限）. 

```
private func restoreFileAccess(with bookmarkData: Data) -> URL? {
        do {
            var isStale = false
            let url = try URL(resolvingBookmarkData: bookmarkData, options: .withSecurityScope, relativeTo: nil, bookmarkDataIsStale: &amp;isStale)
            if isStale {
                // bookmarks could become stale as the OS changes
                print(&quot;Bookmark is stale, need to save a new one... &quot;)
                saveBookmarkData(for: url)
            }
            return url
        } catch {
            print(&quot;Error resolving bookmark:&quot;, error)
            return nil
        }
    }


```

### 开始/停止请求访问 
当我们准备好读取文件时，我们必须将其包装在一对调用中，以发出我们想要访问资源的信号： 

```
if !workingDir.startAccessingSecurityScopedResource() {
    print("startAccessingSecurityScopedResource returned false. This directory might not need it, or this URL might not be a security scoped URL, or maybe something's wrong?")
}

paths = try FileManager.default.contentsOfDirectory(at: workingDir, includingPropertiesForKeys: nil, options: .skipsHiddenFiles)
            .map {
                $0.relativePath.replacingOccurrences(of: workingDir.path, with: &quot;&quot;)
            }
workingDir.stopAccessingSecurityScopedResource()

```

`startAccessingSecurityScopedResource()`返回false 的情况：  

* 用户确实无权访问此
* 该 URL 不是安全范围的 URL 
* 这个目录不需要它（还记得~/Downloads上面的等吗？） 



