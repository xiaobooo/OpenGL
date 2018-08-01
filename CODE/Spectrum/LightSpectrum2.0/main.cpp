//
//  main.cpp
//  LightSpectrum2.0
//
//  Created by boone on 2018/8/1.
//  Copyright © 2018年 boone. All rights reserved.
//


#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void drawLine();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(1.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//draw
vector<float> vertices;    //用于存储pcm文件解析出的数据
int istart=0;
int n;       //记录pcm文件中数据个数

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // glad: 加载所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
     // configure global opengl state
     // -----------------------------
     glEnable(GL_DEPTH_TEST);
     
    // 构建并编译着色器程序
    // ------------------------------------
    Shader ourShader("/Users/boone/Desktop/Github/OpenGL/CODE/Spectrum/LightSpectrum2.0/colors.vs", "/Users/boone/Desktop/Github/OpenGL/CODE/Spectrum/LightSpectrum2.0/colors.fs");
    Shader lampShader("/Users/boone/Desktop/Github/OpenGL/CODE/Spectrum/LightSpectrum2.0/lamp.vs", "/Users/boone/Desktop/Github/OpenGL/CODE/Spectrum/LightSpectrum2.0/lamp.fs");
    // 设置顶点数据（和缓冲区）并配置顶点属性
    // ------------------------------------------------------------------
    float* arr = new float[6*n];
    
    int i=0;
    float xstart=-1.0;
    
    for(vector<float>::iterator it = vertices.begin(); it != vertices.end(); it+=2 )    //用迭代器的方式输出容器对象的值
    {
        arr[i++]=xstart;     //圆上的点
        arr[i++]=0.0f;
        arr[i++]=0.0f;
        
        arr[i++]=xstart;     //由圆向外延伸的终点，表示频谱
        arr[i++]=*it;
        arr[i++]=0.0f;
        
        xstart=xstart+0.002;
        if (xstart>1.0) {
            xstart=-1.0;     //循环存储N个圆形频谱
        }
        
    }
     
     float lamp_vertices[] = {
          -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          
          -0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,
          
          -0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
          
          -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f, -0.5f,
          
          -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f, -0.5f,
     };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    // glBindBuffer()有2个参数：target与buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    //    cout<<sizeof(arr)<<endl;   //使用float*简历数组导致下面一行代码不能使用sizeof(arr) 需要手动设置大小 4*数字长度  这里注释的为注释
    // 当缓存初始化之后，使用glBufferData()将顶点数据拷贝到缓存对象
    glBufferData(GL_ARRAY_BUFFER, 24*n, arr, GL_STATIC_DRAW);
    
    //设置顶点属性指针，告诉OpenGL该如何解析顶点数据
    //          顶点属性位置 顶点属性大小 数据的类型 是否被标准化 步长             偏移
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
    glEnableVertexAttribArray(0);
    
    //解绑缓存着色器
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 解绑顶点着色器，绑定和解绑的顺序很重要！！！
    glBindVertexArray(VAO);
    
    // 循环渲染
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        
        // 渲染
        // ------
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ourShader.use();
     //   ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        // world transformation
        glm::mat4 model;
        ourShader.setMat4("model", model);
        
        glBindVertexArray(VAO); // 激活VAO表示的顶点缓存
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
     
     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
          camera.ProcessKeyboard(FORWARD, deltaTime);
     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
          camera.ProcessKeyboard(BACKWARD, deltaTime);
     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
          camera.ProcessKeyboard(LEFT, deltaTime);
     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
          camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: 每当窗口大小改变时，调用该回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //确保视口与新窗口尺寸匹配；注意宽度和
    //高度将显著大于视网膜显示器上指定的高度。
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
     if (firstMouse)
     {
          lastX = xpos;
          lastY = ypos;
          firstMouse = false;
     }
     
     float xoffset = xpos - lastX;
     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
     
     lastX = xpos;
     lastY = ypos;
     
     camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
     camera.ProcessMouseScroll(yoffset);
}

//绘制频谱
void drawLine()
{
    usleep(99900);   //通过延时实现频谱的显示频率
    
    //颜色随机设置
    float redValue = 0.0f;
    float yellowValue = 1.0f;
    
    for (int i=istart; i<2000+istart; i+=2) {
        glUniform4f(0, redValue, 1.0f, yellowValue, 1.0f);
        
        redValue=redValue+0.001;
        yellowValue=yellowValue-0.001;
        
        glDrawArrays(GL_LINES, i, 2);
    }
    
    istart+=2000;
}
