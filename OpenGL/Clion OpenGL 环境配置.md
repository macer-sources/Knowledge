
1. install 库 

brew install glfw 
brew install glew 



2. 编写 CMake 

```cmake

cmake_minimum_required(VERSION 3.20)
project(Open_GL_Learning)

set(CMAKE_CXX_STANDARD 11)


==================================================
# 定义变量
set(GLEW_HEAD_DIR /usr/local/opt/glew/include/GL/)
set(GLFW_HEAD_DIR /usr/local/opt/glfw/include/GLFW/)

#头文件目录、库目录、要链接的库
include_directories(${GLEW_HEAD_DIR} ${GLFW_HEAD_DIR})


set(GLEW_LINK /usr/local/opt/glew/lib/libGLEW.dylib)
set(GLFW_LINK /usr/local/opt/glfw/lib/libglfw.dylib)




link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})




add_executable(Open_GL_Learning main.cpp Window.cpp Window.h)

if (APPLE)
    target_link_libraries(Open_GL_Learning "-framework OpenGL")
    target_link_libraries(Open_GL_Learning "-framework GLUT")
endif()

==================================================

```

3. 测试代码 

```c++


#include <glew.h>
#include <glfw3.h>
using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}

int main(void)
{
    //初始化GLFW库
    if(!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow* window = glfwCreateWindow(640, 480, "hello world", NULL, NULL);
    if(!window)
    {
        //创建失败会返回NULL
        glfwTerminate();
    }
    //建立当前窗口的上下文
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback); //注册回调函数
    //循环，直到用户关闭窗口
    while(!glfwWindowShouldClose(window))
    {
        /*******轮询事件*******/
        glfwPollEvents();

        /*******渲染*******/
        //选择清空的颜色RGBA
        glClearColor(0.2, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //开始画一个三角形
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); //Red
        glVertex3f(0, 1, 1);

        glColor3f(0, 1, 0); //Green
        glVertex3f(-1, -1, 0);

        glColor3f(0, 0, 1); //Blue
        glVertex3f(1, -1, 0);
        //结束一个画图步骤
        glEnd();

        glBegin(GL_POLYGON);
        //再画个梯形，需要注意笔顺
        glColor3f(0.5, 0.5, 0.5); //Grey
        glVertex2d(0.5, 0.5);
        glVertex2d(1, 1);
        glVertex2d(1, 0);
        glVertex2d(0.5, 0);
        glEnd();


        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}


```