```
-(NSSize)windowWillResize:(NSWindow *)sender
                   toSize:(NSSize)frameSize
{
    frameSize.width = frameSize.height*2;
    return frameSize;
}


```



## 1. 记忆窗口位置 
1. 在程序中记忆窗口的位置，以便在下次打开的时候恢复记忆的位置 
```c++
- (void)windowDidMove:(NSNotification *)notification {
    // 自动记忆和恢复位置
    [self.window setFrameAutosaveName:@"Position"];
    /* 如果是基于文档的程序
    [[self.window windowController] setShouldCascadeWindows:NO];
    [self.window saveFrameUsingName:[window representedFilename]];
    */
    // 手动记忆位置
    [self.window setFrameUsingName:@"Some name"];
}
```

2. 手动保存以及🆑记忆 
```c++
- (void)awakeFromNib {
    [self.window setFrameAutosaveName:@"Some name"];
}
- (IBAction)clearWindowPosition:(id)sender {
    [NSWindow removeFrameUsingName:@"Some name"];
}

```


## 2.  Sheet 弹出位置(默认在window顶部，弹出在具体位置) 
```c++
// 创建一个NSAlert，并以Sheet的形式呈现
- (IBAction)removeRow:(id)sender {
    NSAlert *alert = [NSAlert alertWithMessageText:@"Are you sure?" defaultButton:@"Yes" alternateButton:@"No" otherButton:nil informativeTextWithFormat:@"Deleting a table cell is not reversable."];
    [alert beginSheetModalForWindow:self.window modalDelegate:self didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:) contextInfo:NULL];
}

// NSWindowDelegate方法，用来指定Sheet的显示位置
- (NSRect)window:(NSWindow *)window willPositionSheet:(NSWindow *)sheet usingRect:(NSRect)rect {
    id cell = [self.tableView rowViewAtRow:[self.tableView selectedRow] makeIfNecessary:YES];
    NSRect cellRect = [cell frame];
    NSRect scrollViewRect = [self.tableView.superview frame];
    cellRect.origin.y = scrollViewRect.size.height - cellRect.origin.y; // View-based table view is flipped?
    cellRect.size.height = 0;
    return cellRect;
}

```


## 3. 在Cocoa程序中进行屏幕截图  
> 此方式使用`screencapture`命令进行截图 
```c++
- (IBAction)grabScreen:(id)sender {
    NSTask *capture = [[[NSTask alloc] init] autorelease];
    capture.launchPath = @"/usr/sbin/screencapture";
    // -i参数表示交互模式，-c参数表示把截图保存到剪贴板
    capture.arguments = [NSArray arrayWithObjects:@"-i", @"-c", nil];

    [capture setTerminationHandler: ^(NSTask *t) {
        NSPasteboard *pboard = [NSPasteboard generalPasteboard];
        NSPasteboardItem *pboardItem = [[pboard pasteboardItems] objectAtIndex:0];
        NSString *pboardItemType = [[pboard types] objectAtIndex:0];
        NSData *imageData = [pboardItem dataForType:pboardItemType];
        NSImage *image = [[NSImage alloc] initWithData:imageData];
        [self.imageView setImage:image];
        [image release];
    }];

    [capture launch];
}

```


## 4. 获取窗口应用ID 
> macOS 系统为每个屏幕分配一个 displayId，数据类型为 CGDirectDisplayID，32 位无符号整型。该 ID 对应唯一的 macOS 屏幕，通过这个ID 我们可以进行一些关于有关屏幕的操作，比如屏幕共享。 

```c++
enum
{
   kCGWindowListOptionAll                 = 0,//列出所有窗口，包括屏幕上和屏幕外的窗口。 使用此选项检索列表时，应该将relativeToWindow参数设置为kCGNullWindowID
   kCGWindowListOptionOnScreenOnly        = (1 << 0),//列出当前屏幕上的所有窗口。 Windows按从前到后的顺序返回。 使用此选项检索列表时，应该将relativeToWindow参数设置为kCGNullWindowID
   kCGWindowListOptionOnScreenAboveWindow = (1 << 1),//列出当前在屏幕上并在relativeToWindow参数指定的窗口之前的所有窗口。 Windows按从前到后的顺序返回
   kCGWindowListOptionOnScreenBelowWindow = (1 << 2),//列出当前屏幕上以及在relativeToWindow参数指定的窗口之后的所有窗口。 Windows按从前到后的顺序返回
   kCGWindowListOptionIncludingWindow     = (1 << 3),//在返回列表中包括指定的窗口（来自relativeToWindow参数）。 您必须将此选项与kCGWindowListOptionOnScreenAboveWindow或kCGWindowListOptionOnScreenBelowWindow选项结合使用以检索有意义的结果
   kCGWindowListExcludeDesktopElements    = (1 << 4)//从列表中排除所有属于桌面元素的窗口，包括背景图片和桌面图标。 您可以将此选项与其他选项结合使用
}
CFArrayRef windowListAll = CGWindowListCopyWindowInfo(kCGWindowListOptionAll| kCGWindowListExcludeDesktopElements, kCGNullWindowID);
NSArray* arr = CFBridgingRelease(windowListAll);


let options = CGWindowListOption(arrayLiteral: .excludeDesktopElements, .optionOnScreenOnly)

let windowsListInfo = CGWindowListCopyWindowInfo(options, CGWindowID(0))

let infoList = windowsListInfo as! [[String:Any]]

let visibleWindows = infoList.filter{ $0["kCGWindowLayer"] as! Int == 0 && $0["kCGWindowOwnerName"] as? String == "微信" }

```
