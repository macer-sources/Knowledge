# Macos 提权 




## 方案一 
`AuthorizationExecuteWithPrivileges()`  
> `STPrivilegedTask` 已过期  


## 方案二 
__注册 LaunchdDaemon__  
> 注册LaunchdDaemon的常用方法是通过launchd工具加载一个与Daemon程序相关的标准的plist文件，由于launchd需要高权限运行，所以启动的子程序自然也是高权限运行  

一般 在`PKG` 打包模式下，进行使用，不过，目前大部分都打包成 `dmg` 的方式 

[SMJobBless](https://developer.apple.com/library/archive/samplecode/SMJobBless/Introduction/Intro.html)

> 通过ServiceManagement注册LaunchdDaemon


### LaunchdDaemon 特点：
* 子进程会被放在 /Library/PrivilegedHelperTools 
* 相应的plist配置文件会被放在 /Library/LaunchDaemons ，Launchd加载该子进程会需要读取该配置文件 


### 特点 
* 优点
    * 目前主流的提权方案 
    * 将高权限任务封装到独立的子程序按需调用，不会让整个程序处于高权限的状态，相对安全 
    * 子程序可实现开机启动、长驻后台、高权限的需求
* 缺点
    * 使用起来较为繁琐 
    * 弹出认证对话框的提示内容是”需要安装帮助程序”，这样的提示不够友好
    * LaunchdDaemon 及其配置文件是需要安装到 /Library 下的，可能存在卸载残留的问题  


## 方案三  
__AppleScript__ 


`do shell script "..." with administrator privileges` 
> 省略号部分填入shell脚本即可。


### AppleScript 执行方式 
* Process

```swift 
let task = Process()
task.launchPath = "/usr/bin/osascript"
task.arguments = ["-e", "do shell script \"...\" with administrator privileges"]
task.launch()

```
#### 注意点 
* 在所有shell执行完成后才会把stdout返回，因此当启动的是Daemon进程，就算使用Process的NSFileHandleDataAvailable Notification，也无法把stdout分次读取出来，不能做到与用户进行良好的交互。 
* 这种方法的认证窗口提示信息是 osascript wants to make changes.，对于小白用户来说会不会有一种这样的感觉，我明明安装的是 XXX.app，怎么来了一个osascript让我输入密码？ 
解决方法：添加 with prompt "xxx" 参数自定义提示信息



* NSAppleScript 
```swift 
let script = "do shell script \"...\" with administrator privileges"
var error: NSDictionary?
if let scriptObject = NSAppleScript(source: script) {
  let output: NSAppleEventDescriptor = scriptObject.executeAndReturnError(&error)
  print(output.stringValue)
}
```







