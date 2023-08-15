## Open GL  
是一个与硬件硬件相关的软件接口， 包含__100+__徒刑操作函数， 开发者可以利用这些函数实现构造模型、进行三维图形交互软件开发。  

__用户必须从点、线、面等最基本的图形淡云开始构造自己的三维模型__

### 操作步骤：
1. 根据图形单元建立景物模型， 并且对所建立的模型进行数学描述(点、线、多边形、图像和位图 等基本图形单元)  
2. 把景物模型放在三维空间坐标系中的合适位置， 并且设置视点，以观察所感兴趣的景观   
3. 计算模型中所有物体的色彩， 其中色彩需要根据不同的光照条件、纹理粘贴方式等  
4. 景物模型的数学描述及色彩信息转换为计算机屏幕上的像素---------光栅化



### 主要功能 
1. 模型绘制   
2. 模型观察  
	视点   
3. 颜色模式   
4. 光照应用  
	* 辐射光  
	* 环境光  
	* 镜面光   
	* 漫射光   

5. 图像效果增强  
6. 位图和图像处理   
7. 纹理映射   
8. 实时动画  




### OpenGL语法  
* 核心库： gl  (100+ 函数)
* 实用库： glu (43 函数)
* 辅助库： aux  (32 函数)
* 工具包： glut  (30+ 函数)   


### 函数命名规则  
__<库前缀><根命令><可选参数个数><可选参数类型>__  
* glColor3f(...)  
	* gl 函数来自 gl 函数库  
	* color 该函数用于颜色设定  
	* 3f 表示三个浮点数作为参数   
	





### 概念  
1. 光栅化：  
	图形是由像素点构成的， 将像素点显示在屏幕上的过程称为光栅化    
	__将顶点数据转化为片元的过程 
2. 着色    
	渲染图形的时候给像素点添加颜色  
3. 纹理贴图  
	将纹理图片附着到需要绘制的图像上   
4. 混合  
	颜色混合效果   

5. 渲染  
	* 线框渲染 
		平面通过线条形成   
	* 纯色渲染  
		通过颜色形成   
	* 纹理渲染  
		添加纹理到形状上   

__OpenGL 中， 只有 点， 线， 三角形__ 通过这三者，拼成各种形状    

6. CPU 与 GPU 通信过程   
	CPU 将数据提交给 GPU 处理，通过 OpenGL. OpenGL的buffers缓存区域（颜色缓存区、顶点缓存区、深度缓存区）解决数据饥饿问题，CPU和GPU的数据处理能力高于内存，CPU和GPU控制的内存是分开的，不能从一块内存复制到另一块内存执行，因为复制数据速度很慢，CPU和GPU都不能操作数据，也避免引起错误。   
	

7. __定点数组(VertexArray ) 和 顶点缓冲区(Vertex Buffer)__        
	* 顶点数据由 GPU 处理  
	* 顶点数组   
		OpenGL中图像都是由图元组成的，在OpenGL ES中，有3种类型的图元：点、线、三角形，顶点数组用来存储他们的顶点数据，顶点数组存储在内存中    
	* 顶点缓存区  
		提前分配一块显存，将顶点数据预先传入到显存当 中，这部分的显存，就被称为顶点缓冲区， __顶点缓冲区存储在显卡的显存中__  


8. __管线__  
	显卡在处理数据的时候是按照一个固定的顺序来执行的，而且严格按照这个顺序，像流水线一样，所以称之为管线，这个顺序就是 __渲染流程__   

















##### 着色器  
1. 分类  
	* 定点着色器 (必选)
	* 细分着色器 (可选)
	* 几何着色器 (可选)
	* 片元着色器 (必选)

2. 着色器渲染流程  
	* 定点着色器  
		定点数据 -----> 定点着色器(接收定点数据，单独处理每个顶点)   
	* 片元着色器 
		给像素点添加颜色    
	


##### Context  
__是一个非常强大的状态机， 保存了 OpenGL 中的各种状态__  

* 高效解决方案  
	由于切换上下文是较大开销， 所以在应用程序中分别创建不同的上下文， 在不同线程中使用不同的上下文， __上下文之间共享纹理， 缓存区等资源__   
	

##### 着色器 Shader  
* shader: 自定义可编程管线， 操作 GPU 芯片计算数据   
* 处理流程  
	* 编译  
	* 链接  
	* 生成着色器程序   
	```
	顶点着色器 -----> 顶点数据运算 -----> 图元装配 ----->顶点转化为图元 -------> 光栅化 ------> 片元着色器  
	```   
	
* 顶点着色器(Vertex Shader) 
	用来处理图形每个顶点变化， 旋转， 平移， 投影    
* 片元着色器 (Fragment Shader)  
	处理每隔像素颜色，填充等   


## Open GL 开发  
1. GLAD(用来管理函数指针)
	__由于 OpenGL 只是一个规范，但具体函数实现地址是不确定的， 需要在运行时查询__   
	
	```
	// 定义函数原型  
	tyoedef void (*GL_GENBUFFERS)(GLSizei, GLuint*); 
	
	// 找到正确的函数并赋值给函数指针  
	GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers"); 
	// 使用函数 
	GLuint buffer; 
	glGenBuffers(1, &buffer);
	```
	
	* 初始化 GLAD   
		
		```
		if(!gladloadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			return-1;
		}
		```  

	

2. Open GL 渲染流程  
	![](./assets/pipeline.png)
	



### Open GL 流程  
1. 初始化 open GL 环境  
2. 初始 GLAD  
3. 着色器处理  
	* 顶点着色器 
		* 初始化顶点着色器 
		* 编译
	* 片元着色器
		* 初始化  
		* 编译 
4. program  
	* 初始化  
	* 绑定着色器(片元，顶点)  
	* 链接  
	* 删除顶点，片元着色器 

5. 顶点缓冲区处理  
	* 顶点数据  
	* 生成缓冲区  
	* 绑定缓冲区  
	* 将数据绑定到缓冲区  

6. 链接顶点属性(告诉OpengGL 怎么处理顶点)  
6. 绘制  


### 纹理 
##### 概念 
1. 纹理是一张2D图片， 可以用来给物体添加细节  
2. 可以用来存储大量的数据， 这些数据恶意发送到着色器上  






## GLSL  
#### 通用模板 
```
// 版本 
#version version_number 

// 输入变量   
in type in_variable_name;
in type in_variable_name;

// 输出变量
out type out_variable_name;

// uniform
uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

###### 说明 
1. __顶点着色器__: 每个输入变量叫  __顶点属性__, 能声明的顶点属性是有上限的， OpenGL 确保最少有 16个包含4分量的顶点属性可用   
	`int nrAttributes;  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);` 查询支持个数   

2. 支持数据类型  
	* int 
	* float  
	* double  
	* uint   
	* bool  
	* 容器类型  
		* vector(向量)    
			* `vecn`:   包含 `n` 个 float 分量的默认向量
			* `bvecn`:  包含 `n` 个bool 分量的默认向量
			* `ivecn`:  包含 `n` 个 int 分量的默认向量
			* `uvecn`:  包含 `n` 个 unsigned int 分量的默认向量
			* `dvecn`: 包含 `n` 个 double 分量的默认向量
		* matrix(矩阵)  
	
3. 着色器之间数据传递  
	* 必须在发送方定义一个输出， 在接收方定义同类型，同名的输入  




	




### 顶点着色器(.vsh) 
```
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

#### 说明
* 通过 `in` 声明所有的输入顶点属性  
* 通过`layout (location = 0)` 设置输入变量的位置值   
* 顶点着色器输出`gl_Position ` 预定义变量   




### 片元着色器(.fsh)
```
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```
#### 说明
1. 片元着色器 __只需要一个输出变量, 是一个 vec4向量__     
2. main 中针对这个输出量进行计算   


### 编译 着色器
```
unsigned int shaderID;
//  GL_FRAGMENT_SHADER: 片元着色器 
//  GL_VERTEX_SHADER: 顶点着色器     
shaderID = glCreateShader(GL_VERTEX_SHADER);   
// 附着源码到着色器   
glShaderSource(shaderID, 1, &ShaderSource, NULL);
glCompileShader(shaderID);

// 编译结果判断， 并获取错误日志
int  success;
char infoLog[512];
glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}
```


### 着色器程序(Shader Program Object)
1. 创建shader progrem 对象   
2. 添加着色器  
3. 链接  
4. 使用  

```
unsigned int shaderProgram;
shaderProgram = glCreateProgram();

glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
}

// 链接成功了， 可以删除着色器对象了 
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);

glUseProgram(shaderProgram);
```

### Uniform  
__一种从CPU 中的应用向 GPU 中的着色器发送数据的方式__    

* 全局的； 在每隔着色器程序对象中是独一无二的， 可以被着色器程序的任意着色器在任意节点访问        
* 无论将 uniform 设置为什么值， 会一直保存他们的数据， 直到他们被重置或更新    
* 如果定义，不使用， 编译器会移除定义   

```
// 在着色器中定义 uniform 属性  


// 使用  
float  timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");   // 查询
glUseProgram(shaderProgram);		// 这里特殊要求： 更新前，必须先使用
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```






## 扩展  
#### Open GL 库  
* gl*  
	opengl 原生库  
* glu*  
	opengl 实用库  
* glut/glut*  
	需要下载配置(太老了)  
* freeglut/ glut* 
	需要下载配置，完全兼容 glut, 算是替代品，但 bug 较多  
* glfw*  
	需要下载配置. 跨平台，轻量级，开源  
* glew*  
	跨平台的C++扩展库，为了方便的管理平台与opengl版本不匹配，以及方便的解决不同显卡特有的硬件接口支持。  
	只要包含一个glew.h头文件，你就能使用gl,glu,glext,wgl,glx的全 部函数。    

* gl3w*  
	gl3w是获取OpenGL核心配置文件规范所提供功能的最简单方法。  
* glad*  
	基于官方规格的多语言GL / GLES / EGL / GLX / WGL装载机 - 生成器。一般结合GLFW使用。







