
# Menu  





## 1. 自定义实现 NSPopupButton 效果 

```c++

- (IBAction)showMenu:(id)sender {

    NSRect frame = [(NSButton *)sender frame];
    NSPoint menuOrigin = [[(NSButton *)sender superview] convertPoint:NSMakePoint(frame.origin.x, frame.origin.y) toView:nil];

    NSEvent *event =  [NSEvent mouseEventWithType:NSLeftMouseDown
                                         location:menuOrigin
                                    modifierFlags:NSLeftMouseDownMask
                                        timestamp:0
                                     windowNumber:[[(NSButton *)sender window] windowNumber]
                                          context:[[(NSButton *)sender window] graphicsContext]
                                      eventNumber:0
                                       clickCount:1
                                         pressure:1];

    // contextMenu可以直接在XIB里初始化一个菜单
    [NSMenu popUpContextMenu:contextMenu withEvent:event forView:(NSButton *)sender];
}

```