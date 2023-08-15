

## OpenGL 坐标系  



### 标准化设备坐标 

![](./open_gl_ndc.png) 





## 标准-----> 屏幕坐标系 

使用通过`glViewport`函数提供的数据，进行视口变换(Viewport Transform)完成的。





## 坐标转换  
物体坐标系 ------> 世界坐标系 ------> 观察坐标系 -----> 裁剪空间


1. 定义矩阵  
    ```c++  
    uniform mat4x4 modelMat;        // 模型矩阵
    uniform mat4x4 viewMat;         // 模型矩阵 
    uniform mat4x4 projMat;         // 投影矩阵  

    g_Position =  projMat *  viewMat * modelMat * vec4(pos, 1.0f);

    ```

