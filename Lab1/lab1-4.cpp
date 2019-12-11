
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

#include <direct.h>

float offy;
float offx;
float modi;

static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
	modi = modi + yoffset*0.1;
	}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Reload shaders
		std::string vertex_shader_str = readFile("../Lab1/lab1-4_vs.glsl");
		std::string fragment_shader_str = readFile("../Lab1/lab1-4_fs.glsl");
		const char *vertex_shader_src = vertex_shader_str.c_str();
		const char *fragment_shader_src = fragment_shader_str.c_str();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader_src, NULL);
		glCompileShader(vs);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader_src, NULL);
		glCompileShader(fs);
		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, fs);
		glAttachShader(shader_program, vs);
		glLinkProgram(shader_program);
		glDeleteShader(vs);
		glDeleteShader(fs);

		glUseProgram(shader_program);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

	}
	if (key == GLFW_KEY_DOWN) { offy=offy-0.01; }
	if (key == GLFW_KEY_UP) { offy=offy+0.01; }
	if (key == GLFW_KEY_RIGHT) { offx=offx+0.01; }
	if (key == GLFW_KEY_LEFT) { offx=offx-0.01; }
	
	

}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char const *argv[])
{
	// start GL context and O/S window using the GLFW helper library

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Set up geometry, VBO, VAO
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

	char* path;

	path = _getcwd(NULL, 0);

	std::cout << path;
	std::string vertex_shader_str = readFile("../Lab1/lab1-4_vs.glsl");
	std::string fragment_shader_str = readFile("../Lab1/lab1-4_fs.glsl");
	const char *vertex_shader_src = vertex_shader_str.c_str();
	const char *fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader_src, NULL);
	glCompileShader(fs);
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(shader_program);

	
	
	const GLchar* mod = "modifier";
	const GLchar* posoff = "position_offset";
	GLfloat off[2];
	GLfloat modi2[1];
	while (!glfwWindowShouldClose(window))
	{
		glfwSetScrollCallback(window, scroll_callback);
		modi2[0] = modi;
		off[0] = offx;
		off[1] = offy;
		
	glUniform2fv(glGetUniformLocation(shader_program, posoff) , 1, off);
	glUniform1fv(glGetUniformLocation(shader_program, mod), 1, modi2);
		// update other events like input handling 
		glfwPollEvents();
		// clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
