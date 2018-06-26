#include<iostream>
#include<glad/glad.h>
#include<glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{

	//实例化一个窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	//窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称（标题）
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//设置视口大小 前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
	glViewport(0, 0, 800, 600);

	//注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//渲染循环(Render Loop)，能在我们让GLFW退出前一直保持运行
	while (!glfwWindowShouldClose(window))
	//函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话
		//该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序
	{
		processInput(window); 
		//用户按下了ESC，我们将通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW。
		//下一次while循环的条件检测将会失败，程序将会关闭。

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空屏幕的颜色缓冲


		glfwSwapBuffers(window);//函数检查有没有触发什么事件、更新窗口状态，并调用对应的回调函数
		glfwPollEvents();//函数会交换颜色缓冲）,在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
	}
	glfwTerminate();//确释放/删除之前的分配的所有资源

	return 0;
}