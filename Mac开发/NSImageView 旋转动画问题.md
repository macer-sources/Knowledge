[参考](https://stackoverflow.com/questions/31985344/nsimageview-is-hoping-when-from-the-location-when-trying-to-rotate-how-to-stop)
[Code](https://github.com/bansalvks/Mac-Dummies)

```

-(void)buttonPressed:(id)sender{

    // setting the anchor point of the view
    _img.layer.anchorPoint = CGPointMake(0.5f, 0.5f);
    
    //Position of the imgView
    CGRect frame = _img.layer.frame;
    
    float xCoord = frame.origin.x + frame.size.width;
    float yCoord = frame.origin.y + frame.size.height;
    
    CGPoint myPoint = CGPointMake(xCoord, yCoord);
    _img.layer.position = myPoint;
    
    // calling the animation function
    [self startRefreshAnimation];
}

// in order to repeate the animation
- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag
{
    [self startRefreshAnimation];
}

// the function that will handel all the animation stuff
-(void)startRefreshAnimation {
    
    CABasicAnimation *anim2 = [CABasicAnimation animationWithKeyPath:@"transform.rotation"];
    anim2.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
    
    NSNumber* toValue = [NSNumber numberWithFloat:0 * (M_PI / 180.0f)];
    NSNumber* fromValue = [NSNumber numberWithFloat:(360.0f) * (M_PI / 180.0f)];
    
    
    anim2.fromValue = toValue;
    anim2.toValue = fromValue;
    
    
    anim2.duration = 1.0f;
    anim2.delegate = self;
    [_img.layer addAnimation:anim2 forKey:@"transform"];
}



```



动画： 
```
   let rotateAnimation = CABasicAnimation(keyPath: "transform.rotation")
    rotateAnimation.toValue = CGFloat(-M_PI * 2.0)
    rotateAnimation.duration = 1.0

    viewToRotate.layer?.anchorPoint = CGPoint(x: 0.5, y: 0.5)
    viewToRotate.layer?.position = viewToRotate.center

    viewToRotate.layer?.addAnimation(rotateAnimation, forKey: nil)



extension NSView {
var center: CGPoint {
    get {
        return CGPointMake(NSMidX(self.frame), NSMidY(self.frame))
    }
}
```


