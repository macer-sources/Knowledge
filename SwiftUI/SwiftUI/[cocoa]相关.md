
## Cocoa 相关的swiftUI 实现 


## 检测活动窗口 
* @FocusedBinding 
* EnvironmentValue ----> controlActiveState


## Commands
1. 系统默认提供的
```swift 
.commands {
	SidebarCommands()
	ToolbarCommands()
	TextEditingCommands()
	TextFormattingCommands()
}
```

2. 使用单独文件自定义 
```swift 

struct MenuCommands: Commands {
	@Binding var toggleSetting: Bool

	var body: some Commands {
		CommandMenu("Custom Menu") {
			Button(action: {
				print("Menu Button selected")
			}, label: {
				Text("Menu Button")
			})

			Divider()

			Toggle(isOn: $toggleSetting, label: {
				Text("Toggle")
			})
		}
	}
}

```