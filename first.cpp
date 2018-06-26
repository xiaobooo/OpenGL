#include<iostream>
#include <glad/glad.h>
#include <glfw3.h>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)//这个是窗口变化的回调函数。。注意输入参数
																		 //是一个glfw的窗口，一个宽度和高度
{
	glViewport(0, 0, width, height);//这个是回调函数内的内容
									//这里是将视口改成变化后的的窗口大小
									//注意需要的注册该回调函数
									//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
									//两个参数是，glfw的窗口以及回调函数
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//获取按键，如果等于esc 
		glfwSetWindowShouldClose(window, true);//利用强制窗口应该关闭
}
int main()
{
	glfwInit();//类似于之前的gluinit一般采用库都需要进行初始化
			   //版本号是opengl4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//设置主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);//设置次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//在mac系统上需要设置该语句
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);//后面两个参数是设置显示屏和共享的，
																			   //一般库函数的相关定义，
																			   //可以配置好以后直接查看都有详细的定义
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		system("pause");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//在调用opengl函数之前初始化glad，
															//glad的作用就是快速的将opengl函数映射到相关的显卡函数上
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);//设置视口大小
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//注册函数，该函数还可以注册其他相关硬件的回调函数
																	  //
																	  //渲染流程
	while (!glfwWindowShouldClose(window))//检测glfw窗口是否被指令要求关闭，如果是的话，则会退出循环
	{

		//输入的相关操作
		processInput(window);//



							 // 渲染的相关操作

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//查看所有的事件，并且交换内存
		glfwSwapBuffers(window);//交换颜色缓存，即glfw窗口内的内容。。。注意glfw开头的交换和后面gl开头的区别
		glfwPollEvents();//检测是否有其他的触发时间，例如上述的窗口大小变化，需要调用相关的回调函数
	}
	glfwTerminate();//清除退出
	return 0;
}