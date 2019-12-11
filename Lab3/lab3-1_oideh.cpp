// GL Extension Wrangler
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
#include <lodepng.h>
float g_rotation[2] = { 0,0 };


float t = (1.0f + sqrtf(5.0f))*0.25f;
int num_faces = 20;
int num_verts = 60;

float points[] = {
	// An icosahedron without shared vertices has 20*3 vertices
	-0.5,  t,  0,
	-0.5,  t,  0,
	-0.5,  t,  0,
	-0.5,  t,  0,
	-0.5,  t,  0,

	0.5,  t,  0,
	0.5,  t,  0,
	0.5,  t,  0,
	0.5,  t,  0,
	0.5,  t,  0,

	-0.5, -t,  0,
	-0.5, -t,  0,
	-0.5, -t,  0,
	-0.5, -t,  0,
	-0.5, -t,  0,

	0.5, -t,  0,
	0.5, -t,  0,
	0.5, -t,  0,
	0.5, -t,  0,
	0.5, -t,  0,

	0, -0.5,  t,
	0, -0.5,  t,
	0, -0.5,  t,
	0, -0.5,  t,
	0, -0.5,  t,

	0,  0.5,  t,
	0,  0.5,  t,
	0,  0.5,  t,
	0,  0.5,  t,
	0,  0.5,  t,

	0, -0.5, -t,
	0, -0.5, -t,
	0, -0.5, -t,
	0, -0.5, -t,
	0, -0.5, -t,

	0,  0.5, -t,
	0,  0.5, -t,
	0,  0.5, -t,
	0,  0.5, -t,
	0,  0.5, -t,

	t,  0, -0.5,
	t,  0, -0.5,
	t,  0, -0.5,
	t,  0, -0.5,
	t,  0, -0.5,

	t,  0,  0.5,
	t,  0,  0.5,
	t,  0,  0.5,
	t,  0,  0.5,
	t,  0,  0.5,

	-t,  0, -0.5,
	-t,  0, -0.5,
	-t,  0, -0.5,
	-t,  0, -0.5,
	-t,  0, -0.5,

	-t,  0,  0.5,
	-t,  0,  0.5,
	-t,  0,  0.5,
	-t,  0,  0.5,
	-t,  0,  0.5
};

float uv_coords[] =
{
  0.072405, 0.144686,
  0.27133,  0.10033,
  0.481007, 0.129901,
  0.681948, 0.115116,
  0.928587, 0.107723,
  0.29888,  0.70651,
  0.449421, 0.736084,
  0.108695, 0.129901,
  0.847270, 0.699121,
  0.868776, 0.728691,
  0.576437, 0.721299,
  0.319044, 0.750869,
  0.548883, 0.736084,
  0.388265, 0.115116,
  0.397674, 0.713906,
  0.149690, 0.122508,
  0.347942, 0.115116,
  0.583157, 0.107723,
  0.780738, 0.100331,
  0.229663, 0.107723,
  0.33436,  0.68882,
  0.181276, 0.721299,
  0.379528, 0.721299,
  0.190684, 0.122508,
  0.358023, 0.736084,
  0.103319, 0.713906,
  0.24512,  0.70651,
  0.083157, 0.699121,
  0.31006,  0.10251,
  0.222942, 0.721299,
  0.772001, 0.728691,
  0.618776, 0.721299,
  0.745120, 0.721299,
  0.433292, 0.115116,
  0.598614, 0.713906,
  0.520657, 0.721299,
  0.650362, 0.736084,
  0.700765, 0.728691,
  0.878184, 0.107723,
  0.671867, 0.743476,
  0.914475, 0.743476,
  0.819717, 0.736084,
  0.201437, 0.721299,
  0.633561, 0.122508,
  0.796195, 0.743476,
  0.140953, 0.728691,
  0.122136, 0.706514,
  0.262593, 0.721299,
  0.157754, 0.728691,
  0.825765, 0.115116,
  0.723614, 0.728691,
  0.897001, 0.721299,
  0.499152, 0.736084,
  0.731007, 0.115116,
  0.469582, 0.721299,
  0.045523, 0.691729,
  0.967566, 0.721299,
  0.28341,  0.67157,
  0.532754, 0.122508,
  0.418507, 0.706514,
};

int num_indices = num_faces * 3;
unsigned short faces[] = {
	// ... and 20 triangular faces, 
	// defined by the following vertex indices:
	0,  55, 25,
	1,  26,  5,
	2,  6, 35,
	3,  36, 50,
	4,  51, 56,
	7,  27, 45,
	28, 57, 20,
	58, 52, 10,
	53, 37, 30,
	38,  8, 40,
	15, 46, 21,
	16, 22, 11,
	17, 12, 31,
	18, 32, 41,
	19, 42, 47,
	23, 48, 29,
	13, 24, 59,
	33, 14, 54,
	43, 34, 39,
	49, 44, 9
};

void MUL_4x4(float(*C)[4], const float(*A)[4], const float(*B)[4])
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			C[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				C[i][j] += A[j][k] * B[k][i];
			}
		}

	}
	//computes C = A x B
}

void invertMatrix(float(*C)[4], float(*A)[4])
{
	//computes C = A^(-1) for a transformation matrix
	for (int j = 0; j < 4; j++) {
		for (int k = 0; k < 4; k++) {
			C[j][k] = A[j][k];

		}
	}


	for (int i = 0; i < 3; i++) {
		C[i][3] = -A[i][3];
	}
	//The rotation part can be inverted separately from the translation part
	//and the last row is the same
}


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
		std::string vertex_shader_str = readFile("../Lab3/lab3-1_vs.glsl");
		std::string fragment_shader_str = readFile("../Lab3/lab3-1_fs.glsl");
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


	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Update rotation angle here, for example

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

	if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) { g_rotation[1]++; }
	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) { g_rotation[1]--; }
	if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) { g_rotation[0]++; }
	if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) { g_rotation[0]--; }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
glm::vec4 normals[sizeof(points) / (3 * sizeof(float))];

void normaler() {
	for (int i = 0; i < sizeof(faces) / (sizeof(short) * 3); i++) {
		glm::vec3 vertex[3];
		for (int k = 0; k < 3; k++) {
			vertex[k] = glm::vec3(points[faces[i * 3 + k] * 3], points[faces[i * 3 + k] * 3 + 1], points[faces[i * 3 + k] * 3 + 2]);
		}
		glm::vec3 a = vertex[0] - vertex[1];
		glm::vec3 b = vertex[0] - vertex[2];
		glm::vec3 c = glm::normalize(glm::cross(a, b));
		for (int k = 0; k < 3; k++) {
			normals[faces[i * 3 + k]] = glm::vec4(c, 0);
		}

	}


}

int main(int argc, char const *argv[])
{


	// start GL context and O/S window using the GLFW helper library

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Icosahedron", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int w_height = 600;
	int w_width = 800;

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
	GLuint texture_handle;
	glGenTextures(1, &texture_handle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_handle);	
	unsigned char* image_data;
	unsigned image_w;
	unsigned  image_h;	unsigned  image_file = lodepng_decode32_file(&image_data, &image_w, &image_h, "../../common/data/numberline_hires.png");
	std::cout << "Read " << image_h << " x " << image_w << " image\n";	//(unsigned char** out, unsigned* w, unsigned* h,
	//	const char* filename);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	unsigned int VBO2;

	unsigned int EBO;
	glGenBuffers(1, &VBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	normaler();
	for (int i = 0; i < sizeof(normals) / sizeof(glm::vec4); i++) {

		std::cout << i << ": ";
		std::cout << normals[i].x << " ";
		std::cout << normals[i].y << " ";
		std::cout << normals[i].z << " ";
		std::cout << normals[i].w << "\n";
	};

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-6_vs.glsl" and "../lab1-6_fs.glsl"
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	std::string vertex_shader_str = readFile("../Lab3/lab3-1_vs.glsl");
	std::string fragment_shader_str = readFile("../Lab3/lab3-1_fs.glsl");
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
	// attach and link vertex and fragment shaders into a shader program
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

	float n = 1.0;
	float f = 100.0;
	float a = -(f + n) / (f - n);
	float b = -(2 * f*n) / (f - n);

	float tx = 0;
	float ty = 0;
	float tz = 0;

	const GLchar* mv = "mv";
	const GLchar* mvp = "mvp";

	while (!glfwWindowShouldClose(window))
	{




		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Define the projection matrix, rotation matrices, model matrix, etc. The variable names and code structure is a simple suggestion, you may improve on it!
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//





		glm::mat4 rotx = glm::mat4(1.0f);
		glm::mat4 roty = glm::mat4(1.0f);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		rotx = glm::rotate(rotx, glm::radians(g_rotation[0]), glm::vec3(1.0, 0.0, 0.0));
		roty = glm::rotate(roty, glm::radians(g_rotation[1]), glm::vec3(0.0, 1.0, 0.0));
		modelMatrix = rotx * roty;

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));


		glm::mat4 projectionMatrix;
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 1.0f, 100.0f);

		glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

		glm::mat4 modelViewProjectionMatrix = glm::mat4(1.0f);
		modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Send your modelViewProjection matrix to your vertex shader as a uniform varable

		glUniformMatrix4fv(glGetUniformLocation(shader_program, mv), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, mvp), 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));


		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

		// update other events like input handling 
		glfwPollEvents();

		// clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES,
			sizeof(faces) / sizeof(short),
			GL_UNSIGNED_SHORT,
			0);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


