//
//  main.cpp
//  Soectrum6.0
//
//  Created by boone on 2018/8/20.
//  Copyright © 2018年 boone. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fftw3.h>

#include <learnopengl/shader.h>

using namespace std;

struct wav_struct
{
    unsigned long file_size;        //文件大小
    unsigned short channel;            //通道数
    unsigned long frequency;        //采样频率
    unsigned long Bps;                //Byte率
    unsigned short sample_num_bit;    //一个样本的位数
    unsigned long data_size;        //数据大小
    unsigned char *data;            //音频数据
};

vector<float> vertices;    //用于存储pcm文件解析出的数据
int istart=0;              //记录频谱每次绘制的起点、控制频谱的绘制
int wstart=0;
int pstart=10000;
int n;       //记录pcm文件中数据个数

int NUM=1000;  //一个圆周上分布频谱的个数
float PI=3.1415926f;
float R=0.5f;  //半径

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void fileOutput();
void drawLine();
void drawPoint();
void drawWave();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // 构建并编译着色器程序
    // ------------------------------------
    Shader ourShader("/Users/boone/Desktop/Github/OpenGL/CODE/Spectrum/Spectrum6.0/spectrum.vs", "/Users/boone/Desktop/Github/OpenGL/CODE/Spectrum/Spectrum6.0/spectrum.fs");
    
    //FFTW
    fftw_complex *in,*out;
    fftw_plan p;
    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*n);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*n);
    if (in==NULL||out==NULL) {
        cout<<"ERROR: Fail to memory allocation"<<endl;
    }else{
        int  i=0;
        for(vector<float>::iterator it = vertices.begin(); it != vertices.end(); it+=2 ){
            in[i][0]=*it;
            in[i][1]=0;
            i++;
        }
    }
    p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_cleanup();
    
    //顶点数组
    float* arr = new float[3*n];
    float* arr1 = new float[3*n];
    float* arr2 = new float[3*n/2];
    
    int i=0;
    int j=1000;
    int k=0;
    //直线型频谱图数据存储
    while (j<n&&(i<3*n-6)) {
        
        float temp =sqrt(out[j][0]*out[j][0]+out[j][1]*out[j][1])/30000;
        j++;
        
        arr[i++]=R*cos(2*PI/NUM*k);     //圆上的点
        arr[i++]=R*sin(2*PI/NUM*k);
        arr[i++]=0.0f;
        
        arr[i++]=(R+temp)*cos(2*PI/NUM*k);     //由圆向外延伸的终点，表示频谱
        arr[i++]=(R+temp)*sin(2*PI/NUM*k);
        arr[i++]=0.0f;
        
        k++;
        if (k>NUM) {
            k=0;     //循环存储N个圆形频谱
        }

    }
    
    //离散点频谱图数据存储
    i=0;
    j=1000;
    k=0;
    while (j<n&&(i<3*n-6)) {
        
        float temp =sqrt(out[j][0]*out[j][0]+out[j][1]*out[j][1])/30000;
        j++;
        
        arr1[i++]=(R-temp)*cos(2*PI/NUM*k);     //由圆向外延伸的终点，表示频谱
        arr1[i++]=(R-temp)*sin(2*PI/NUM*k);
        arr1[i++]=0.0f;
        
        arr1[i++]=(R+temp)*cos(2*PI/NUM*k);     //由圆向外延伸的终点，表示频谱
        arr1[i++]=(R+temp)*sin(2*PI/NUM*k);
        arr1[i++]=0.0f;
        
        k++;
        if (k>NUM) {
            k=0;     //循环存储N个圆形频谱
        }
    }
    
    //波形频谱图数据存储
    i=0;
    j=0;
    k=0;
    NUM=1000;
    while (j<n&&(i<3*n/2-6)){
        
        float temp =sqrt(out[j][0]*out[j][0]+out[j][1]*out[j][1])/30000;
        j++;
        
        arr2[i++]=(R-temp)*cos(2*PI/NUM*k);     //由圆向外延伸的终点，表示频谱
        arr2[i++]=(R-temp)*sin(2*PI/NUM*k);
        arr2[i++]=0.0f;
        
        k++;
        if (k>NUM) {
            k=0;     //循环存储N个圆形频谱
        }
        
    }
    
    if (in!=NULL) {
        fftw_free(in);
    }
    if (out!=NULL) {
        fftw_free(out);
    }
    
    //直线型
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, 12*n, arr, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    
    //离散型
    unsigned int pointVBO, pointVAO;
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);
    
    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    
    glBufferData(GL_ARRAY_BUFFER, 12*n, arr1, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    
    
    //波形
    unsigned int waveVAO,waveVBO;
    glGenVertexArrays(1, &waveVAO);
    glGenBuffers(1, &waveVBO);
    
    glBindVertexArray(waveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
    
    glBufferData(GL_ARRAY_BUFFER, 6*n, arr2, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    
    GLint location = glGetUniformLocation("u_hue");
    cout<<location<<endl;
    
    // 循环渲染
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // 渲染
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ourShader.use();
        
//        glBindVertexArray(VAO); // 激活VAO表示的顶点缓存
//        if (istart<3*n) {
//            drawLine();
//        }
        
        glBindVertexArray(pointVAO); // 激活VAO表示的顶点缓存
        if (pstart<3*n) {
            drawPoint();
        }
        
//        glBindVertexArray(waveVAO); // 激活VAO表示的顶点缓存
//        if (wstart<3*n/2) {
//            drawWave();
//        }
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glDeleteVertexArrays(1, &waveVAO);
    glDeleteBuffers(1, &waveVBO);
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小改变时，调用该回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//PCM文件数据解码保存到数组中
void fileOutput()
{
    fstream fs;
    wav_struct WAV;
    
    fs.open("/Users/boone/Desktop/Music/Seve.wav", ios::binary | ios::in);
    
    fs.seekg(0x28);
    fs.read((char*)&WAV.data_size, sizeof(WAV.data_size));
    
    WAV.data = new unsigned char[WAV.data_size];
    
    fs.seekg(0x2c);
    fs.read((char *)WAV.data, sizeof(char)*WAV.data_size);
    
    n=WAV.data_size;
    
    for (unsigned long i =0; i<WAV.data_size; i = i + 2)
    {
        //右边为大端
        unsigned long data_low = WAV.data[i];
        unsigned long data_high = WAV.data[i + 1];
        double data_true = data_high * 256 + data_low;
        long data_complement = 0;
        //取大端的最高位（符号位）
        int my_sign = (int)(data_high / 128);
        
        if (my_sign == 1)
        {
            data_complement = data_true - 65536;
        }
        else
        {
            data_complement = data_true;
        }
        
        double float_data = (double)(data_complement/(double)32768);
        //printf("%f ", float_data);
        vertices.push_back(float_data);
        
    }
    fs.close();
    
    delete[] WAV.data;
}

//绘制频谱
void drawLine()
{
    usleep(99900);   //通过延时实现频谱的显示频率
    
    //颜色随机设置
    float redValue = 0.0f;
    float blueValue = 1.0f;
    
    for (int i=istart; i<2000+istart; i+=2) {
        
        glUniform4f(0, redValue, 1.0f, blueValue, 1.0f);
        
        if (i<=1000+istart) {
            redValue=redValue+0.002;
            blueValue=blueValue-0.002;
        }else{
            redValue=redValue-0.002;
            blueValue=blueValue+0.002;
        }
        
        glDrawArrays(GL_LINES, i, 2);

    }
    
    istart+=2000;
}

//绘制离散型频谱
void drawPoint()
{
    usleep(99999);
    
    for (int i=pstart; i<2000+pstart; i+=2) {
        
        glUniform4f(0, 0.2f, 0.7f, 1.0f, 1.0f);
        
        glPointSize(3);
        glDrawArrays(GL_POINTS, i, 2);
    }
    
    pstart+=2000;
}
//绘制波形频谱
void drawWave()
{
    usleep(99999);
    //颜色随机设置
    float redValue = 0.0f;
    float blueValue = 1.0f;
    
    for (int i=wstart; i<1000+wstart; i++) {
        
        glUniform4f(0, redValue, blueValue, 1.0f, 1.0f);
        glUniform1f(location,200.0f);
        
        if (i<=500+wstart) {
            redValue=redValue+0.002;
            blueValue=blueValue-0.002;
        }else{
            redValue=redValue-0.002;
            blueValue=blueValue+0.002;
        }
        
        glPointSize(4);
        glDrawArrays(GL_POINTS, i, 1);
    }
    
    wstart+=1000;
}
