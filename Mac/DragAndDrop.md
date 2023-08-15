# 拖拽功能实现 



## 简介
拖拽功能主要使用到了 `NSDraggingSource` 和 `NSDraggingDestination` 以及 `NSPasteboardItemDataProvider` 

`source ------> pasteboard ------> destination`  



## 源部分实现(NSDraggingSource)
1. 拖出部需要实现 `NSDraggingSource` 协议 


```objective-c
- (BOOL)acceptsFirstMouse:(NSEvent *)event 
{
    return YES;
}

- (NSDragOperation)draggingSession:(NSDraggingSession *)session sourceOperationMaskForDraggingContext:(NSDraggingContext)context
{
    switch (context) {
        case NSDraggingContextOutsideApplication:
            return NSDragOperationNone;     // 拖动到applicaiton外部时，针对自定义类型不支持此操作
            
        case NSDraggingContextWithinApplication:
        default:
            return NSDragOperationCopy;
            break;
    }
}

```

### 提供数据
1. 直接在拖拽开始时将数据放入到 pasteboard

2. 实现 `NSPasteboardItemDataProvider`以提供数据

```objective-c

- (void)pasteboard:(nullable NSPasteboard *)pasteboard item:(NSPasteboardItem *)item provideDataForType:(NSPasteboardType)type {
    // 根据 type 提供数据

    NSData *data = //;
    [item setData:data forType:type];
}

```


### 拖动画实现 
在拖拽时， 有一个拖的图标跟随鼠标移动。

```objective-c
- (void)mouseDown:(NSEvent*)event 
- (void)mouseDragged:(NSEvent *)event 
{

    NSPasteboardItem *pbItem = [NSPasteboardItem new]; 
    [pbItem setDataProvider:self forTypes:[NSArray arrayWithObjects:NSPasteboardTypeTIFF, NSPasteboardTypePDF, @"自定义类型", nil]];

    NSDraggingItem *dragItem = [[NSDraggingItem alloc] initWithPasteboardWriter:pbItem];


    // 设置frame 和拖拽图 
    // 方式1  
    [dragItem setDraggingFrame:self.bounds contents:[self image]];

    // 方式2
    dragItem.imageComponentsProvider = ^{
        NSDraggingImageComponent *component = [NSDraggingImageComponent  draggingImageCpmponentWithKey:NSDraggingImageComponentIconKey];
        component.frame = NSRectMake(0,0,16,16);
        component.contents = [NSImage imageWithSize:NSSizeMake(16,16) flipped:NO drawingHandler:^(NSRect rect){
            NSImage *image = ....
            [image drawInRect:rect];
            return YES;
        }];
        return @[component];
    };



    NSDraggingSession *draggingSession = [self beginDraggingSessionWithItems:[NSArray arrayWithObject:[dragItem autorelease]] event:event source:self];
    draggingSession.animatesToStartingPositionsOnCancelOrFail = YES;
    draggingSession.draggingFormation = NSDraggingFormationNone;
}

```



## 接收部分实现 
1. 注册自己能接受的数据类型 

```objective-c 
[self.window/view registerForDraggedTypes:@[

]];

```

2. 实现 `NSDraggingDestination` 协议 

```objective-c
// 拖放进入目标区
- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {
    // 判断是否可接收
}

//  拖放进入目标区移动(如果目标响应draggingEntered: 但不响应draggingUpdated: 使用draggingEntered: 的返回值)
- (NSDragOperation)draggingUpdated:(id <NSDraggingInfo>)sender {

}
// 拖放退出目标区 __拖放的图像会弹回到拖放源__  
- (void)draggingExited:(nullable id <NSDraggingInfo>)sender {

}
//  当你在视图内松开鼠标时，系统会调用上述方法：拒绝或接受的最后机。 拖放预处理，__一般是根据拖放type，判断是否接受拖放__
- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender; {

}
// 允许接受拖放，系统移除拖动图像，并开始接收处理拖放数据 
- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {

}
// 拖放完成结束
- (void)concludeDragOperation:(nullable id <NSDraggingInfo>)sender;
- (void)draggingEnded:(id<NSDraggingInfo>)sender;




```