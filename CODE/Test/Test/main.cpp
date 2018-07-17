
//
//  main.cpp
//  Waveform3.0
//
//  Created by boone on 2018/7/9.
//  Copyright © 2018年 boone. All rights reserved.
//

/*
#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#define SHAPE_N 160
#define SPACE	0.3f

#include <iostream>
#include <vector>

using namespace std;


template<typename T,
template<typename Elem, typename Allocator = std::allocator<Elem>> class C>
void fileOutput(C<T>& vertices)
{
    short pcm_In = 0;
    int size = 0;
    FILE *fp = fopen(OLD_FILE_PATH, "rb+");     //为读写打开一个二进制文件 即pcm文件
	if (!fp) {
		printf("open file failed!!\n");
		return;
	}
    while(!feof(fp))
    {
        size = fread(&pcm_In, 2, 1, fp);     //pcm中每个数据大小为2字节，每次读取1个数据
        if(size>0)
        {
            //-------------------------------------------------------------------------------------------------------------------------
            vertices.push_back((T)pcm_In/10000);
        }
    }
    
    fclose(fp);
}


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>

static const struct
{
	float x, y,z;
} 
vertices[] =
{
	{ -0.1f, -0.5f ,0.0f	},
	{ 0.1f,  -0.5f ,0.0f	},
	{  0.1f,  0.5f ,0.0f	},
	{ 0.1f,   0.5f , 0.0f	},
	{ -0.1f,   0.5f , 0.0f	},
	{ -0.1f,   -0.5f , 0.0f }
};

static const char* vertex_shader_text =
"#version 330 core\n"
"\n"
"uniform mat4 ortho; \n"
"uniform mat4 world; \n"
"uniform mat4 model; \n"
"\n"
"layout(location = 0) in vec3 vposition; \n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = ortho * world * model * vec4( vposition, 1);\n"
"}\n";

static const char* fragment_shader_text =
"#version 330 core\n"
"\n"
"out vec4 color; \n"
"\n"
"uniform vec4 Ucolor; \n"
"\n"
"void main()\n"
"{\n"
"	color = Ucolor; \n"
"}\n";

static float world_x = 0.0f;
static float world_y = 0.0f;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		--world_y;
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		++world_y;
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		--world_x;
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		++world_x;
}

int main(void)
{
	vector<float> vs;
	vs.reserve(300000);
	fileOutput<float, vector>(vs);
	printf("%d\n", vs.size());

	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	window = glfwCreateWindow(600, 480, "Music Spectrum", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

#ifdef _DEBUG
	printf("program = %u\n", program);
#endif
	GLuint vposition = 0;
	glUseProgram(program);

	GLint ucolor = glGetUniformLocation(program, "Ucolor");
	GLint ortho = glGetUniformLocation(program, "ortho");
	GLint world = glGetUniformLocation(program, "world");
	GLint model = glGetUniformLocation(program, "model");


	glVertexAttribPointer(vposition, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(vposition);

	std::vector<glm::vec3> poss;
	poss.reserve(60);

	bool isGo = true;
	int begin_index = 300000;

	glm::mat4 ortho_mat = glm::ortho(-20.f, 20.f, 20.f, -20.f,-3.f,1000.f);

	glUniformMatrix4fv(ortho , 1, GL_FALSE, glm::value_ptr(ortho_mat));


    
	while (!glfwWindowShouldClose(window))
	{
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vertex_buffer);

		glm::mat4 world_mat;
		world_mat = glm::translate(world_mat, glm::vec3(world_x, world_y, 3.0f));
		glUniformMatrix4fv(world, 1, GL_FALSE, glm::value_ptr(world_mat));

		float begin_x = -20.0f;
		float begin_r = 0.0f;
#ifdef _DEBUG
		printf("%f %f\n", world_x,world_y);
#endif
		glUniform4f(ucolor, begin_r, 1.0f, 1.0f, 1.0f);

		if (begin_index + (SHAPE_N - 1) >= vs.size())
				isGo = false;
		for (int i = 0; i < SHAPE_N; i++)
		{
			glm::mat4 model_mat;
			model_mat = glm::scale(model_mat, glm::vec3(1.0f, 0.1f + vs[begin_index + i] * 10.0f  , 1.0f));
			model_mat = glm::translate(model_mat, glm::vec3(begin_x + i * SPACE, 0.0f, 0.0f));
			//begin_x += SPACE;

			glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(model_mat));
			

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float) / 3);
		}

		if (isGo)
			++begin_index;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER



//
//  main.cpp
//  Waveform3.0
//
//  Created by boone on 2018/7/9.
//  Copyright © 2018年 boone. All rights reserved.
//


#define OLD_FILE_PATH "/Users/boone/Desktop/Music/Seve.pcm"     //PCM源文件

#define SHAPE_N 160
#define SPACE    0.2f

#include <iostream>
#include <vector>

using namespace std;


template<typename T,
template<typename Elem, typename Allocator = std::allocator<Elem>> class C>
void fileOutput(C<T>& vertices)
{
    short pcm_In = 0;
    int size = 0;
    FILE *fp = fopen(OLD_FILE_PATH, "rb+");     //为读写打开一个二进制文件 即pcm文件
    if (!fp) {
        printf("open file failed!!\n");
        return;
    }
    while(!feof(fp))
    {
        size = fread(&pcm_In, 2, 1, fp);     //pcm中每个数据大小为2字节，每次读取1个数据
        if(size>0)
        {
            //-------------------------------------------------------------------------------------------------------------------------
            vertices.push_back((T)pcm_In/10000);
        }
    }
    
    fclose(fp);
}


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>

static const struct
{
    float x, y,z;
}
vertices[] =
{
    { 0.0f, -0.5f ,0.0f },
    { 0.0f, 0.5f ,0.0f }
    /*{ -0.1f, -0.5f ,0.0f    },
     { 0.1f,  -0.5f ,0.0f    },
     {  0.1f,  0.5f ,0.0f    },
     { 0.1f,   0.5f , 0.0f    },
     { -0.1f,   0.5f , 0.0f    },
     { -0.1f,   -0.5f , 0.0f }*/
};

static const char* vertex_shader_text =
"#version 330 core\n"
"\n"
"uniform mat4 ortho; \n"
"uniform mat4 world; \n"
"uniform mat4 model; \n"
"\n"
"layout(location = 0) in vec3 vposition; \n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = ortho * world * model * vec4( vposition, 1);\n"
"}\n";

static const char* fragment_shader_text =
"#version 330 core\n"
"\n"
"out vec4 color; \n"
"\n"
"uniform vec4 Ucolor; \n"
"\n"
"void main()\n"
"{\n"
"    color = Ucolor; \n"
"}\n";

static float world_x = 0.0f;
static float world_y = 0.0f;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        --world_y;
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        ++world_y;
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        --world_x;
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        ++world_x;
}

int main(void)
{
    vector<float> vs;
    vs.reserve(300000);
    fileOutput<float, vector>(vs);
    printf("%d\n", vs.size());
    
    GLFWwindow* window;
    GLuint vertex_buffer,vertex_array, vertex_shader, fragment_shader, program;
    
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    
    window = glfwCreateWindow(600, 480, "example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetKeyCallback(window, key_callback);
    
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    
    // NOTE: OpenGL error checks have been omitted for brevity
    
    
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
#ifdef _DEBUG
    printf("program = %u\n", program);
#endif
    GLuint vposition = 0;
    glUseProgram(program);
    
    GLint ucolor = glGetUniformLocation(program, "Ucolor");
    GLint ortho = glGetUniformLocation(program, "ortho");
    GLint world = glGetUniformLocation(program, "world");
    GLint model = glGetUniformLocation(program, "model");
    
    
    glVertexAttribPointer(vposition, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(vposition);
    
    std::vector<glm::vec3> poss;
    poss.reserve(60);
    
    bool isGo = true;
    int begin_index = 300000;
    
    glm::mat4 ortho_mat = glm::ortho(-20.f, 20.f, 20.f, -20.f,-3.f,1000.f);
    
    glUniformMatrix4fv(ortho , 1, GL_FALSE, glm::value_ptr(ortho_mat));
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        //glBindVertexArray(vertex_buffer);
        
        glm::mat4 world_mat;
        world_mat = glm::translate(world_mat, glm::vec3(world_x, world_y, 3.0f));
        glUniformMatrix4fv(world, 1, GL_FALSE, glm::value_ptr(world_mat));
        
        float begin_x = -20.0f;
        float begin_r = 0.0f;
#ifdef _DEBUG
        printf("%f %f\n", world_x,world_y);
#endif
        glUniform4f(ucolor, begin_r, 1.0f, 1.0f, 1.0f);
        
        if (begin_index + (SHAPE_N - 1) >= vs.size())
            isGo = false;
        for (int i = 0; i < SHAPE_N; i++)
        {
            glm::mat4 model_mat;
            model_mat = glm::scale(model_mat, glm::vec3(1.0f, 0.1f + vs[begin_index + i] * 10.0f  , 1.0f));
            model_mat = glm::translate(model_mat, glm::vec3(begin_x + i * SPACE, 0.0f, 0.0f));
            //begin_x += SPACE;
            
            glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(model_mat));
            
            
            glDrawArrays(GL_LINES, 0, sizeof(vertices) / sizeof(float) / 3);
        }
        
        if (isGo)
            ++begin_index;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
#ifdef _MSC_VER
    //system("pause");
#endif // _MSC_VER
    return 0;
}
