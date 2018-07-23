//
//  main.cpp
//  Sphere
//
//  Created by boone on 2018/7/23.
//  Copyright © 2018年 boone. All rights reserved.
//
#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cmath>

using namespace std;

vector<float> vertices;    //用于存储pcm文件解析出的数据
vector<float>::iterator istart;   //指向每次绘图的的数据起点
vector<float>::iterator iend;     //指向每次绘图的数据终点

const int n = 1000;
GLfloat R = 0.6f;
const GLfloat pi = 3.1415926536f;


//回调函数、窗口调整大小时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void pressInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//PCM文件数据解码保存到数组中
void fileOutput()
{
    short pcm_In = 0;
    int size = 0;
    FILE *fp = fopen(OLD_FILE_PATH, "rb+");     //为读写打开一个二进制文件 即pcm文件
    
    while(!feof(fp))
    {
        size = fread(&pcm_In, 2, 1, fp);     //pcm中每个数据大小为2字节，每次读取1个数据
        if(size>0)
        {
            //-------------------------------------------------------------------------------------------------------------------------
            if(pcm_In<0){
                pcm_In=-pcm_In;
            }
            vertices.push_back((float)pcm_In/50000);
        }
    }
    
    fclose(fp);
}

void drawLint()
{
    usleep(99900);    //实现延时
    
    glClearColor (0, 0, 0, 0.8);
    glClear (GL_COLOR_BUFFER_BIT);
    
    glLineWidth(3);//设置线段宽度
    glBegin(GL_LINES);
    
    //设置颜色动态变化
    float timeValue = glfwGetTime();
    float redValue = sin(timeValue) / 2.0f +0.5f;
    glColor3f(redValue,0.1,0.6);
    
    //testing-------------------------------------------------------------------------------------------------------------------
    for (int i = 0; i < n; i++)
    {
        glVertex2f(R*cos(2*pi/n*i), R*sin(2*pi/n*i));
    }

    glEnd();
}

int main(void)
{
    fileOutput();
    istart = vertices.begin();
    iend = vertices.begin()+3000;
    
    GLFWwindow* window;
    
    //初始化库
    if (!glfwInit())
        return -1;
    
    //创建窗口
    window = glfwCreateWindow(800, 800, "Visualize Music ", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //循环渲染直至用户关闭窗口
    while (!glfwWindowShouldClose(window))
    {
        //检测按键，判断是否退出
        pressInput(window);
        
        //绘图
        drawLint();
        
        //交换颜色缓冲
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    //删除分配的所有资源
    glfwTerminate();
    
    return 0;
}
