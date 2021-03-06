// Copy from lab2-1.cpp, and just replace the shader program.
// Replace with your own code, starting from what you had in Lab 1,
// but use the geometry defined below.
// GL Extension Wrangler
#define USE_BUNNY 1


#include "IMGUI_GLFW/imgui.h"
#include "IMGUI_GLFW/imgui_impl_glfw_gl3.h"

#include <GL/glew.h>
#include <cmath>
// http://www.glfw.org/faq.html#what-is-glfw
//OpenGL doesn't have a way of creating window, this does it in an OS-independent way
#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include "windows_users/glm/glm/glm.hpp"
#include "windows_users/glm/glm/gtc/matrix_transform.hpp"
#include "windows_users/glm/glm/gtc/type_ptr.hpp"

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"


float g_rotation[2] = { 0,0 };
int w_height = 600;
int w_width = 800;

void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << "shader compilation failed:" << std::endl;
		std::cout << errorLog << std::endl;
		return;
	}
	else
		std::cout << "shader compilation success." << std::endl;

	return;
}

static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Reload shaders
		std::string vertex_shader_str = readFile("../Lab2/lab2-3_vs.glsl");
		std::string fragment_shader_str = readFile("../Lab2/lab2-3_2-4_fs.glsl");
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
		
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	w_width = width;
	w_height = height;
}

int main(int argc, char const *argv[])
{


	// start GL context and O/S window using the GLFW helper library

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Icosahedron", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  // Set up geometry, VBO, EBO, VAO

	//-----------------------------------------------------------------------------------------------
	//Bunny----------------------------
	//------------------------------------------------------------------------
#if USE_BUNNY
	std::string inputfile = "../common/data/bunny.obj";
#else
	std::string inputfile = "../common/data/dice (1).obj";
#endif
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	int flags = 0; // see load_flags_t enum for more information.

	// Load the object
	bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), 0, flags);

	// Check for errors
	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}
	else
	{
		std::cout << "Loaded " << inputfile
			<< " ("
			<< shapes.size() << " shapes"
			<< ")"
			<< "\n";
		if (shapes.size() > 0)
		{
			std::cout << shapes[0].mesh.positions.size() << " points "
				<< "(" << shapes[0].mesh.positions.size() * sizeof(float) << " bytes), "
				<< shapes[0].mesh.indices.size() << " indices "
				<< "(" << shapes[0].mesh.indices.size() * sizeof(unsigned int) << " bytes), "
				<< shapes[0].mesh.normals.size() << " normals.\n"
				;
		}
	}



	unsigned int VAO_bunny;
	glGenVertexArrays(1, &VAO_bunny);
	glBindVertexArray(VAO_bunny);

	unsigned int VBO_bunny;
	unsigned int VBO2_bunny;

	unsigned int EBO_bunny;


	glGenBuffers(1, &VBO_bunny);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bunny);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &VBO2_bunny);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2_bunny);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), &shapes[0].mesh.normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &EBO_bunny);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_bunny);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size()* sizeof(unsigned int), &shapes[0].mesh.indices[0], GL_STATIC_DRAW);






	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// load and compile shaders
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	std::string vertex_shader_str = readFile("../Lab2/lab2-3_vs.glsl");
	std::string fragment_shader_str = readFile("../Lab2/lab2-3_2-4_fs.glsl");
	const char *vertex_shader_src = vertex_shader_str.c_str();
	const char *fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);
	checkShaderCompileError(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader_src, NULL);
	glCompileShader(fs);
	checkShaderCompileError(fs);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(shader_program);
	

	//Uniforms
	float n = 1.0;
	float f = 100.0;
	float a = -(f + n) / (f - n);
	float b = -(2 * f*n) / (f - n);

	float tx = 0;
	float ty = 0;
	float tz = 0;

	const GLchar* m = "m";
	const GLchar* mvp = "mvp";
	const GLchar* lc = "light_count";
	const GLchar* lp = "light_position";
	const GLchar* lcol = "light_colour";
	const GLchar* eye = "eyepos";
	const GLchar* wp = "window_width";
	const GLchar* av = "alpha_var";
	const GLchar* bc = "bunny_colour";
	const GLchar* f0v = "f0_var";
	const GLchar* klv = "kl_var";
	const GLchar* kgv = "kg_var";
	const GLchar* fv = "f_var";
	const GLchar* ss = "slider_scale";
	const GLchar* offset = "offset_value";
	const GLchar* ls = "light_strength";

	int light_count = 1;
	glm::vec4 light_colour[4] = {
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		
	};;
	glm::vec4 light_position[4] = {
		{0.0, 0.0, 100.0, 1.0},
		{-100.0, 0.0, 0.0, 1.0},
		{0.0, -100.0, 0.0, 1.0},
		{0.0, 100.0, 0.0, 1.0},

	};


	glm::vec4 light_strength = 
		{3.0, 1.0, 1.0, 1.0};


	float offset_value = 0.5f;
	float alpha = 0.2;
	glm::vec4 f0 = { 1.0, 1.0, 1.0, 1.0 };
	float kl_var = 1;
	float kg_var = 0;
	float f_var = 10;
	glm::vec4 bunny_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float slider_scale=1.0;


	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	ImGui::StyleColorsDark();

	glfwSwapInterval(1);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		// update other events like input handling 
		glfwPollEvents();
		
		// clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Imgui
		ImGui_ImplGlfwGL3_NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Cook-Torrance");                           // Display some text (you can use a format string too)
			ImGui::InputFloat("alpha", &alpha);
			ImGui::ColorEdit3("f0", (float*)&f0);
			ImGui::SliderFloat("Specular-lambertian", &slider_scale, 0.0f, 1.0f);

			
			ImGui::Text("Blinn-phong");
			ImGui::InputFloat("kl", &kl_var);
			ImGui::InputFloat("kg", &kg_var);
			ImGui::InputFloat("f", &f_var);

			ImGui::Text("Bunny_modifier");
			ImGui::SliderFloat("horizontal", &g_rotation[1], -180.0f, 180.0f);
			ImGui::SliderFloat("vertical", &g_rotation[0], -180.0f, 180.0f);
			ImGui::ColorEdit3("diffuse colour", (float*)&bunny_color); // Edit 3 floats representing a color
			ImGui::InputInt("Light sources", &light_count, 0, 4);
			

			for (int i = 0; i < light_count; i++)
			{
				std::string lightText = std::string("Position light ") + std::to_string(i);
				ImGui::InputFloat3(lightText.c_str(), &light_position[i][0]);

				lightText = std::string("light_strength ") + std::to_string(i);
				ImGui::SliderFloat(lightText.c_str(), &light_strength[i], 0.5f, 10.0f);

				lightText = std::string("Color light ") + std::to_string(i);
				ImGui::ColorEdit3(lightText.c_str(), &light_colour[i][0]);
				
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}



		//Our stuff
		glUseProgram(shader_program);
		glm::mat4 rotx = glm::mat4(1.0f);
		glm::mat4 roty = glm::mat4(1.0f);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
#if USE_BUNNY
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, -0.25, 0.0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5));
#else
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
#endif
		rotx = glm::rotate(rotx, glm::radians(g_rotation[0]), glm::vec3(1.0, 0.0, 0.0));
		roty = glm::rotate(roty, glm::radians(g_rotation[1]), glm::vec3(0.0, 1.0, 0.0));
		modelMatrix = rotx * roty*modelMatrix;

		glm::mat4 offsetMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-offset_value, 0.0, 0.0));
		glm::mat4 modelMatrix_left = offsetMatrix * modelMatrix;

		offsetMatrix = glm::translate(glm::mat4(1.0), glm::vec3(+offset_value, 0.0, 0.0));
		glm::mat4 modelMatrix_right = offsetMatrix * modelMatrix;
		
		glm::vec4 eyepos = glm::vec4(0.0f, 0.0f, -2.0f, 0.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::translate(viewMatrix, glm::vec3(eyepos));


		glm::mat4 projectionMatrix;
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w_width / (float)w_height, 1.0f, 100.0f);

		glm::mat4 modelViewMatrix = glm::mat4(1.0);

		glm::mat4 modelViewProjectionMatrix = glm::mat4(1.0f);

		modelViewMatrix = viewMatrix * modelMatrix;
		modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
		
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Send your modelViewProjection matrix to your vertex shader as a uniform varable
		glUniformMatrix4fv(glGetUniformLocation(shader_program, m), 1, GL_FALSE, glm::value_ptr(modelMatrix_left));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, mvp), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

		glUniform1i(glGetUniformLocation(shader_program, lc), light_count);
		glUniform4fv(glGetUniformLocation(shader_program, lp), 4, (GLfloat*)light_position);
		glUniform4fv(glGetUniformLocation(shader_program, lcol), 4, (GLfloat*)light_colour);
		glUniform4fv(glGetUniformLocation(shader_program, eye), 1, (GLfloat*)&eyepos[0]);
		glUniform1f(glGetUniformLocation(shader_program, wp), (float)w_width);
		glUniform1f(glGetUniformLocation(shader_program, av), (float)alpha);
		glUniform4fv(glGetUniformLocation(shader_program, bc), 1, (GLfloat*)&bunny_color[0]);
		glUniform4fv(glGetUniformLocation(shader_program, f0v), 1, (GLfloat*)&f0[0]);
		glUniform1f(glGetUniformLocation(shader_program, klv), (float)kl_var);
		glUniform1f(glGetUniformLocation(shader_program, kgv), (float)kg_var);
		glUniform1f(glGetUniformLocation(shader_program, fv), (float)f_var);
		glUniform1f(glGetUniformLocation(shader_program, ss), (float)slider_scale);
		glUniform4fv(glGetUniformLocation(shader_program, ls), 1, (GLfloat*)&light_strength[0]);


		//Draw bunny
		glBindVertexArray(VAO_bunny);

		{
			glUniform1f(glGetUniformLocation(shader_program, offset), -offset_value);
			glDrawElements(GL_TRIANGLES,
				shapes[0].mesh.indices.size(),
				GL_UNSIGNED_INT,
				0);
		}


		{
			
			glUniform1f(glGetUniformLocation(shader_program, offset), +offset_value);
			glDrawElements(GL_TRIANGLES,
				shapes[0].mesh.indices.size(),
				GL_UNSIGNED_INT,
				0);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}








