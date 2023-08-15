
# Dock 相关


## 1. 跳动Dock图标的实现
```c++

//跳动一次
- (void)bounceOnce:(id)sender {
    [NSApp requestUserAttention:NSInformationalRequest];
}
//总是跳动，直到用户干预为止
- (void)bounceAlways:(id)sender {
    [NSApp requestUserAttention:NSCriticalRequest];
}

```


## 2. 玩转Dock图标 

Dock图标的自定义: 
* 加徽章（Badge）
* 换图标
* 隐藏和显示最小化时的图标徽章
* 增加自定义Dock菜单

### 1. Dock徽章(角标) 
```c++

// Add badge
- (IBAction)addBadge:(id)sender {
    [[NSApp dockTile] setBadgeLabel:[NSString stringWithFormat:@"%d", random() % 11]];
}

// Remove badge
- (IBAction)removeBadge:(id)sender  {
    [[NSApp dockTile] setBadgeLabel:nil];
}

```

### 2. 更换Dock图标  
1. 方式一： 直接指定一个NSImage对象设置应用程序的图标 
2. 方式二： 用一个自定义的View，来显示为Dock图标
    __用自定义View做图标不能自动刷新， 需要手动通过-display方法刷新。__ 
```c++ 

// 方式一： Customize Dock Icon
- (IBAction)changeDockIcon:(id)sender {
    [NSApp setApplicationIconImage:[NSImage imageNamed:@"Icon"]];
}

// Restore Dock Icon
- (IBAction)restoreDockIcon:(id)sender {
    [NSApp setApplicationIconImage:nil];
}

// 方式二： Draw Dock Icon with Custom View
- (IBAction)changeDockIconWithCustomView:(id)sender {
    DockIconView *dockView = [[[DockIconView alloc] init] autorelease];
    [[NSApp dockTile] setContentView: dockView];
    [[NSApp dockTile] display];
}

```

> 上面介绍的两种方法修改的程序图标会在程序退出之后还原为在Info.plist里指定的应用程序图标。要永久的改变程序图标（也就是退出程序的时候也能显示修改后的图标）的方法是 __创建Dock图标的插件__  



### 3. 隐藏和显示最小化时的图标徽章 
```c++ 
// Show minimize icon badge
- (IBAction)showMinimizeBadge:(id)sender {
    [[self.window dockTile] setShowsApplicationBadge:YES];
}

// Remove minimize icon badge
- (IBAction)removeMinimizeBadge:(id)sender {
    [[self.window dockTile] setShowsApplicationBadge:NO];
}


```


### 4. Dock菜单 
* 方式一： 直接在XIB中静态的设计菜单  
* 方式二： -applicationDockMenu:方法，在程序中动态指定菜单

```c++

- (NSMenu *)applicationDockMenu:(NSApplication *)sender {
    return self.dynamicMenu;
}

```



## 3. 隐藏 Dock 图标 
* 方式一： 在Application Bundle里的Info.plist中增加一个LSUIElement的键，设置值为YES 
* 方式二： 动态设置是否隐藏 
```c++ 

ProcessSerialNumber psn = { 0, kCurrentProcess };
TransformProcessType(&psn, kProcessTransformToForegroundApplication);

```

> 在设置隐藏Dock图标的时候需要重启应用程序，无法在运行时动态完成的，而在显示Dock图标的时候则不需要