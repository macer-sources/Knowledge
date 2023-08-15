
```
- (NSMutableArray *)getscreenCapture
{
    NSMutableArray *datas = [NSMutableArray array];
    //存储所有显示器显示id
    CGDirectDisplayID dspyIDArray[10];
    uint32_t dspyIDCount = 0;
    //获取当前活跃的所有显示器id及其个数
    if (CGGetActiveDisplayList(10, dspyIDArray, &dspyIDCount) != kCGErrorSuccess)
        return nil;
    CFStringRef dspyDestType = CFSTR("public.png");
    for(uint32_t i = 0; i < dspyIDCount; i++) {
        CGDirectDisplayID mainID = dspyIDArray[i];
        // 根据Quartz分配给显示器的id，生成显示器mainID的截图
        CGImageRef mainCGImage = CGDisplayCreateImage(mainID);
        CFMutableDataRef mainMutData = CFDataCreateMutable(NULL, 0);
        CGImageDestinationRef mainDest = CGImageDestinationCreateWithData(mainMutData, dspyDestType, 1, NULL);
        CGImageDestinationAddImage(mainDest, mainCGImage, NULL);
        CGImageRelease(mainCGImage);
        CGImageDestinationFinalize(mainDest);
        CFRelease(mainDest);
        [datas addObject:(__bridge NSData *)mainMutData];
        CFRelease(mainMutData);
    }
    CFRelease(dspyDestType);
    NSInteger imageCount = [datas count];
    if (imageCount <= 0) {
        return nil;
    }
    return datas;
}



```