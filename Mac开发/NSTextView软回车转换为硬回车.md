
## 方式一 
layoutManager:shouldBreakLineByWordBeforeCharacterAtIndex 

## 方法二

`- (void)enumerateLineFragmentsForGlyphRange:(NSRange)glyphRange usingBlock:(void (^)(NSRect rect, NSRect usedRect, NSTextContainer *textContainer, NSRange glyphRange, BOOL *stop))block NS_AVAILABLE(10_11, 7_0);`  
```
rect 预留给该行使用的大小
usedRect 实际该行使用的大小
textContainer 自己的textContainer实例
glyphRange 文本Range,由Postion和length组成。表示该行开始位置，以及长度

```

```
__weak typeof(self) wself = self;
    [_layoutManagerB enumerateLineFragmentsForGlyphRange:NSMakeRange(0, testString.length) usingBlock:^(NSRect rect, NSRect usedRect, NSTextContainer * _Nonnull textContainer, NSRange glyphRange, BOOL * _Nonnull stop) {
        __strong typeof(self) sself = wself;
        NSInteger index = glyphRange.location + glyphRange.length;
        [sself.breaklineIndexs addObject:@(index)];
        if (sself.layoutManagerB.numberOfGlyphs == index) { //因为最后一个字符后一定会插入\n，当字符数量==index时,即stop
            NSString *result = [self getBreaklineString];
            NSLog(@"添加硬回车后的字符串为 %@",result);
            *stop = YES;
        }
    }];

```