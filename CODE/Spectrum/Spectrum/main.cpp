//
//  main.cpp
//  Spectrum
//
//  Created by boone on 2018/7/30.
//  Copyright © 2018年 boone. All rights reserved.
//
#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

vector<float> vertices;    //用于存储pcm文件解析出的数据
vector<float>::iterator istart;   //指向每次绘图的的数据起点
vector<float>::iterator iend;     //指向每次绘图的数据终点
int n;       //记录pcm文件中数据个数

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void pressInput(GLFWwindow *window);
void fileOutput();
void drawSpectrum();
void drawLine();
void drawPoint();

int main()
{
    fileOutput();
    
    //初始化绘图的起点终点
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
        if (iend<=vertices.end()) {
            // sleep(0.003);
            drawSpectrum();
            drawLine();
            drawPoint();
        }
        
        //交换颜色缓冲
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    //删除分配的所有资源
    glfwTerminate();
    
    return 0;
}


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
    
    int i=0;
    while(!feof(fp))
    {
        size = fread(&pcm_In, 2, 1, fp);     //pcm中每个数据大小为2字节，每次读取1个数据
        if(size>0)
        {
            //-------------------------------------------------------------------------------------------------------------------------
            if(pcm_In<0){
                pcm_In=-pcm_In;
            }
            vertices.push_back((float)pcm_In/15000);
        }
        i++;
    }
    
    n=i;
    //    cout<<"数据个数： "<<n<<endl;
    
    fclose(fp);
}

//柱状频谱图的绘制
void drawSpectrum()
{
    usleep(99900);    //实现延时
    
    glClearColor (0, 0, 0, 0.8);
    glClear (GL_COLOR_BUFFER_BIT);
    
    glLineWidth(3);//设置线段宽度
    glBegin(GL_LINES);
    
    float xstart=-1.0;
    //testing-------------------------------------------------------------------------------------------------------------------
    float redValue = 0.0f;
    float yellowValue = 1.0f;
    int k=0;
    
    //绘制频谱图
    for(vector<float>::iterator it = istart; it != iend; it+=2 )    //用迭代器的方式输出容器对象的值
    {
        glColor3f( redValue, 1.0f, yellowValue);
        k++;
        if (k<=500) {
            redValue=redValue+0.006;
            yellowValue=yellowValue-0.006;
        }else{
            redValue=redValue-0.006;
            yellowValue=yellowValue+0.006;
        }
        
        xstart=xstart+0.006;
        glVertex2f(xstart,0);
        glVertex2f(xstart,*it+0.001);
    }
    
    //进行下一次绘制的起点和终点
    istart+=1111;    //通过更改每次前进的数字可以实现波形振动频率的改变
    iend+=1111;
    
    glEnd();
}


//波形图的绘制
void drawLine()
{
    
    glLineWidth(2);//设置线段宽度
    glBegin(GL_LINES);
    
    float temp = 0.0;
    float xstart=-1.0;
    float xend=-1.0;
    
    float redValue = 1.0f;
    float yellowValue = 1.0f;
    int k=0;
    
    //绘制波形图
    for(vector<float>::iterator it = istart; it != iend; it+=2 )    //用迭代器的方式输出容器对象的值
    {
        glColor3f( redValue, 1.0f, yellowValue);
        
        k++;
        
        if (k<=250) {
            redValue=redValue-0.0006;
            yellowValue=yellowValue-0.0006;
        }else{
            redValue=redValue+0.0006;
            yellowValue=yellowValue+0.0006;
        }
        
        xstart=xstart+0.006;
        xend=xstart+0.006;
        
        glVertex2f(xstart,-temp);
        glVertex2f(xend,-*it);
        temp = *it;
    }
    
    //进行下一次绘制的起点和终点
    istart+=1111;
    iend+=1111;
    
    glEnd();
}

//离散频谱点的绘制
void drawPoint()
{
    glPointSize(2);
    glBegin(GL_POINTS);

    float xstart=-1.0;
    
    float redValue = 0.0f;
    float yellowValue = 1.0f;
    int k=0;
    
    //绘制波形图
    for(vector<float>::iterator it = istart; it != iend; it+=2 )    //用迭代器的方式输出容器对象的值
    {
        glColor3f( redValue, 1.0f, yellowValue);
        k++;
        if (k<=250) {
            redValue=redValue+0.0006;
            yellowValue=yellowValue-0.0006;
        }else{
            redValue=redValue-0.0006;
            yellowValue=yellowValue+0.0006;
        }
        
        xstart=xstart+0.011;
        
        glVertex2f(xstart,*it);
        glVertex2f(xstart,-*it);

    }
    
    //进行下一次绘制的起点和终点
    istart+=1111;
    iend+=1111;
    
    glEnd();
}

