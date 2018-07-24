//
//  main.cpp
//  Spectrum3.0
//
//  Created by boone on 2018/7/18.
//  Copyright © 2018年 boone. All rights reserved.
//

#define OLD_FILE_PATH "/Users/boone/Desktop/Music/16k.pcm"     //PCM源文件

#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

vector<float> vertices;    //用于存储pcm文件解析出的数据
vector<float>::iterator istart;   //指向每次绘图的的数据起点
vector<float>::iterator iend;     //指向每次绘图的数据终点
int n;       //记录pcm文件中数据个数

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

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

void drawLine()
{
    usleep(44100);    //实现延时
    
    glClearColor (0, 0, 0, 0.8);
    glClear (GL_COLOR_BUFFER_BIT);
    
    glLineWidth(9);//设置线段宽度
    glBegin(GL_LINES);
    glColor3f(0.9,0.3,0.3);
    
    float xstart=-1.0;
    
    //testing-------------------------------------------------------------------------------------------------------------------
    
    //绘制波形图
    for(vector<float>::iterator it = istart; it != iend; it+=2)    //用迭代器的方式输出容器对象的值
    {
        xstart=xstart+0.016;
        glVertex2f(xstart,0);
        glVertex2f(xstart,*it+0.001);
    }
    
    //进行下一次绘制的起点和终点
    istart+=1111;    //通过更改每次前进的数字可以实现波形振动频率的改变
    iend+=1111;
    
    glEnd();
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    
    fileOutput();
    
    //将数据传入顶点坐标数组 VAO
    float* arr = new float[6*n];

    int i=0;
    float xstart=-1.0;
    for(vector<float>::iterator it = vertices.begin(); it != vertices.end(); it++ )    //用迭代器的方式输出容器对象的值
    {
        arr[i++]=xstart;     //每次频谱线绘制的起点  在x轴上
        arr[i++]=0.0f;
        arr[i++]=0.0f;

        arr[i++]=xstart;     //终点，文件中解析出的音量数据
        arr[i++]=*it;
        arr[i++]=0.0f;

        xstart=xstart+0.002;
        if (xstart>1.0) {
            xstart=-1.0;
        }
    }
    
//    for (int j=0; j<6*n; j++) {
//        cout<<arr[j]<<endl;
//    }
//
    
    GLFWwindow* window;
    
    //初始化库
    if (!glfwInit())
        return -1;
    
    //创建窗口
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Visualize Music ", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("/Users/boone/Desktop/Github/OpenGL/CODE/Test/Test/spectrum.vs", "/Users/boone/Desktop/Github/OpenGL/CODE/Test/Test/spectrum.fs");
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    //循环渲染直至用户关闭窗口
    while (!glfwWindowShouldClose(window))
    {
        //检测按键，判断是否退出
        pressInput(window);
        
        // activate shader
        ourShader.use();
        
        // render container
        glBindVertexArray(VAO);
        
        glDrawArrays(GL_LINES, 0, 2);

        //绘图
//        if (iend<=vertices.end()){
//            // sleep(0.003);
//            drawLine();
//        }
        //交换颜色缓冲
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    //删除分配的所有资源
    glfwTerminate();
    
    return 0;
}

