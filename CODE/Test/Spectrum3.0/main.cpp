//
//  main.cpp
//  Spectrum3.0
//
//  Created by boone on 2018/7/18.
//  Copyright © 2018年 boone. All rights reserved.
//

#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <GLFW/glfw3.h>
#include "FFT.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

vector<double> vertices;    //用于存储pcm文件解析出的数据
int istart;   //指向每次绘图的的数据起点
int iend;     //指向每次绘图的数据终点
int n;       //记录pcm文件中数据个数

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

void drawLint(Complex* outarr)
{
    usleep(44100);    //实现延时
    
    glClearColor (0, 0, 0, 0.8);
    glClear (GL_COLOR_BUFFER_BIT);
    
    glLineWidth(9);//设置线段宽度
    glBegin(GL_LINES);
    
    //设置颜色动态变化
    float timeValue = glfwGetTime();
    float redValue = sin(timeValue) / 2.0f + 0.5f;
    glColor3f(redValue,0.5,0.5);
    
    float xstart=-1.0;
    
    //testing-------------------------------------------------------------------------------------------------------------------
    
    //绘制波形图
    for(int k = istart; k <= iend;  )    //用迭代器的方式输出容器对象的值
    {
        xstart=xstart+0.016;
        glVertex2f(xstart,0);
        glVertex2f(xstart,outarr[k].real);
        k+=2;
    }
    
    //进行下一次绘制的起点和终点
    istart+=1111;    //通过更改每次前进的数字可以实现波形振动频率的改变
    iend+=1111;
    
    glEnd();
}

int main()
{
    fileOutput();
    
    Complex* inarr =new Complex[n];
    Complex* outarr =new Complex[n];
    
    int i=0;
    for(vector<double>::iterator it = vertices.begin(); it != vertices.end(); it++ )    //用迭代器的方式输出容器对象的值
    {
        inarr[i++].real=*it;
    }
    
    FFT(inarr, outarr, n);     //傅里叶变换 时域转换为频域
//    
//    for (int j=0; j<n; j++) {
//        cout<<outarr[j].real<<endl;
//    }
    
    //初始化绘图的起点终点
    istart = 0;
    iend = 3000;
    
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
        if (iend<=n) {
            // sleep(0.003);
            drawLint(outarr);
        }
        
        //交换颜色缓冲
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    //删除分配的所有资源
    glfwTerminate();
    
    return 0;
}
