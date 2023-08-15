## 基本步骤 
1. 导入包    

	```
	import 'package:flutter/material.dart';
	```
2. 入口    

	```
	void main() => runApp(MyApp());
	```
3. 应用结构  

	```
	class MyApp extends StatelessWidget {
		
		Widget build(BuildContext context) {
			
			return new MaterialApp (
			
			)
		}
	}
	```
	
	* Flutter中大多数的东西都是 `widget`  
	* Flutter 在构建页面时， 会调用组件的 `build` 方法， widget的主要工作是提供一个 build 方法来提供UI 界面构建  

	
4. 首页  
	
	```   
	StatefulWidget 代表有状态的 widget 
	class HomePage extends StatefulWidget {
		// 创建状态
		XXXX createState() => new XXXX();
	}
	``



### Widget 生命周期  
#### State
1. init 方法  
2. initState 
3. build  
4. dispose  

#### Widget

 











### 总结  
1. Widget 的有状态和无状态  
	* 有状态`StatefulWidget `: 可以有状态， 这些状态在生命周期中可以变的  
		* 至少由两个类组成： `State` 类  和 `StatefulWidget ` 类
		* build 方法会被移动到 `State` 类中实现   	  
	* 无状态 `StatelessWidget`： 


2. 导航控制 
	
	```
	
	Navigator.push(context,
	 MaterialPageRoute(builder: (context) {
              return NewRoute();
           }));
	)
	// 等价于 
	Navigator.of(context).push()
	```
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	