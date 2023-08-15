




## VAO (Vertex Array Object: 顶点数组对象) 

VBO保存了一个模型的顶点属性信息，每次绘制模型之前需要绑定顶点的所有信息，当数据量很大时，重复这样的动作变得非常麻烦。VAO可以把这些所有的配置都存储在一个对象中，每次绘制模型时，只需要绑定这个VAO对象就可以了。

__VAO是一个保存了所有顶点数据属性的状态结合，它存储了顶点数据的格式以及顶点数据所需的VBO对象的引用。__  

VAO本身并没有存储顶点的相关属性数据，这些信息是存储在VBO中的，VAO相当于是对很多个VBO的引用，把一些VBO组合在一起作为一个对象统一管理。



### 使用 
```c

GLuint vaoId;//vertext array object句柄
glGenVertexArrays(1, &vaoId);
glBindVertexArray(vaoId);

```


### 绘制glDrawArrays  
glDrawArrays函数负责把模型绘制出来，它使用当前激活的着色器，当前VAO对象中的VBO顶点数据和属性配置来绘制出来基本图形。


```c

/** 
第一个参数表示绘制的类型：
    1.GL_TRIANGLES：每三个顶之间绘制三角形，之间不连接；
    2.GL_TRIANGLE_FAN：以V0V1V2,V0V2V3,V0V3V4，……的形式绘制三角形；
    3.GL_TRIANGLE_STRIP：顺序在每三个顶点之间均绘制三角形。这个方法可以保证从相同的方向上所有三角形均被绘制。以V0V1V2,V1V2V3,V2V3V4……的形式绘制三角形；

第二个参数定义从缓存中的哪一位开始绘制，一般定义为0；

第三个参数定义绘制的顶点数量；

*/

glDrawArrays (GLenum mode, GLint first, GLsizei count)
```








#### 存储的内容
1. `glEnableVertexAttribArray`和`glDisableVertexAttribArray`的调用。
2. 通过`glVertexAttribPointer`设置的顶点属性配置。
3. 通过`glVertexAttribPointer`调用与顶点属性关联的顶点缓冲对象。




## VBO (Vertex Buffer Object: 顶点缓冲对象)(可以有多个)

顶点缓冲对象VBO是在显卡存储空间中开辟出的一块内存缓存区，用于存储顶点的各类属性信息，如顶点坐标，顶点法向量，顶点颜色数据等。在渲染时，可以直接从VBO中取出顶点的各类属性数据，由于VBO在显存而不是在内存中，不需要从CPU传输数据，处理效率更高。

__VBO就是显存中的一个存储区域，可以保持大量的顶点属性信息__  


* 类型： `GL_ARRAY_BUFFER`



```C
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// 创建
unsigned int VBO;
glGenBuffers(1, &VBO);      //生成缓冲对象，给定id 1

// 绑定为  GL_ARRAY_BUFFER 顶点缓冲对象, 同一个类型一次只能绑定一个 
glBindBuffer(GL_ARRAY_BUFFER, VBO);

// 将数据绑定到缓冲区上 
/**
GL_STATIC_DRAW ：数据不会或几乎不会改变。
GL_DYNAMIC_DRAW：数据会被改变很多。
GL_STREAM_DRAW ：数据每次绘制时都会改变
*/
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


/**

第一个参数指定顶点属性位置，与顶点着色器中layout(location=0)对应。
第二个参数指定顶点属性大小。
第三个参数指定数据类型。
第四个参数定义是否希望数据被标准化。
第五个参数是步长（Stride），指定在连续的顶点属性之间的间隔。
第六个参数表示我们的位置数据在缓冲区起始位置的偏移量。

*/

// 通知OpenGL如何解释这些顶点数据
glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);


/**
参数： 顶点属性位置值
*/
// 激活顶点属性(默认是关闭的)
glEnableVertexAttribArray(0);
```





## IBO/EBO (Element Buffer Object: 索引缓冲对象)  
索引缓冲对象EBO相当于OpenGL中的顶点数组的概念. 
__是为了解决同一个顶点多次重复调用的问题，可以减少内存空间浪费，提高执行效率。__
当需要使用重复的顶点时，通过顶点的位置索引来调用顶点，而不是对重复的顶点信息重复记录，重复调用。   


__EBO中存储的内容就是顶点位置的索引indices，EBO跟VBO类似，也是在显存中的一块内存缓冲器，只不过EBO保存的是顶点的索引__  


```c
// GL_ELEMENT_ARRAY_BUFFER 是关键
GLuint EBO;
glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// 注意： 


```

### 绘制 
__使用此种方式 绘制需要使用`glDrawElements` 而不是  `glDrawArrays`__

```c

/**
第一个参数指定了要绘制的模式；
第二个参数指定要绘制的顶点个数；
第三个参数是索引的数据类型；
第四个参数是可选的EBO中偏移量设定。
*/

glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


```


=============================================

## 使用 

### VBO + VAO  

```c
//使用VAO VBO绘制矩形
#include <GL/glew.h>  
#include <GL/freeglut.h>  
 
void userInit();  //自定义初始化
void reshape(int w, int h);   //重绘
void display(void);
void keyboardAction(unsigned char key, int x, int y);   //键盘退出事件
 
GLuint vboId;//vertex buffer object句柄    
GLuint vaoId;//vertext array object句柄    
GLuint programId;//shader program 句柄    
 
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Rectangle demo");
 
	//使用glew，需要执行glewInit，不然运行过程会报错
	//glewInit要放在glut完成了基本的初始化之后执行
	glewInit();
 
	//自定义初始化，生成VAO，VBO对象
	userInit();
 
	//重绘函数
	glutReshapeFunc(reshape);
 
	glutDisplayFunc(display);
 
	//注册键盘按键退出事件
	glutKeyboardFunc(keyboardAction);
 
	glutMainLoop();
	return 0;
}
 
//自定义初始化函数    
void userInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//创建顶点数据    
	const GLfloat vertices[] = {
		-0.5f,-0.5f,0.0f,1.0f,
		0.5f,-0.5f,0.0f,1.0f,
		0.5f,0.5f,0.0f,1.0f,
		-0.5f,0.5f,0.0f,1.0f,
	};
 
	//创建VAO对象
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
 
	//创建VBO对象	
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//传入VBO数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//解除VBO绑定(此时，数据已经存储在vboId对应的vbo 中了，这里暂时不用，所以解绑掉)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
 
//调整窗口大小回调函数    
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
 
//绘制回调函数    
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
 
	//绑定VBO(这里准备使用缓存的数据了，所以绑定vboId对应的缓存)
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glEnableVertexAttribArray(0);
 
	//解释顶点数据方式(此条语句会将解析记录在 vao中，代表如何去解析vboId中的数据)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
 
	//绘制模型
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
 
	glutSwapBuffers();
}
 
//键盘按键回调函数    
void keyboardAction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033:  // Escape key    
		exit(EXIT_SUCCESS);
		break;
	}
}

```






### EBO  


```c

//使用EBO绘制矩形(两个三角形)
#include <GL/glew.h>  
#include <GL/freeglut.h>  
 
void userInit();  //自定义初始化
void reshape(int w, int h);   //重绘
void display(void);
void keyboardAction(unsigned char key, int x, int y);   //键盘退出事件
 
GLuint eboId;//element buffer object句柄    
GLuint vboId;//vertext buffer object句柄    
GLuint vaoId;//vertext array object句柄    
 
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Rectangle demo");
 
	//使用glew，需要执行glewInit，不然运行过程会报错
	//glewInit要放在glut完成了基本的初始化之后执行
	glewInit();
 
	//自定义初始化，生成VAO，VBO,EBO
	userInit();
 
	//重绘函数
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	//注册键盘按键退出事件
	glutKeyboardFunc(keyboardAction);
	glutMainLoop();
	return 0;
}
 
//自定义初始化函数    
void userInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
 
	//创建顶点数据    
	const GLfloat vertices[] = {
		-0.5f,-0.5f,0.0f,1.0f,
		0.5f,-0.5f,0.0f,1.0f,
		0.5f,0.5f,0.0f,1.0f,
		-0.5f,0.5f,0.0f,1.0f,
	};
	// 索引数据
	GLshort indices[] = {
		0, 1, 3,  // 第一个三角形
		1, 2, 3   // 第二个三角形
	};
 
	//创建VAO对象
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
 
	//创建VBO对象，把顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
	//创建EBO对象	
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	//传入EBO数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
 
	//解释顶点数据方式
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
 
	//解绑VAO
	glBindVertexArray(0);
	//解绑EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
 
//调整窗口大小回调函数    
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
 
//绘制回调函数    
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//绑定VAO
	glBindVertexArray(vaoId);
	//绘制模型 (null 如果是ebo 数组索引，就不需要ebo 了)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
	glutSwapBuffers();
}
 
//键盘按键回调函数    
void keyboardAction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033:  // Escape key    
		exit(EXIT_SUCCESS);
		break;
	}
}



```







* 优势
    可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。  
    从CPU把数据发送到显卡相对较慢，所以只要可能我们都要尝试尽量一次性发送尽可能多的数据。



当数据发送至显卡的内存中后，顶点着色器几乎能立即访问顶点，这是个非常快的过程。







## 总结  
1. VBO: 用于管理存储在gpu 上的顶点数据。 缓存类型`GL_ARRAY_BUFFER` __有具体的类型__  
2. VAO: 不能存储实际的顶点数据， 而是放的顶点结构定义，数组 中的每一项都对应一个属性的解析，用来OpenGl 如何解释顶点数据
3. IBO/EBO: 缓存类型`GL_ELEMENT _ARRAY_BUFFER`时， VAO 也会存储 `glBindBuffer` 函数的调用。也就意味着它也会存储解绑掉用； 但是 VAO 不会存储 `GL_ARRAY_BUFFER` 的  `glBindBuffer` 函数的调用；
