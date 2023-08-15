## 参考 
    * https://www.jianshu.com/p/8ebf09a8cb9a

1. 获取文件的类型
`NSString *fileType = [[NSWorkspace sharedWorkspace] typeOfFile:@"文件路径" error:nil];` 

2. 打开Finder并选中某个文件

`[[NSWorkspace sharedWorkspace] selectFile:@"文件路径" inFileViewerRootedAtPath:@""];`

3. 获取某个文件类型的本地化描述 

`NSString *desc = [[NSWorkspace sharedWorkspace] localizedDescriptionForType:fileType];` 

4. 打开URL 

`[[NSWorkspace sharedWorkspace] openURL:url];`

5. 获取文件的icon. 可以使用此函数获取文件的默认图标。

`NSImage *image = [[NSWorkspace sharedWorkspace] iconForFile:@"文件路径"];` 

6. 其他操作 
        * duplicateURLs:completionHandler: - 以 Finder 操作的相同方式异步复制指定的 URL。
        * recycleURLs:completionHandler: - 以 Finder 操作的相同方式，移动指定的url到废纸篓
        * activateFileViewerSelectingURLs: 激活Finder，根据指定的多个文件，打开一个或者多个windows并且选中他们。

7. 打开操作 
    * openURL:，打开指定的URL;
    * openFile: ，根据文件类型使用默认的app打开指定的文件
    * openFile:withApplication: ，使用指定的app打开文件
    * openFile:withApplication:andDeactivate: 使用指定的app打开文件， andDeactivate传入YES，如果发送请求前app在后台禁用，允许已经打开的app到前台。
    * 打开偏好设置 [[NSWorkspace sharedWorkspace] openFile:@"/System/Library/PreferencePanes/Security.prefPane”];  
    ![](/i/cfa71cb8-0355-4570-b07e-ef8ef87d8ace.jpg)




8. 文件信息 
    * getInfoForFile:application:type: 检索指定的文件的信息。 
    * URLForApplicationToOpenURL: 返回将用于打开给定的URL的默认的app的URL。
    * fullPathForApplication: 返回指定app的全路径
    * getFileSystemInfoForPath:isRemovable:isWritable:isUnmountable:description:type: 用fullPath描述文件系统。
    * isFilePackageAtPath: 确定指定的路径是不是一个文件包。 

```
@property(readonly, strong) NSRunningApplication frontmostApplication; 返回最前面的应用程序,接收事件的app。
@property(readonly, copy) NSArray<NSRunningApplication > runningApplications; , 返回正在运行的app
@property(readonly, strong) NSRunningApplication menuBarOwningApplication; 返回属于当前显示的menubar的app
```
    
    
   
  9. 统一标识符 (Uniform Type Identifier) 
    * typeOfFile:error: 返回指定文件的统一类型标识符，如果他能被探测到的话。 
    * localizedDescriptionForType: 返回指定统一类型标识符的本地化描述
    * preferredFilenameExtensionForType: 返回指定统一类型标识符的文件后缀名
    * filenameExtension:isValidForType: 返回是否指定文件后缀是否适合统一类型标识符
    * type:conformsToType: 返回一个布尔值表示第一个统一类型标识符是否符合第二个统一类型标识符。
    * URLForApplicationWithBundleIdentifier: 返回相对于app指定标识符的url



10. 管理图标 
    * iconForFile: 返回指定文件包含的图标图片
    * iconForFileType: 返回指定类型指定文件包含的图标文件
    * iconForFiles: 返回指定多个文件包含的图标文件
    * setIcon:forFile:options: 带着附加的选项，为指定的路径文件或者目录 设置图标

```
- (void)getIconOfFile
{
    NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
    
    NSString *path = @"/Users/MelissaShu/Pictures/背景图/background0.png";
    NSURL *fileURL = [NSURL fileURLWithPath: path];
    self.imgView.image = [workspace iconForFile:[fileURL path]];//不显示图片，而是现实 图片类型文件 对应的图标
    
    NSString *path2 = [workspace fullPathForApplication:@"Safari"];
    self.imgView1.image  = [workspace iconForFile:path2];
    
    NSImage *xcodeIcon = [workspace iconForFileType:@"xcodeproj"];
    self.imgView2.image = xcodeIcon;
}
```
11. 卸载设备 
    * unmountAndEjectDeviceAtPath: 在指定的路径卸载和弹出设备。 
    * unmountAndEjectDeviceAtURL:error: 尝试弹出安装在指定的路径的卷。 
12. 管理Bundles 
    * absolutePathForAppBundleWithIdentifier: 返回一个app bundle在文件系统的绝对路径 
    * launchAppWithBundleIdentifier:options:additionalEventParamDescriptor:launchIdentifier: 指定 bundleIdentifier 启动该应用程序。
    * openURLs:withAppBundleIdentifier:options:additionalEventParamDescriptor:launchIdentifiers: 从一个url数组打开一个或者多个文件



13. 管理桌面图片 
    * desktopImageURLForScreen: 返回给定屏幕的桌面图片
    * setDesktopImageURL:forScreen:options:error: 指定给定的屏幕与图片url，为桌面设置图片
    * desktopImageOptionsForScreen: 返回给定屏幕的桌面图片选项
    * showSearchResultsForQueryString: 显示 Spotlight 搜索结果窗口
    ![](/i/271a341a-82b4-46dd-a08d-d37f511be720.jpg)




13. 跟踪文件系统变化 
    * noteFileSystemChanged: 通知workspace对象，文件系统在指定的路径发生变化。注销前请求额外的时间
    * extendPowerOffBy: 当关闭电源注销用户的时候，请求系统等待自定数量的时间
