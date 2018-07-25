//
//  test2.cpp
//  Test
//
//  Created by boone on 2018/7/18.
//  Copyright © 2018年 boone. All rights reserved.
//
#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

vector<float> vertices;    //用于存储pcm文件解析出的数据
int istart=0;
int n;       //记录pcm文件中数据个数

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawLine();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

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

int main()
{
    fileOutput();
    
    // glfw: 初始化并配置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // ！！！for OS X   没有此行报错为 not support version 330
#endif
    
    // glfw 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glad: 家在所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    
    // 构建并编译着色器程序
    // ------------------------------------
    Shader ourShader("/Users/boone/Desktop/Github/OpenGL/CODE/Test/Spectrum4.0/spectrum.vs", "/Users/boone/Desktop/Github/OpenGL/CODE/Test/Spectrum4.0/spectrum.fs");
    
    // 设置顶点数据（和缓冲区）并配置顶点属性
    // ------------------------------------------------------------------
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

        xstart=xstart+0.004;
        if (xstart>1.0) {
            xstart=-1.0;
        }
    }
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    //first 绑定顶点数组对象 second 设置顶点缓冲区 third 配置顶点属性
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    cout<<sizeof(arr)<<endl;   //使用float*简历数组导致下面一行代码不能使用sizeof(arr) 需要手动设置大小 4*数字长度  这里注释的为注释
    glBufferData(GL_ARRAY_BUFFER, 24*n, arr, GL_STATIC_DRAW);
    
    //告诉OpenGL该如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // 调用到GL_ARRAY_BUFFER注册的VBO作为顶点属性的绑定顶点缓冲对象，这样我们就可以安全地解除绑定。
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
    
    // 取消注释此调用以线框多边形绘制。
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // 循环渲染
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // 渲染
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 频谱图绘制
        //-------
        ourShader.use();
        // 更新uniform颜色
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 1.0f;
        glUniform4f(0, 0.0f, greenValue, 0.0f, 1.0f);
        
        glBindVertexArray(VAO); // VAO只需绑定一次即可
        
        if (istart<6*n) {   //到达终点之前每次绘制一帧的频谱图
             drawLine();
        }
        
        // glBindVertexArray(0); // 不需要每次都解除绑定
        
        // glfw: 交换缓冲区和轮询IO事件（按下/释放键，鼠标移动等）
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // 绘制完成后释放资源
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // glfw: 清除先前分配的所有GLFW资源
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// glfw:对相应的按键作出相应的响应
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小改变时，调用该回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //确保视口与新窗口尺寸匹配；注意宽度和
    //高度将显著大于视网膜显示器上指定的高度。
    glViewport(0, 0, width, height);
}
//绘制频谱
void drawLine()
{
    usleep(44100);   //通过延时实现频谱的显示频率
    for (int i=istart; i<1000+istart; i=i+2) {
        glDrawArrays(GL_LINES, i, 2);
    }
    istart+=1000;
}
