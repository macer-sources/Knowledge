在OSX开发与系统密切相关的软件，经常会使用到高权限的操作，让整个程序都以高权限执行显然是不合适的，通过将高权限的任务封装到独立的子程序，按需要进行调用是比较合理的做法。而常见的做法是通过注册LaunchdDaemon，便可轻松实现开机启动、长驻后台、高权限的需求。  


注册LaunchdDaemon的常用方法是通过launchd工具去load一个与Daemon程序相关的标准的plist文件，这个过程一般放在PKG的安装脚本中完成，但当前越来越多的软件摒弃了PKG的打包方式，而是直接选择了打包成app，这样确实提升用户体验，但安装辅助工具的工作都放到了app运行过程中了。由于launchd是需要高权限运行的，而app中提权运行工具并不那么方便。所以便可以考虑通过ServiceManagement的API来完成这样的操作。


首先你需要在你的工程中创建一个用于LaunchdDaemon的命令行程序的Target，然后为主程序添加工程依赖，并Copy Files中将此子程序拷贝到Contents/Library/LaunchServices目录下。

然后为该子程序添加两个配置文件，一个是程序环境相关的Info.plist，它与普通app中的info.plist一样，添加上一些必须的字段如BundleID，额外需要添加一项SMAuthorizedClients，用于标识可加载或卸载它的主程序，形如下面的代码，指定了它的主程序BundleID为me.tanhao.SMJobDemo。

```
<key>SMAuthorizedClients</key>
  <array> 
    <string> me.tanhao.SMJobDemo</string>
  </array> 
  
```

而另一个是用于Launchd加载时的配置文件，但与以前通过launchd工具去加载的那个plist不同的是不需要添加ProgramArguments项，这些字段都会在注册成功后自动填充。

由于这是一个命令行工具，所以要将两个plist文件编译进Mach-O文件之中，就需要在工程配置中的“Other Linker Flags”中添加额外的参数，形式如下：

```
<key>SMPrivilegedExecutables</key>
  <dict>
    <key>SMJobHelper</key>
    <string>identifier me.tanhao.SMJobHelper</string>
  </dict>

```

然后我们就来完成主程序中注册LaunchdDaemon的代码: 
```
#define kSMJobHelperBunldeID @"SMJobHelper"
- (void)addHelper
{
    NSDictionary *helperInfo = (__bridge NSDictionary*)SMJobCopyDictionary(kSMDomainSystemLaunchd,
                                                                           (__bridge CFStringRef)kSMJobHelperBunldeID);
    if (!helperInfo)
    {
        AuthorizationItem authItem = { kSMRightBlessPrivilegedHelper, 0, NULL, 0 };
        AuthorizationRights authRights = { 1, &authItem };
        AuthorizationFlags flags = kAuthorizationFlagDefaults|
        kAuthorizationFlagInteractionAllowed|
        kAuthorizationFlagPreAuthorize|
        kAuthorizationFlagExtendRights;
        
        AuthorizationRef authRef = NULL;
        OSStatus status = AuthorizationCreate(&authRights, kAuthorizationEmptyEnvironment, flags, &authRef);
        if (status != errAuthorizationSuccess)
        {
            NSLog(@"Failed to create AuthorizationRef, return code %i", status);
        } else
        {
            CFErrorRef error = NULL;
            BOOL result = SMJobBless(kSMDomainSystemLaunchd, (__bridge CFStringRef)kSMJobHelperBunldeID, authRef, &error);
            if (!result)
            {
                NSLog(@"SMJobBless Failed, error : %@",error);
            }
        }
    }
}


```

编译并运行以上代码(必须签名)，首先会弹出一个系统的权限框，然后你输入管理员密码，你的LaunchdDaemon就注册成功了。你会发现子程序被拷贝到了/Library/PrivilegedHelperTools目录，而在/Library/LaunchDaemons也生成了一份与之对应的配置文件，用于launchd进程在合适的时机调用。
