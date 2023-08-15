
# NSImage 相关 

## 1. NSImage 圆角绘制 
```c++

/* NSImage+RoundCorner.h */
#import <Foundation/Foundation.h>
/* C Prototypes */
void addRoundedRectToPath(CGContextRef context, CGRect rect, float ovalWidth, float ovalHeight);

@interface NSImage(RoundCorner)
- (NSImage *)roundCornersImageCornerRadius:(NSInteger)radius;
@end


/* NSImage+RoundCorner.m */
#import "NSImage+RoundCorner.h"

@implementation NSImage (RoundCorner)

void addRoundedRectToPath(CGContextRef context, CGRect rect, float ovalWidth, float ovalHeight)
{
    float fw, fh;
    if (ovalWidth == 0 || ovalHeight == 0) {
        CGContextAddRect(context, rect);
        return;
    }
    CGContextSaveGState(context);
    CGContextTranslateCTM (context, CGRectGetMinX(rect), CGRectGetMinY(rect));
    CGContextScaleCTM (context, ovalWidth, ovalHeight);
    fw = CGRectGetWidth (rect) / ovalWidth;
    fh = CGRectGetHeight (rect) / ovalHeight;
    CGContextMoveToPoint(context, fw, fh/2);
    CGContextAddArcToPoint(context, fw, fh, fw/2, fh, 1);
    CGContextAddArcToPoint(context, 0, fh, 0, fh/2, 1);
    CGContextAddArcToPoint(context, 0, 0, fw/2, 0, 1);
    CGContextAddArcToPoint(context, fw, 0, fw, fh/2, 1);
    CGContextClosePath(context);
    CGContextRestoreGState(context);
}

- (NSImage *)roundCornersImageCornerRadius:(NSInteger)radius {
    int w = (int) self.size.width;
    int h = (int) self.size.height;

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(NULL, w, h, 8, 4 * w, colorSpace, kCGImageAlphaPremultipliedFirst);

    CGContextBeginPath(context);
    CGRect rect = CGRectMake(0, 0, w, h);
    addRoundedRectToPath(context, rect, radius, radius);
    CGContextClosePath(context);
    CGContextClip(context);

    CGImageRef cgImage = [[NSBitmapImageRep imageRepWithData:[self TIFFRepresentation]] CGImage];

    CGContextDrawImage(context, CGRectMake(0, 0, w, h), cgImage);

    CGImageRef imageMasked = CGBitmapContextCreateImage(context);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);

    NSImage *tmpImage = [[NSImage alloc] initWithCGImage:imageMasked size:self.size];
    NSData *imageData = [tmpImage TIFFRepresentation];
    NSImage *image = [[NSImage alloc] initWithData:imageData];
    [tmpImage release];

    return [image autorelease];
}
@end


```


## 2. 获取NSImage图片的准确大小 
> size 和图片的DPI有关，如果图片的DPI不是72，NSImage加载图片后size属性就会出错

```c++
NSImage *image = [NSImage imageNamed:@"image"];
NSBitmapImageRep *rep = [NSBitmapImageRep imageRepWithData:[image TIFFRepresentation]];
NSSize size = NSMakeSize([rep pixelsWide], [rep pixelsHigh]);
[image setSize: size];
```

## 3. CGImage转NSImage 
```c++

/* CGImage to NSImage */
NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:cgImage];
NSImage *image = [[NSImage alloc] init];
[image addRepresentation:bitmapRep];

```



## 3. 拍照  
```c++

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // 初始化一个pictureTaker对象
    pictureTaker = [IKPictureTaker pictureTaker];
    /* 下面两行代码对pictureTaker进行一些设置，比如增加拍照效果的支持，以及设置拍摄的照片的最大尺寸 */
    //[pictureTaker setValue:[NSNumber numberWithBool:YES] forKey:IKPictureTakerShowEffectsKey];
    //[pictureTaker setValue:[NSValue valueWithSize:NSMakeSize(800.0, 600.0)] forKey:IKPictureTakerOutputImageMaxSizeKey];
}
// Picture taker的回调方法。
- (void) pictureTakerDidEnd:(IKPictureTaker *)picker
                 returnCode:(NSInteger)code
                contextInfo:(void*)contextInfo
{
    // 获取图片，传递给Image Well。
    NSImage *image = [picker outputImage];
    [imageView setImage:image];
}
// 这里，用主窗口sheet的方式打开拍照窗口。
- (IBAction)takePicture:(id)sender {
    [pictureTaker beginPictureTakerSheetForWindow:[self window] withDelegate:self didEndSelector:@selector(pictureTakerDidEnd:returnCode:contextInfo:) contextInfo:nil];
}


```