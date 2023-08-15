
```
[self setLevel:NSScreenSaverWindowLevel];
        
self.animationBehavior = NSWindowAnimationBehaviorNone;
        
self.collectionBehavior = NSWindowCollectionBehaviorMoveToActiveSpace;


```



```
//workspace改变时通知
[[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self selector:@selector(changeActiveSpace:) name:NSWorkspaceActiveSpaceDidChangeNotification object:nil];
 
- (void)changeActiveSpace:(NSNotification *)sender
{
    [localControl.window close];
    [localControl.window makeKeyAndOrderFront:nil];
}
```