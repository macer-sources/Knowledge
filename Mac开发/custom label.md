 
自定义 label 

```
//
//  NNLabel.h
//  MacTemplet
//
//  Created by Bin Shang on 2020/3/26.
//  Copyright © 2020 Bin Shang. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSContentVerticalAlignment) {
    NSContentVerticalAlignmentTop      = 0,
    NSContentVerticalAlignmentCenter    = 1,
    NSContentVerticalAlignmentBottom     = 2,
};

@interface NNLabel : NSView

@property(nullable, nonatomic,copy)   NSString           *text; // default is nil
@property(nonatomic, strong) NSFont      *font; // default is nil (system font 17 plain)
@property(nonatomic, strong) NSColor     *textColor; // default is labelColor
@property(nonatomic, assign) NSTextAlignment    textAlignment;   // default is NSTextAlignmentLeft
/// 整体内容垂直对齐方向
@property(nonatomic, assign) NSContentVerticalAlignment    contentVerticalAlignment;   // default is NSContentVerticalAlignmentTop

@property(nonatomic, assign)        NSLineBreakMode    lineBreakMode;   // default is NSLineBreakByWordWrapping. used for single and multiple lines of text

// the underlying attributed string drawn by the label, if set, the label ignores the properties above.
@property(nullable, nonatomic,copy)   NSAttributedString *attributedText;  // default is nil

// the 'highlight' property is used by subclasses for such things as pressed states. it's useful to make it part of the base class as a user property

@property(nullable, nonatomic,strong)       NSColor *highlightedTextColor; // default is nil
@property(nonatomic,getter=isHighlighted)   BOOL     highlighted;          // default is NO

@property(nonatomic,getter=isUserInteractionEnabled) BOOL userInteractionEnabled;  // default is NO
@property(nonatomic,getter=isEnabled)                BOOL enabled;                 // default is YES. changes how the label is drawn
//@property(nonatomic) NSInteger numberOfLines;

//- (CGRect)textRectForBounds:(CGRect)bounds limitedToNumberOfLines:(NSInteger)numberOfLines;
//- (void)drawTextInRect:(CGRect)rect;

@property(nonatomic, copy) void(^mouseDownBlock)(NNLabel *sender);
///返回事件
- (void)actionBlock:(void(^)(NNLabel *sender))block;

@end

NS_ASSUME_NONNULL_END


```


```
//
//  NNLabel.m
//  MacTemplet
//
//  Created by Bin Shang on 2020/3/26.
//  Copyright © 2020 Bin Shang. All rights reserved.
//

#import "NNLabel.h"

@implementation NNLabel

- (BOOL)isFlipped{
    return true;
}

- (instancetype)initWithFrame:(NSRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
        self.enabled = true;
        self.userInteractionEnabled = false;
        
        self.wantsLayer = true;
        self.font = [NSFont systemFontOfSize:13 weight:NSFontWeightLight];
        self.textColor = NSColor.labelColor;
        self.textAlignment = NSTextAlignmentLeft;
        self.lineBreakMode = NSLineBreakByWordWrapping;
        self.contentVerticalAlignment = NSContentVerticalAlignmentTop;
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
    CGFloat padding = 8.0;
    if (self.enabled == false) {
        if (!self.text) {
            return;
        }
        NSColor *textColor = NSColor.lightGrayColor;
        [self drawString:self.text textColor:textColor padding:padding];
        return;
    }
    
    if (self.attributedText) {
        [self drawAttributedString:self.attributedText padding:padding];

    } else {
        if (!self.text) {
            return;
        }
        NSColor *textColor = self.isHighlighted ? self.highlightedTextColor : self.textColor;
        [self drawString:self.text textColor:textColor padding:padding];

//        NSMutableParagraphStyle *paraStyle = [[NSMutableParagraphStyle defaultParagraphStyle]mutableCopy];
//        paraStyle.alignment = self.textAlignment;
//
//        NSDictionary *attrDic = @{NSParagraphStyleAttributeName: paraStyle,
//                                  NSForegroundColorAttributeName: textColor,
//                                  NSFontAttributeName: self.font,
//                                }.mutableCopy;
//        
//        NSAttributedString *attString = [[NSAttributedString alloc]initWithString:self.text attributes:attrDic];
//        [self drawAttributedString:attString padding:padding];
    }
}

- (void)mouseDown:(NSEvent *)event{
    if (self.mouseDownBlock) {
        self.mouseDownBlock(self);
    }
}

- (void)actionBlock:(void(^)(NNLabel *sender))block{
    if (self.isUserInteractionEnabled == false) {
        return;
    }
    self.mouseDownBlock = block;
}

#pragma mark -funtions
- (void)drawAttributedString:(NSAttributedString *)attributedString padding:(CGFloat)padding{
    CGSize maxSize = CGSizeMake(self.bounds.size.width - padding*2, CGFLOAT_MAX);
    CGSize size = [attributedString boundingRectWithSize:maxSize
                                                 options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading
                                                 context:nil].size;

    CGFloat gapX = padding + (maxSize.width - size.width)/2;
    CGFloat gapY = (CGRectGetHeight(self.bounds) - size.height)/2;

    CGRect contentRect = NSMakeRect(floorf(gapX), floorf(gapY), size.width, size.height);
//    NSLog(@"contentRect_%@", @(contentRect));
//    [attributedString drawInRect:contentRect];

    switch (self.contentVerticalAlignment) {
        case NSContentVerticalAlignmentCenter:
            break;
        case NSContentVerticalAlignmentBottom:
            gapY *= 2;
            break;
        default:
            gapY = 0;
            break;
    }
    if (self.textAlignment == NSTextAlignmentLeft) {
        gapX = size.width < maxSize.width ? 0 : gapX;
    } else if (self.textAlignment == NSTextAlignmentRight) {
        gapX = size.width < maxSize.width ? gapX*2 : 0;
    }
    contentRect = NSMakeRect(floorf(gapX), floorf(gapY), size.width, size.height);
//    NSLog(@"contentRect_%@", @(contentRect));
    [attributedString drawInRect:contentRect];
}

- (void)drawString:(NSString *)string textColor:(NSColor *)textColor padding:(CGFloat)padding{
    NSMutableParagraphStyle *paraStyle = [NSMutableParagraphStyle.defaultParagraphStyle mutableCopy];
    paraStyle.alignment = self.textAlignment;
    paraStyle.lineBreakMode = self.lineBreakMode;

    NSDictionary *attrDic = @{NSParagraphStyleAttributeName: paraStyle,
                              NSForegroundColorAttributeName: textColor,
                              NSFontAttributeName: self.font,
                            };
    
    NSAttributedString *attString = [[NSAttributedString alloc]initWithString:string attributes:attrDic];
    [self drawAttributedString:attString padding:padding];
    
}


@end


```