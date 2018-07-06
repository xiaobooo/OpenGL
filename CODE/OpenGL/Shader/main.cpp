//
//  main.cpp
//  Shader
//
//  Created by boone on 2018/7/3.
//  Copyright © 2018年 boone. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

// windows settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

//顶点着色器
const char *vertexShaderSource ="#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n" // 向片段着色器输出一个颜色

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"    // 将ourColor设置为我们从顶点数据那里得到的输入颜色
"}\0";

//片段着色器
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor,1.0f);\n"
"}\n\0";

void pressInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS) {
        glfwSetWindowShouldClose(window , true);
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glad_glViewport(0,0,width,height);
}

int main(){
    //窗口初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    //兼容 Mac OS X
    
    //创建窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Xiaobooo's Shader", NULL, NULL);
    if(window == NULL){
        cout<<"ERROR: Fail to Create Window!"<<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //glad用于OpenGL中的指针
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<<"ERROR: Fail to Initialize GLAD"<<endl;
        return -1;
    }
    
    //创建并编译顶点着色器
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    
    //检查编译是否成功
    int succees;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&succees);
    if(!succees){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        cout<<"ERROR: Fail to Compile Shader!"<<infoLog<<endl;
        
    }
    
    //创建并编译片段着色器
    
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&succees);
    if (!succees) {
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        cout<<"ERROR: Fail to Compile Fragment Shader!"<<infoLog<<endl;
    }
    //链接着色器并检查
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&succees);
    if(!succees){
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        cout<<"ERROR: Fail to Link ShaderProgram!"<<infoLog<<endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //设置顶点数据
    float verties[]={
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verties),verties,GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);
    
    while (!glfwWindowShouldClose(window)) {
        
        pressInput(window);
        
        glClearColor(0.8f,0.8f,0.8f,0.8f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //随着时间变化每次更新渲染新的颜色
       /* float timeValue = glfwGetTime();
        float colorValue = sin(timeValue)/2.0f+0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        glUniform4f(vertexColorLocation,0.6f,colorValue,0.8f,1.0f);*/
        
        //画出三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    glDeleteVertexArrays(1,&VAO);
    glad_glDeleteBuffers(1,&VBO);
    
    glfwTerminate();
    return 0;
}
