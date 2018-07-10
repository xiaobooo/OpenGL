//
//  main.cpp
//  Waveform2.0
//
//  Created by boone on 2018/7/9.
//  Copyright © 2018年 boone. All rights reserved.
//

#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

void drawLint()
{
    glClearColor (0.8, 0.8, 0.8, 0.8);
    glClear (GL_COLOR_BUFFER_BIT);
    
    glLineWidth(2);//设置线段宽度
    glBegin(GL_LINES);
    glColor3f(0.3,0.6,0.8);
    
    short pcm_In = 0;
    int size = 0;
    float xstart=-1.0;
    float ystart=0.0;
    float xend=0.0;
    float yend=0.0;
    float temp = 0.0;
    
    FILE *fp = fopen(OLD_FILE_PATH, "rb+");     //为读写打开一个二进制文件 即pcm文件
    
    while(!feof(fp))
    {
        size = fread(&pcm_In, 2, 1, fp);     //pcm中每个数据大小为2字节，每次读取1个数据
        if(size>0)
        {
            //确定绘制波形的折线两点坐标
            xstart=xstart+0.0000001;
            ystart=temp;
            xend=xstart+0.0000001;
            
            //-------------------------------------------------------------------------------------------------------------------------
            yend=(float)pcm_In/30000;       //short类型除以一个数要么是0要么是大于一的整数，所以坐标变换要注意先转为float
            
            //绘制折线
            glVertex2f(xstart,ystart);
            glVertex2f(xend,yend);
            
            temp=yend;    //终点作为下一次的起点
        }
    }
    
    fclose(fp);
    
    glEnd();
}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Visualize Music ", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        
        /*your draw*/
        // drawPoint();
        drawLint();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}


