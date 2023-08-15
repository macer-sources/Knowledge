```
- (BOOL)applicationShouldHandleReopen:(NSApplication *)theApplication 
                    hasVisibleWindows:(BOOL)flag{ 
  if (!flag){ 
    [NSApp activateIgnoringOtherApps:NO]; 
    [self.window makeKeyAndOrderFront:self]; 
  } 
  return YES; 
} 


```