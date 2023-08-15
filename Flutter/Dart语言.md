## 屏幕显示原理  
1. 单缓存  
	造成显示的时两帧画面的结合体   
2. 双缓存  
3. 三重缓存  


## 变量  
* Var  
	```
	var t; 
	t = "hellc world"
	
	// 报错
	t = 1000;
	
	String name = "指定类型";
	
	```
	__可以接收任意类型，但一旦赋值类型确定，将无法改变类型__
* final 和 const  
	final 是可以在运行期确定值； const 必须在编译期确定值   
	
	```
	const name = ""; 
	final p = Person("test"); 
	
	// 定义可以在const 使用的对象  
	class Test { 
		final String name;
		// const 构造器  
		const Test(this.name) 
	}
	
	```

* dynamic 和 Object   
	__Object 是所有对象的基类， 包括`Function和 Null`，任何对象都可以赋值给 Objhect 声明的对象__  
	__dynamic 声明的变量可以赋值任意对象__   
	
	```
	dynamic t; 
	Object x; 
	
	t = "hello"  
	x = "hellc Object"  
	
	t = 1000; 
	x = 1;
	
	```
	* 相同点   
		声明的变量都可以在后期更改赋值类型     
	* 不同点    
		dynamic 声明的对象编译器会提供所有的可能， Object 声明的对象只能使用 Object 的属性和方法。

* fonal 和 const 
	```
	final str = "he snkskns";
	
	const str2 = "jksjskna";
	```
	__const变量是一个编译时常量， final 变量在第一次使用时被初始化, 都可以省略具体类型__   
	

* String  
	
	```
	var str1 = 'ssss'; 
	var str2 = "ssss"; 
	var str3 = """
			dnakfndka,
			nfkdankfdnakfa
	
		""";
		
	// 字符串拼接  
	var message = "anme is ${name} type is ${name.runtimeType}"	// 如果拼接的是一个变量， 可以省略大括号  
	
	```

* 集合  
	* List 
	* Set 
	* Map



* 类  
	* 类的构造函数  
		* 默认提供一个同名无参构造函数  
		* 可显示提供此构造函数  
		* 不支持重载，需要指定为其他名字构造函数  
			`Person.withNameHeight(this.name, this.height);`
	* 初始化列表  
		```
		final String name; 
		final int age;
		// 注意: 构造函数的 {} 中的代码是在对象初始化完毕后执行的，所以此处在 {} 中进行赋值是不对的 
		Person(this.name, {int age}) : this.age = age ?? 10 {
		}
		```
	* 构造函数重定向  
		
		```
		class Person {
			String name;
			int age; 
			Person(this.name) : this._internal(name, 0); 
			Person._internal(this.name, this.age);
		}
		```
	* 常量构造函数  
		
		```
		class Person {
			final String name;
			
			const Person(this.name);
		}
		
		
		``` 
		__通过常量构造函数，创建出的对象，是指向同一块内存的__ 
	* 工厂构造函数  
	
		```
		class Person {
			String name; 
			
			static final Map<String, Person> _nameCache = {};
			
			factor Person.withName(String name) {
				if(_nameCache.containsKey(name)) {
					return _nameCache[name];
				}
			}
		}
		```
		__默认构造函数不需要手动返回对象， 工厂构造函数需要__  
	* 私有属性  
		`String _name;`  
	* getter/ setter  
		
		```
		set setName(String name) {
			this.name = name;
		}
		get getName {
			return name;
		}
		
		```
	* 继承  
		
		```
		class Animal {
			int age; 
			Animal(this.age); 
		}
		
		class Person extends Animal {
			String name; 
			Person(this.name, int age): super(age);
		}
		
		```
	* 抽象类  
		
		```
		abstract class Shape {
			void getArea();
		}
		
		```
		
		* __继承抽象类，必须实现抽象方法__  
		* __抽象类，不能实例化, 但可以通过工厂构造函数实例化__    
			
			```
			abstract class Map {
				int key();
				
				factory Map() {
					return //子类对象
				}
			}
			
			```
		
		 
	

## 函数  
__函数也是对象，函数可以赋值给变量或者作为参数传递给其他函数__   
 
1. 可选参数：
	* 位置可选参数  
		* 传参的顺序不可更改，形参和实参会根据位置进行匹配  
	* 命名可选参数  
		* 根据名称来做一一对应  
2. 参数默认值  
	只有可选参数可以带有默认值  



* 函数声明  
	```
	typeof Calculate = int Function(int num1, int num2);
	
	bool  test(int a) {
		return a > 0;
	}
	```
	__如果不给定具体返回类型，默认返回类型为 `dynamic`__  
	
* 函数简写方式  
	```
	bool test(int a) => a > 0;
	```
* 函数作为变量  
	```
	var say = (str) {
		print(str);
	}
	say("test");
	```
* 函数作为参数传递  
	```
	void execute(var callback) {
		callback();
	}
	execute(() => print(""));	
	```
* 可选参数   
	__可选位置参数通过 [] 标记， 并且放在参数列表做后面__  
	```
	String say(String from, String msg, [String device = "可选参数可以有默认值"]) {
		  var result = '$from says $msg';
		  if (device != null) {
		    result = '$result with a $device';
		  }
		  return result;
		} 
	say('Bob', 'Howdy');
	say('Bob', 'Howdy', 'smoke signal');
	```

* 可选命名参数(__优先使用__)    
	```
	//设置[bold]和[hidden]标志
	void enableFlags({bool bold, bool hidden}) {
	    // ... 
	}
	
	enableFlags(bold: true, hidden: false);
	```
	此处注意与上边那个的区别
	
### 运算符  
1. `??=`  
	```
	var name = "xxx"; 
	name ??= "xnkxnkx"; 
	
	当原来对象有值时， 不进行赋值操作  
	
	var name = null;
	var temp = name ?? "dfa"; 
	和swift一样，默认值  
	
	```



## 异步 
* `Future` 和 `Stream` 作为异步函数的返回值  
* `async` 和 `await` 支持异步编程  

 
#### Future 
 * then     
	接收异步结果
 * catchError     
 	出现错误，捕捉错误    
 * whenComplete     
 	无论成功或者失败都会执行   
 * wait    
	组合异步任务，接收一个 Future 数组，只有数组中所有Future都执行成功后，才会触发回调。
	
 ```
 Future.delayed(new Duration(seconds: 2),() {
 	
 }).then(data) {
 
 }
 
 ```
 
#### async 和 await 
使用同步的方式执行异步任务  

```
task() async {
   try{
    String id = await login("alice","******");
    String userInfo = await getUserInfo(id);
    await saveUserInfo(userInfo);
    //执行接下来的操作   
   } catch(e){
    //错误处理   
    print(e);   
   }  
}
```

 * async 用来表示函数是异步的，定义的函数返回一个 Future 对象， 可以使用then 方法添加回调函数  
 * await 后面是一个 Future，表示等待该异步任务执行完成，异步完成后才会往下执行。__await必须出现在 async函数内部__  


#### Stream 
__可以接收异步事件数据，可以接收多个异步操作的结果。常用于会多次读取数据的异步任务场景__   
```
Stream.fromFutures([
  // 1秒后返回结果
  Future.delayed(new Duration(seconds: 1), () {
    return "hello 1";
  }),
  // 抛出一个异常
  Future.delayed(new Duration(seconds: 2),(){
    throw AssertionError("Error");
  }),
  // 3秒后返回结果
  Future.delayed(new Duration(seconds: 3), () {
    return "hello 3";
  })
]).listen((data){
   print(data);
}, onError: (e){
   print(e.message);
},onDone: (){

});

```




















