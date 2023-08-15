// setIndentationPerLevel 



删除显示三角形：在您的子类中实现该frameOfOutlineCellAtRow:方法NSOutlineView并返回NSZeroRect（当然，仅当您想隐藏该特定行的三角形时。）

禁用缩进：大纲视图的标准布局在最左边保留空间来绘制三角形，以防项目可扩展。但是您可以通过指定不同的图框来覆盖单个项目。您还可以通过响应以下消息在您的子类中执行此操作：


```SWIFT
- (NSRect)frameOfCellAtColumn:(NSInteger)column row:(NSInteger)row {
    NSRect superFrame = [super frameOfCellAtColumn:column row:row];


    if ((column == 0) /* && isGroupRow */) {
        return NSMakeRect(0, superFrame.origin.y, [self bounds].size.width, superFrame.size.height);
    }
    return superFrame;
}

```

// 可以设置 indentationMarkerFollowsCell = false 来影响缩紧





## 更改 三角形图片 

```
- (void)outlineView:(NSOutlineView *)outlineView willDisplayOutlineCell:(id)cell forTableColumn:(NSTableColumn *)tableColumn item:(id)item {
    NSString *theImageName;
    NSInteger theCellValue = [cell integerValue];
    if (theCellValue==1) {
        theImageName = @"PMOutlineCellOn";
    } else if (theCellValue==0) {
        theImageName = @"PMOutlineCellOff";
    } else {
        theImageName = @"PMOutlineCellMixed";
    }

    NSImage *theImage = [NSImage imageNamed: theImageName];
    NSRect theFrame = [outlineView frameOfOutlineCellAtRow:[outlineView rowForItem: item]];
    theFrame.origin.y = theFrame.origin.y +17;
    // adjust theFrame here to position your image
    [theImage compositeToPoint: theFrame.origin operation:NSCompositeSourceOver];
    [cell setImagePosition: NSNoImage];
}

```


#### 备份，可能用到的代码 

```
- (id)makeViewWithIdentifier:(NSString *)identifier owner:(id)owner
{
    id view = [super makeViewWithIdentifier:identifier owner:owner];

    if ([identifier isEqualToString:NSOutlineViewDisclosureButtonKey])
    {
        // Do your customization
        // return disclosure button view

        [view setImage:[NSImage imageNamed:@"Disclosure_Categories_Plus"]];
        [view setAlternateImage:[NSImage imageNamed:@"Disclosure_Categories_Minus"]];
        [view setBordered:NO];
        [view setTitle:@""];

        return view;
    }

    return view;
}

//Frame of the disclosure view
- (NSRect)frameOfOutlineCellAtRow:(NSInteger)row
{
    NSRect frame = NSMakeRect(4, (row * 22), 19, 19);
    return frame;
}


```



https://stackoverflow.com/questions/11127764/how-to-customize-disclosure-cell-in-view-based-nsoutlineview




如何一进入就展开？
如果设置缩进？
如何设置递进的图标？



How do I unfold as soon as I enter?
How do you set indentation?
How do I set the progressive icon?
