# GLSL 指南


## shader 中访问顶点数据 
1. 另一种启用顶点数组的方式 

```c++
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);

//禁用
glDisenableClientState(GL_VERTEX_ARRAY);
glDisenableClientState(GL_COLOR_ARRAY);

```



2. 顶点着色器中的变量 
* `gl_Vertex`: 顶点数据  
* `gl_Position`: 
* `gl_ModelViewProjectionMatrix`: 



## shader 中访问颜色数据 
1. 每个顶点支持两个颜色  `gl_Color` 和  `gl_SecondaryColor` 
2. 在传递颜色参数的时候，有两种数据类型。 float 和 char 类型 



## shader 中访问纹理坐标数据 
1. 顶点着色器中， 通过 `gl_MultiTexCoord0`(__只能在顶点着色器中使用__)从应用程序中输入的纹理坐标。 `gl_TexCoord` 是输出的纹理坐标。 __最少支持 8 组坐标数据__

2. 启用纹理坐标数组 
```c++
glEnableClientState(GL_TEXTURE_COORD_ARRAY);

// 传递数据 
glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &rect[0].u);

```


3. 通过顶点着色器传过来的纹理坐标获取纹理颜色 

```
uniform sampler2D _texture; // 纹理

void main() {
    vec4 color = texture2D(_texture, gl_TexCoord[0].st);
    gl_FragColor = color;
}

```



## shader 中访问法线数据
法线： 用于光照计算  
* `点法线`： 
* `面法线`：

```c++
// 开启法线
glEnableClientState(GL_NORMAL_ARRAY);


// 设置发现数据 
glNormalPointer(GL_FLOAT, sizeof(xxx), 首地址);
```




## GLSL 内置变量 
GLSLangSpec.pdf 

---------------------------------------------------------------------------

## attribute 属性 
基础情况下， shader 中可以传递的参数是 颜色，法线，纹理，顶点 。 使用 attribute 属性，可以添加自定义的数据

```c++
attribute vet4 _xxxx;


// 设置值 
int color = glGetUniformLocation(_propram, "_color");
int xxxx  = glGetAttribLocation(_propram, "_xxxx");
glEnableVertexAttribArray(_shader.xxxx);        // 启用这个属性
glVertexAttribPointer(_shader.xxxx, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex));// 传递参数
```

### attribute 和 unifrom 的区别 
1. 可以自己定义， 传递任意类型的数据 
2. 接口统一了，不需要根据不同的设置不同的









## uniform 
* 是全局的， 可以被人以着色器程序在任意阶段访问  
* __另一种从cpu 应用， 向 gpu 着色器发送数据的方式(另一种是： in)__
* 如果声明 uniform 没有使用， 编译器会优化并移除～～

```

int vertexColorLocation = glGetUniformLocation(shader, "color");
shader.setUniformValue(vertexColorLocation, 0.0f, 1.0, 0, 1.0);



```


## in/out 使用(1.3版本)
## layout 使用修饰 attribute (3.0版本 330)  
__只能修饰在 `in` 之前__  

```c++
使用layout 
layout(location = 0) in vec3 _position;


glEnableVertexAttribArray(0);               // 启用
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex));// 传递参数
// 第一个参数就可以直接传递 layout 中的 location 来确定传递的参数是给那个的 

// 不使layout 
in vec3 _position;
int _position  = glGetAttribLocation(_propram, "_position");
glEnableVertexAttribArray(_shader._position);        // 启用这个属性
glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex));// 传递参数

```


#### layout location 必要性 
layout location 可以忽略， 在代码中使用 `glGetAttribLocation` 来查询属性位置值， 或者通过 `glBindAttribLocation` 属性位置值，但是 __推荐在着色器中设置__




## layout 修饰  out (410 版本) 
__可以修饰 out__  

### 带来的变化： 之前要求， fs 输出必须和 vs 输出变量名一致， 通过 layout 修饰out 后。可以不一致


```c++

// vs
layout(location = 0) out vec3 _outColor;
layout(location = 1) out vec3 _outUV;


// fs  
layout(location = 0) in vec3 _inColor;
layout(location = 0) in vec3 _inUV;

```


## layout 修饰 unifrom (430 版本) 



## 属性数组

```c++
layout(location  = 0) in    vec3    _posAndColor[3];
layout(location  = 3) in    vec3    _uv;            // 当上边的是数组时， 这个location 必须是 后移的


```







