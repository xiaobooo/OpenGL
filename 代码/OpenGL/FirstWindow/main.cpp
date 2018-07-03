//
//  main.cpp
//  FirstWindow
//
//  Created by boone on 2018/7/3.
//  Copyright © 2018年 boone. All rights reserved.
//

#include<GLFW/glfw3.h>

#include<iostream>

using namespace std;

const unsigned int WID =700;

const unsigned int HEI = 700;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void pressInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
}

int main(){
    //初始化窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //此行用以兼容Mac OS X系统
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    
    //创建窗口对象
    GLFWwindow * win =glfwCreateWindow(WID, HEI, "Xiaobooo's test windows!", NULL, NULL);
    if(win == NULL){
        cout<<"Fail to Create the windows"<<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);
    
    //设置回调函数，当窗口改变大小时调用
    glfwSetFramebufferSizeCallback(win, frame_buffer_size_callback);
    
    //渲染循环，退出前一直保持运行
    while (!glfwWindowShouldClose(win)) {
        
        //检测按键，判断是否退出
        pressInput(win);
        
        //设置渲染背景色
        glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(win);   //交换颜色缓冲
        glfwPollEvents();   //检查触发事件
        
    }
    //正确释放分配的所有资源
    glfwTerminate();
    
    return 0;
}

