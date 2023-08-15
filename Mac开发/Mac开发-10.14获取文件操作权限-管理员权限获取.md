
### 场景
MacOS的更新功能，需要替换 Application下的应用程序（先删除再移动新app到 Application目录）。在Mac os 10.14以上版本中，需要获得管理员权限

## 使用NSAppleScript来操作 
使用 NSTask 不能获取相关权限，所以使用NSAppleScript来进行权限获取的同时，执行脚本

```

Boolean runProcessAsAdministrator( NSString *scriptPath, NSArray *arguments,BOOL isAdmin, NSString **output,NSString **errorDescription)
{
    NSString * allArgs = [arguments componentsJoinedByString:@" "];
    NSString *isAdminPre = @"";
    if (isAdmin) {
        isAdminPre = @"with administrator privileges";
    }
    NSString * fullScript = [NSString stringWithFormat:@"%@ %@", scriptPath, allArgs];
    NSDictionary *errorInfo = [NSDictionary new];
    NSString *script = [NSString stringWithFormat:@"do shell script \"%@\" %@", fullScript,isAdminPre];
    NSLog(@"script = %@",script);
    NSAppleScript *appleScript = [[NSAppleScript new] initWithSource:script];
    NSAppleEventDescriptor * eventResult = [appleScript executeAndReturnError:&errorInfo];
    // Check errorInfo/var/tmp
    if (! eventResult)
    {
        // Describe common errors
        *errorDescription = nil;
        if ([errorInfo valueForKey:NSAppleScriptErrorNumber])
        {
            NSNumber * errorNumber = (NSNumber *)[errorInfo valueForKey:NSAppleScriptErrorNumber];
            if ([errorNumber intValue] == -128)
                *errorDescription = @"The administrator password is required to do this.";
        }
        // Set error message from provided message
        if (*errorDescription == nil)
        {
            if ([errorInfo valueForKey:NSAppleScriptErrorMessage])
                *errorDescription = (NSString *)[errorInfo valueForKey:NSAppleScriptErrorMessage];
        }
        return NO;
    }
    else
    {
        // Set output to the AppleScript's output
        *output = [eventResult stringValue];
        return YES;
    }
}



```
runProcessAsAdministrator(脚本路径,参数数组,是否管理员权限运行,输出结果,错误描述)

* 使用方法：
```
    NSString *output = nil;
    NSString *errorDescription = nil;
    runProcessAsAdministrator(@"",@[@"/usr/bin/unzip",@"-u", @"-d",
                                    unzipFolder, zipPath],YES,&output,&errorDescription);




```
值得注意的是需要自己判断是否没有权限，没有权限才传入YES，以管理员身份运行，并会要求用户输入用户密码。


## 使用Security框架来获取权限 
Security.framework包含权限获取的功能，通过使用AuthorizationExecuteWithPrivileges函数来获取所需要的权限。  

```
char * const GSFileOpRemoveCommand = "rm";
char * const GSFileOpMoveCommand = "mv";
//这里进行预授权，如果能获取权限，则进行下一步
- (BOOL)_acquireAuthorizationWithError:(NSError *__autoreleasing *)error {
    NSLog(@"AuthHelperTool executing self-repair");
    OSStatus myStatus;
    
    myStatus = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &_auth);
    if (myStatus != errAuthorizationSuccess){
        _auth = NULL;
        NSLog(@"AuthHelperTool AuthorizationCreate failed");
        return NO;
    }
    AuthorizationItem rightItems[] = {
        // The right that allows us to run tools as root user
        {.name = kAuthorizationRightExecute, .valueLength = 0, .value = NULL, .flags = 0}
    };
    AuthorizationRights rights = {
        .count = sizeof(rightItems) / sizeof(*rightItems),
        .items = rightItems
    };
    
    AuthorizationFlags flags =
    (AuthorizationFlags)(kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed| kAuthorizationFlagExtendRights | kAuthorizationFlagPreAuthorize);
    
    myStatus = AuthorizationCopyRights (_auth, &rights, kAuthorizationEmptyEnvironment, flags, NULL);
    if (myStatus != errAuthorizationSuccess) {
        AuthorizationFree(_auth, kAuthorizationFlagDefaults);
        _auth = NULL;
        NSLog(@"AuthHelperTool AuthorizationCopyRights failed");
        return NO;
    }
    return YES;
}
//这里进行命令操作，同时进行授权
- (BOOL)_authorizeAndExecuteCommand:(char *)command sourcePath:(char *)sourcePath destinationPath:(char *)destinationPath error:(NSError * __autoreleasing *)error
{
    NSError *acquireError = nil;
    if (![self _acquireAuthorizationWithError:&acquireError]) {
        if (error != NULL) {
            *error = acquireError;
        }
        return NO;
    }
    
    char *arguments[] = { command, sourcePath, destinationPath, NULL };
    
    char toolPath[PATH_MAX] = {0};
    //这里由于是更新程序的授权，所以取的是系统缓存目录中的Unix可执行文件
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *genseePath = [[paths objectAtIndex:0]stringByAppendingPathComponent:@"gensee"];
    NSString* updaterFolder = [genseePath stringByAppendingPathComponent:@"update"];
    [[NSFileManager defaultManager]createDirectoryAtPath:updaterFolder withIntermediateDirectories:YES attributes:NULL error:NULL];
    NSString* updaterPath =[updaterFolder stringByAppendingPathComponent:@"Updater.app"];
    NSString* bundleName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
    NSString* fileOperationToolPath =[NSString stringWithFormat:@"%@/Contents/MacOS/%@",updaterPath,bundleName];
    //这里取到需要获取权限的Unix执行文件程序路径
    NSLog(@"fileOperationToolPath (%@) ",fileOperationToolPath);
    if (![fileOperationToolPath getFileSystemRepresentation:toolPath maxLength:sizeof(toolPath)]) {
        NSLog(@"File to author folder permission (%@) cannot be represented as a valid file name.",fileOperationToolPath);
        return NO;
    }
    FILE *pipe = NULL;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
//这里值得注意的就是第二个参数是可执行文件路径，就是需要获取权限的程序路径,例如Test.app,则是Test.app/Content/MacOS/Test执行文件
    OSStatus runStatus = AuthorizationExecuteWithPrivileges(_auth, toolPath, kAuthorizationFlagDefaults, arguments, &pipe);
    if (runStatus != errAuthorizationSuccess) {
        NSLog(@"AuthorizationExecuteWithPrivileges not success");
#pragma clang diagnostic pop
        return NO;
    }
    return YES;
}

```

正常使用中我们需要先使用NSFileManager进行删除和移动操作，当获取的错误码为
NSFileReadNoPermissionError和NSFileWriteNoPermissionError时，就进行权限获取逻辑 

#define NS_HAS_PERMISSION_ERROR(error) (error.code == NSFileReadNoPermissionError || error.code == NSFileWriteNoPermissionError)

#### 示例
```
NSError* error;
if ([[NSFileManager defaultManager]fileExistsAtPath:applicationPath]){
    BOOL result = [[NSFileManager defaultManager]removeItemAtPath:applicationPath error:&error];
    if (!result) {
        NSLog(@"[Appdelegate] removeItemAtPath : %@ error : %@",applicationPath,error.description);
        if(NS_HAS_PERMISSION_ERROR(error)){
            char rmPath[PATH_MAX] = {0};
            if (![applicationPath getFileSystemRepresentation:rmPath maxLength:sizeof(rmPath)]) {
                NSLog(@"File to remove (%@) cannot be represented as a valid file name.", applicationPath.lastPathComponent);
                return;
            }
            // rm
            NSError *executeError = nil;
            BOOL success = [sself _authorizeAndExecuteCommand:GSFileOpRemoveCommand sourcePath:rmPath destinationPath:NULL error:&executeError];
            if (!success) {
                NSLog(@"File to remove (%@) _authorizeAndExecuteCommand", applicationPath.lastPathComponent);
                //                return;
            }
        }
        
    }
}

BOOL result = [[NSFileManager defaultManager] moveItemAtPath:unzipPath toPath:applicationPath error:&error];
if (!result) {
    NSLog(@"[Appdelegate] moveItemAtPath : %@ to %@ error : %@",unzipPath,applicationPath,error.description);
    if (NS_HAS_PERMISSION_ERROR(error)) {
        //do new version handler
        if ([[NSFileManager defaultManager]fileExistsAtPath:applicationPath]){
            
        }
        //mv
        char sourcePath[PATH_MAX] = {0};
        if (![unzipPath getFileSystemRepresentation:sourcePath maxLength:sizeof(sourcePath)]) {
            NSLog(@"File to move (%@) cannot be represented as a valid file name.", applicationPath.lastPathComponent);
            return;
        }
        char destinationPath[PATH_MAX] = {0};
        if (![applicationPath getFileSystemRepresentation:destinationPath maxLength:sizeof(destinationPath)]) {
            NSLog(@"File to remove (%@) cannot be represented as a valid file name.", applicationPath.stringByDeletingLastPathComponent);
            return;
        }
        
        NSError *executeError = nil;
        BOOL success = [sself _authorizeAndExecuteCommand:GSFileOpMoveCommand sourcePath:sourcePath destinationPath:destinationPath error:&executeError];
        if (!success) {
            NSLog(@"File to move (%@) sourcPath %s destinationPath %s", applicationPath.lastPathComponent,sourcePath,destinationPath);
            //            return;
        }
    }
}



```



## ServiceManagement注册LaunchdDaemon. (更加先进)。
大概就是将你的app需要root权限的操作独立到另一个helper tool中，通过给与这个helper tool高级权限，来完成你的操作。这样你的app也不会在高权限下运行，且权限获取只需要一次。







[参考](https://blog.csdn.net/shengpeng3344/article/details/103497508?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-103497508-blog-71374287.pc_relevant_3mothn_strategy_and_data_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-103497508-blog-71374287.pc_relevant_3mothn_strategy_and_data_recovery&utm_relevant_index=2)  










