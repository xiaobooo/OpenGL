//
//  main.cpp
//  Spectrum2.0
//
//  Created by boone on 2018/8/6.
//  Copyright © 2018年 boone. All rights reserved.
//

#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

vector<float> vertices;    //用于存储pcm文件解析出的数据
int istart=0;
int n;       //记录pcm文件中数据个数

int NUM=1000;  //一个圆周上分布频谱的个数
float PI=3.1415926f;
float R=0.3f;  //半径

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
            vertices.push_back((float)pcm_In/30000);
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Music Circular Spectrum", NULL, NULL);
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
    Shader ourShader("", "");
    
    // 设置顶点数据（和缓冲区）并配置顶点属性
    // ------------------------------------------------------------------
    float* arr = new float[6*n];
    
    int i=0;
    int j=0;
    float xstart=-1.0;
    
    for(vector<float>::iterator it = vertices.begin(); it != vertices.end(); it+=2 )    //用迭代器的方式输出容器对象的值
    {
        arr[i++]=xstart;
        arr[i++]=0.0f;
        arr[i++]=0.0f;
        
        arr[i++]=xstart;
        arr[i++]=*it;
        arr[i++]=0.0f;
        
        xstart=xstart+0.002;
        if (xstart>1.0) {
            xstart=-1.0;
            
        }
    }
    
    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    // glGenBuffers() 创建一个缓存对象并且返回缓存对象的标示符。
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, 24*n, arr, GL_STATIC_DRAW);
    //          顶点属性位置 顶点属性大小 数据的类型 是否被标准化 步长             偏移
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // 渲染
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 频谱图绘制
        ourShader.use();    //启用着色器程序
        
        glBindVertexArray(VAO); // 激活VAO表示的顶点缓存
        
        drawLine();
       
        
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
    
    GLfloat ratio=(GLfloat)width/(GLfloat)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(width<=height)//保持形状不变
        glOrtho(-15.0,15.0,-15.0/ratio,15.0/ratio,0.0,0.0);
    else
        glOrtho(-15.0*ratio,15.0*ratio,-15.0,15.0,0.0,0.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//绘制频谱
void drawLine()
{
    usleep(99900);   //通过延时实现频谱的显示频率
    
    //颜色随机设置
    float redValue = 0.0f;
    float yellowValue = 1.0f;
    
    istart=770000;
    for (int i=istart; i<2000+istart; i=i+2) {
        glUniform4f(0, redValue, 1.0f, yellowValue, 1.0f);
        
        if (i<=1000+istart) {
            redValue=redValue+0.002;
            yellowValue=yellowValue-0.002;
        }else{
            redValue=redValue-0.002;
            yellowValue=yellowValue+0.002;
        }
        glDrawArrays(GL_LINES, i, 2);
    }
    
    istart+=1000;
}


