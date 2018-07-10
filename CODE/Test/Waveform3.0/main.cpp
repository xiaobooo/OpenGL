//
//  main.cpp
//  Waveform3.0
//
//  Created by boone on 2018/7/9.
//  Copyright © 2018年 boone. All rights reserved.
//

#define OLD_FILE_PATH "/Users/boone/Desktop/Music/16k.pcm"     //PCM源文件

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

using namespace std;

vector<float> vertices;
vector<float>::iterator istart;
vector<float>::iterator iend;

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
            vertices.push_back((float)pcm_In/1000);
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
    
    float temp = 0.0;
    float xstart=-1.0;
    float xend=-1.0;
    
    //testing-------------------------------------------------------------------------------------------------------------------
            
    //绘制波形图
    for(vector<float>::iterator it = istart; it != iend; it++ )    //用迭代器的方式输出容器对象的值
    {
        xstart=xstart+0.0033;
        xend=xstart+0.0033;
        
        glVertex2f(xstart,temp);
        glVertex2f(xend,*it);
        temp = *it;
    }
    
    //进行下一次绘制的起点和终点
    istart++;
    iend++;
    
    glEnd();
}

int main(void)
{
    fileOutput();
    istart = vertices.begin();
    iend = vertices.begin()+600;
    
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
        if (iend<=vertices.end()) {
            drawLint();
        }
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}
