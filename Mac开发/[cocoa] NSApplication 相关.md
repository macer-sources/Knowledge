
# NSApplication 相关


## 1. Mac下处理自定义URL Scheme 
```c++
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    [[NSAppleEventManager sharedAppleEventManager] setEventHandler:self andSelector:@selector(handleURLEvent:withReplyEvent:) forEventClass:kInternetEventClass andEventID:kAEGetURL];
}

- (void)handleURLEvent:(NSAppleEventDescriptor*)theEvent withReplyEvent:(NSAppleEventDescriptor*)replyEvent {
    // Process URL Request
    NSString* path = [[theEvent paramDescriptorForKeyword:keyDirectObject] stringValue];

    [[NSAlert alertWithMessageText:@"URL Request" defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:@"%@", path] runModal];
}
```


## 2. 把程序移动到/Applications文件夹 
> [LetsMove](http://cocoa.venj.me/blog/move-app-to-application-folder-at-runtime/)
```c++

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    PFMoveToApplicationsFolderIfNecessary(); // 增加这行代码。

    // Other Code…
}

```
