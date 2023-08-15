
```
#import <Cocoa/Cocoa.h>
 
NS_ASSUME_NONNULL_BEGIN
 
@interface myWindow : NSPanel
 
@end
 
NS_ASSUME_NONNULL_END



#import "myWindow.h"
 
@implementation myWindow
 
- (instancetype)initWithContentRect:(NSRect)contentRect
                          styleMask:(NSWindowStyleMask)aStyle
                            backing:(NSBackingStoreType)bufferingType
                              defer:(BOOL)flag{
    self = [super initWithContentRect:contentRect
                            styleMask:aStyle
                              backing:bufferingType
                                defer:flag];
    if (self) {
        
        [self setHasShadow:YES];
        
        [self setOpaque:NO];
        
        [self setBackgroundColor:[NSColor clearColor]];
        
        [self setMovableByWindowBackground:NO];
        
//注意
        [self setLevel:NSScreenSaverWindowLevel];
        
        self.styleMask = NSWindowStyleMaskBorderless | NSWindowStyleMaskNonactivatingPanel;
        
        self.animationBehavior = (CGWindowLevelForKey(NSMainMenuWindowLevel));
        
//能加入所以活动空间以及在全屏应用上边显示
        self.collectionBehavior = NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorFullScreenAuxiliary;
        
        if (@available(macOS 10.14, *)) {
            self.appearance  = [NSAppearance appearanceNamed:NSAppearanceNameVibrantLight];
        } else {
            // Fallback on earlier versions
        }
        
        [self invalidateShadow];
    }
    return self;
}
 
- (void)setContentView:(__kindof NSView *)contentView{
    
    contentView.wantsLayer            = YES;
    contentView.layer.frame           = contentView.frame;
    contentView.layer.cornerRadius    = 5.0;
    contentView.layer.masksToBounds   = YES;
    
    [super setContentView:contentView];
}
 
- (BOOL)canBecomeKeyWindow{
    return YES;
}
 
- (BOOL)canBecomeMainWindow{
    return YES;
}
 
@end

```