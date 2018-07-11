//
//  main.cpp
//  Waveform4.0
//
//  Created by boone on 2018/7/10.
//  Copyright © 2018年 boone. All rights reserved.
//

#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

using namespace std;

vector<float> vertices;    //用于存储pcm文件解析出的数据
vector<float>::iterator istart;   //指向每次绘图的的数据起点
vector<float>::iterator iend;     //指向每次绘图的数据终点

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
            vertices.push_back((float)pcm_In/30000);
        }
    }
    
    fclose(fp);
}

void drawLint()
{
    glClearColor (0.8, 0.8, 0.8, 0.8);
    glClear (GL_COLOR_BUFFER_BIT);
    
    glLineWidth(2);//设置线段宽度
    glBegin(GL_LINES);
    glColor3f(0.6,0.6,0.6);
    
    float xstart=-1.0;
    
    //testing-------------------------------------------------------------------------------------------------------------------
    
    //绘制波形图
    for(vector<float>::iterator it = istart; it != iend; it++ )    //用迭代器的方式输出容器对象的值
    {
        xstart=xstart+0.00066;
        glVertex2f(xstart,0);
        glVertex2f(xstart,*it);
    }
    
    //进行下一次绘制的起点和终点
    istart+=60;    //通过更改每次前进的数字可以实现波形振动频率的改变
    iend+=60;
    
    glEnd();
}

int main(void)
{
    fileOutput();
    istart = vertices.begin();
    iend = vertices.begin()+6000;
    
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
        if (iend<=vertices.end()) {
            drawLint();
        }
        
        //交换颜色缓冲
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    //删除分配的所有资源
    glfwTerminate();
    
    return 0;
}

