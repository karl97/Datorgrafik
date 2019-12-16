// Copy from lab2-1.cpp, and just replace the shader program.
// Replace with your own code, starting from what you had in Lab 1,
// but use the geometry defined below.
// GL Extension Wrangler



#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

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
#include <lodepng.h>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

#include "config.h"

#if !USE_OBJ
float points[] = {
-0.500000, 0.809017, 0.000000,
-0.500000, 0.809017, 0.000000,
-0.500000, 0.809017, 0.000000,
-0.500000, 0.809017, 0.000000,
-0.500000, 0.809017, 0.000000,
0.500000, 0.809017, 0.000000,
0.500000, 0.809017, 0.000000,
0.500000, 0.809017, 0.000000,
0.500000, 0.809017, 0.000000,
0.500000, 0.809017, 0.000000,
-0.500000, -0.809017, 0.000000,
-0.500000, -0.809017, 0.000000,
-0.500000, -0.809017, 0.000000,
-0.500000, -0.809017, 0.000000,
-0.500000, -0.809017, 0.000000,
0.500000, -0.809017, 0.000000,
0.500000, -0.809017, 0.000000,
0.500000, -0.809017, 0.000000,
0.500000, -0.809017, 0.000000,
0.500000, -0.809017, 0.000000,
0.000000, -0.500000, 0.809017,
0.000000, -0.500000, 0.809017,
0.000000, -0.500000, 0.809017,
0.000000, -0.500000, 0.809017,
0.000000, -0.500000, 0.809017,
0.000000, 0.500000, 0.809017,
0.000000, 0.500000, 0.809017,
0.000000, 0.500000, 0.809017,
0.000000, 0.500000, 0.809017,
0.000000, 0.500000, 0.809017,
0.000000, -0.500000, -0.809017,
0.000000, -0.500000, -0.809017,
0.000000, -0.500000, -0.809017,
0.000000, -0.500000, -0.809017,
0.000000, -0.500000, -0.809017,
0.000000, 0.500000, -0.809017,
0.000000, 0.500000, -0.809017,
0.000000, 0.500000, -0.809017,
0.000000, 0.500000, -0.809017,
0.000000, 0.500000, -0.809017,
0.809017, 0.000000, -0.500000,
0.809017, 0.000000, -0.500000,
0.809017, 0.000000, -0.500000,
0.809017, 0.000000, -0.500000,
0.809017, 0.000000, -0.500000,
0.809017, 0.000000, 0.500000,
0.809017, 0.000000, 0.500000,
0.809017, 0.000000, 0.500000,
0.809017, 0.000000, 0.500000,
0.809017, 0.000000, 0.500000,
-0.809017, 0.000000, -0.500000,
-0.809017, 0.000000, -0.500000,
-0.809017, 0.000000, -0.500000,
-0.809017, 0.000000, -0.500000,
-0.809017, 0.000000, -0.500000,
-0.809017, 0.000000, 0.500000,
-0.809017, 0.000000, 0.500000,
-0.809017, 0.000000, 0.500000,
-0.809017, 0.000000, 0.500000,
-0.809017, 0.000000, 0.500000,
};
float uv_coords[] = {
0.072405, 0.144686,
0.271330, 0.100330,
0.481007, 0.129901,
0.681948, 0.115116,
0.928587, 0.107723,
0.298880, 0.706510,
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
0.334360, 0.688820,
0.181276, 0.721299,
0.379528, 0.721299,
0.190684, 0.122508,
0.358023, 0.736084,
0.103319, 0.713906,
0.245120, 0.706510,
0.083157, 0.699121,
0.310060, 0.102510,
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
0.283410, 0.671570,
0.532754, 0.122508,
0.418507, 0.706514,
};
float normals[] = {
-0.500000, 0.500000, 0.500000, 0.000000,
-0.000000, 0.809017, 0.309017, 0.000000,
0.000000, 0.809017, -0.309017, 0.000000,
-0.500000, 0.500000, -0.500000, 0.000000,
-0.809017, 0.309017, 0.000000, 0.000000,
-0.000000, 0.809017, 0.309017, 0.000000,
0.000000, 0.809017, -0.309017, 0.000000,
0.500000, 0.500000, 0.500000, 0.000000,
0.500000, 0.500000, -0.500000, 0.000000,
0.809017, 0.309017, 0.000000, 0.000000,
-0.809017, -0.309017, -0.000000, 0.000000,
0.000000, -0.809017, 0.309017, 0.000000,
-0.000000, -0.809017, -0.309017, 0.000000,
-0.500000, -0.500000, 0.500000, 0.000000,
-0.500000, -0.500000, -0.500000, 0.000000,
0.500000, -0.500000, 0.500000, 0.000000,
0.000000, -0.809017, 0.309017, 0.000000,
-0.000000, -0.809017, -0.309017, 0.000000,
0.500000, -0.500000, -0.500000, 0.000000,
0.809017, -0.309017, 0.000000, 0.000000,
-0.309017, 0.000000, 0.809017, 0.000000,
0.500000, -0.500000, 0.500000, 0.000000,
0.000000, -0.809017, 0.309017, 0.000000,
0.309017, -0.000000, 0.809017, 0.000000,
-0.500000, -0.500000, 0.500000, 0.000000,
-0.500000, 0.500000, 0.500000, 0.000000,
-0.000000, 0.809017, 0.309017, 0.000000,
0.500000, 0.500000, 0.500000, 0.000000,
-0.309017, 0.000000, 0.809017, 0.000000,
0.309017, -0.000000, 0.809017, 0.000000,
-0.309017, 0.000000, -0.809017, 0.000000,
-0.000000, -0.809017, -0.309017, 0.000000,
0.500000, -0.500000, -0.500000, 0.000000,
-0.500000, -0.500000, -0.500000, 0.000000,
0.309017, 0.000000, -0.809017, 0.000000,
0.000000, 0.809017, -0.309017, 0.000000,
-0.500000, 0.500000, -0.500000, 0.000000,
-0.309017, 0.000000, -0.809017, 0.000000,
0.500000, 0.500000, -0.500000, 0.000000,
0.309017, 0.000000, -0.809017, 0.000000,
0.500000, 0.500000, -0.500000, 0.000000,
0.500000, -0.500000, -0.500000, 0.000000,
0.809017, -0.309017, 0.000000, 0.000000,
0.309017, 0.000000, -0.809017, 0.000000,
0.809017, 0.309017, 0.000000, 0.000000,
0.500000, 0.500000, 0.500000, 0.000000,
0.500000, -0.500000, 0.500000, 0.000000,
0.809017, -0.309017, 0.000000, 0.000000,
0.309017, -0.000000, 0.809017, 0.000000,
0.809017, 0.309017, 0.000000, 0.000000,
-0.500000, 0.500000, -0.500000, 0.000000,
-0.809017, 0.309017, 0.000000, 0.000000,
-0.809017, -0.309017, -0.000000, 0.000000,
-0.309017, 0.000000, -0.809017, 0.000000,
-0.500000, -0.500000, -0.500000, 0.000000,
-0.500000, 0.500000, 0.500000, 0.000000,
-0.809017, 0.309017, 0.000000, 0.000000,
-0.309017, 0.000000, 0.809017, 0.000000,
-0.809017, -0.309017, -0.000000, 0.000000,
-0.500000, -0.500000, 0.500000, 0.000000,
};
unsigned int faces[] = {
0, 55, 25,
1, 26, 5,
2, 6, 35,
3, 36, 50,
4, 51, 56,
7, 27, 45,
28, 57, 20,
58, 52, 10,
53, 37, 30,
38, 8, 40,
15, 46, 21,
16, 22, 11,
17, 12, 31,
18, 32, 41,
19, 42, 47,
23, 48, 29,
13, 24, 59,
33, 14, 54,
43, 34, 39,
49, 44, 9,
};
#else
#endif

float g_rotation[2] = { 0,0 };
int w_height = 600;
int w_width = 800;

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

void calcTangentspace(unsigned int* faces, int numFaces, float* points, float* uvs, float* tangent,float* bitangent) {
	int temp = 1;
	for (int i = 0; i < numFaces; i++) {
		glm::vec3 pos[3];
		glm::vec2 uv[3];

		std::cout << "Face " << i << "\n";
		for (int k = 0; k < 3; k++) {
			int index = faces[i * 3 + k];
			pos[k] = glm::vec3(points[index * 3], points[index * 3 + 1], points[index * 3 + 2]);
			uv[k] = glm::vec2(uvs[index * 2], uvs[index * 2 + 1]);
			std::cout << "["<< index << "] ";
		}
		std::cout << "\n";
		glm::vec3 e1 = pos[1] - pos[0];
		glm::vec3 e2 = pos[2] - pos[0];
		glm::vec2 f1 = uv[1] - uv[0];
		glm::vec2 f2 = uv[2] - uv[0];
		double dx = (double)(f1.x)*(double)(f2.y);
		double dy = (double)(f2.x)*(double)(f1.y);
		float r = dx - dy;
		float f = 1.0f / (f1.x*f2.y - f2.x*f1.y);
		

		int x = 0;
		std::cout << "pos1: (" << pos[x].x << ", " << pos[x].y << ", " << pos[x].z << ") \n";
		std::cout << "uv1: (" << uv[x].x << ", " << uv[x].y << ") \n";
x = 1;
		std::cout << "pos2: (" << pos[x].x << ", " << pos[x].y << ", " << pos[x].z << ") \n";
		std::cout << "uv2: (" << uv[x].x << ", " << uv[x].y << ") \n";
x = 2;
		std::cout << "pos3: (" << pos[x].x << ", " << pos[x].y << ", " << pos[x].z << ") \n";
		std::cout << "uv3: (" << uv[x].x << ", " << uv[x].y << ") \n";
		std::cout << "e1: (" << e1.x << ", " << e1.y << ", " << e1.z << ") \n";
		std::cout << "e2: (" << e2.x << ", " << e2.y << ", " << e2.z << ") \n";
		std::cout << "f1: (" << f1.x << ", " << f1.y << ") \n";
		std::cout << "f2: (" << f2.x << ", " << f2.y << ") \n";
		std::cout << "dx: " << dx << " \n";
		std::cout << "dy: " << dy << " \n";
		std::cout << "r: " << r << " \n";
		std::cout << "f: " << f << " \n";

		glm::vec3 t = (f2.y*e1 - f1.y*e2) / r;
		glm::vec3 b = (f1.x*e2 - f2.x*e1) / r;
		for (int k = 0; k < 3; k++) {
			int index = faces[i * 3 + k];
			tangent[index * 3 + 0] = t.x;
			tangent[index * 3 + 1] = t.y;
			tangent[index * 3 + 2] = t.z;

			bitangent[index * 3 + 0] = b.x;
			bitangent[index * 3 + 1] = b.y;
			bitangent[index * 3 + 2] = b.z;


			std::cout << "Tangent: (" <<t.x <<", " << t.y << ", " << t.z << ") \n";
			std::cout << "Bitangent: (" <<b.x <<", " << b.y << ", " << b.z << ") \n";
		}

		std::cout << "\n";
	}


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
		std::string vertex_shader_str = readFile("../Lab3/lab3-3_vs.glsl");
		std::string fragment_shader_str = readFile("../Lab3/lab3-3_fs.glsl");
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
	//glfwSetKeyCallback(window, key_callback);
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
	// Textures...
	unsigned char* image_data;
	unsigned image_w;
	unsigned image_h;
	unsigned image_file;

	//Diffuse texture
	GLuint texture_handle;
	glGenTextures(1, &texture_handle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	image_file =
		lodepng_decode32_file(&image_data, &image_w, &image_h,
			"../common/data/numberline_hires.png");
	std::cout << "Read " << image_h << " x " << image_w << " image\n";

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	//normal map
	GLuint texture_normal;
	glGenTextures(1, &texture_normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	image_file =
		lodepng_decode32_file(&image_data, &image_w, &image_h,
			"../common/data/numberline_nmap_hires.png");
	std::cout << "Read " << image_h << " x " << image_w << " image\n";

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);



	//Cube map
	GLuint texture_environment;
	glGenTextures(1, &texture_environment);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_environment);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

#if USE_TEST_ENV
	const char* cubeMapSides[6] = {
		"../common/data/cube-test_010.png",
		"../common/data/cube-test_011.png",
		"../common/data/cube-test_100.png",
		"../common/data/cube-test_101.png",
		"../common/data/cube-test_110.png",
		"../common/data/cube-test_111.png",
	};
#else
	const char* cubeMapSides[6] = {
		"../common/data/cube_right.png",
		"../common/data/cube_left.png",
		"../common/data/cube_up.png",
		"../common/data/cube_down.png",
		"../common/data/cube_front.png",
		"../common/data/cube_back.png",
	};
#endif

	for (int i = 0; i < 6; i++)
	{
		image_file = lodepng_decode32_file(&image_data, &image_w, &image_h,
			cubeMapSides[i]);
		std::cout << "Read " << image_h << " x " << image_w << " image\n";
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	}


#if !USE_OBJ


	unsigned int VAO_bunny;
	glGenVertexArrays(1, &VAO_bunny);
	glBindVertexArray(VAO_bunny);

	unsigned int VBO_position;
	unsigned int VBO_normal;
	unsigned int VBO_uv;

	unsigned int EBO_bunny;


	glGenBuffers(1, &VBO_position);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &VBO_normal);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), uv_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &EBO_bunny);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_bunny);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	float tangent[sizeof(points) / sizeof(float)];
	float bitangent[sizeof(points) / sizeof(float)];

	std::cout << "Faces: " << sizeof(faces) / sizeof(unsigned int) / 3 << " Size of tangent: " << sizeof(tangent) << " Array size: " << sizeof(points) / sizeof(float) << "\n";

	calcTangentspace(faces, sizeof(faces) / sizeof(unsigned int) / 3, points, uv_coords, tangent, bitangent);
	
	GLuint VBO_tan, VBO_bitan;

	glGenBuffers(1, &VBO_tan);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tan);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangent), tangent, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &VBO_bitan);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bitan);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bitangent), bitangent, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(4);

#else
	//-----------------------------------------------------------------------------------------------
	//Bunny----------------------------
	//-----------------------------------------------------------------------------------------------

	std::string inputfile = OBJ_FILE;
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

	unsigned int VBO_position;
	unsigned int VBO_normal;
	unsigned int VBO_uv;

	unsigned int EBO_bunny;


	glGenBuffers(1, &VBO_position);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &VBO_normal);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), &shapes[0].mesh.normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.texcoords.size() * sizeof(float), &shapes[0].mesh.texcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &EBO_bunny);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_bunny);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(unsigned int), &shapes[0].mesh.indices[0], GL_STATIC_DRAW);



	float* tangent = new float[shapes[0].mesh.positions.size()];
	float* bitangent = new float[shapes[0].mesh.positions.size()];
	calcTangentspace(&shapes[0].mesh.indices[0], shapes[0].mesh.indices.size() / 3, &shapes[0].mesh.positions[0], &shapes[0].mesh.texcoords[0], tangent, bitangent);

	GLuint VBO_tan, VBO_bitan;

	glGenBuffers(1, &VBO_tan);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tan);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), tangent, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &VBO_bitan);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bitan);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), bitangent, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(4);

#endif

	

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-6_vs.glsl" and "../lab1-6_fs.glsl"
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	std::string vertex_shader_str = readFile("../Lab3/lab3-3_vs.glsl");
	std::string fragment_shader_str = readFile("../Lab3/lab3-3_fs.glsl");
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
	const GLchar* tex = "texture_diffuse";
	const GLchar* texEnv = "texture_environment";
	const GLchar* texn = "texture_normal";
	const GLchar* refl = "r";


	int light_count = 4;
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
	{ 1.0, 1.0, 1.0, 1.0 };


	float offset_value = 0.5f;
	float alpha = 0.2;
	glm::vec4 f0 = { 1.0, 1.0, 1.0, 1.0 };
	float kl_var = 3;
	float kg_var = 0;
	float f_var = 10;
	glm::vec4 bunny_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float slider_scale = 0.5;
	float reflect_ratio = 0.5;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	ImGui::StyleColorsDark();

	//bool show_demo_window = true;
	//bool show_another_window = false;


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

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Cook-Torrance");                           // Display some text (you can use a format string too)
			ImGui::InputFloat("alpha", &alpha);
			ImGui::ColorEdit3("f0", (float*)&f0);
			ImGui::SliderFloat("specular-lambertian", &slider_scale, 0.0f, 1.0f);

			ImGui::Text("Blinn-phong");
			ImGui::InputFloat("kl", &kl_var);
			ImGui::InputFloat("kg", &kg_var);
			ImGui::InputFloat("f", &f_var);



			//ImGui::SliderFloat("alpha", &alpha, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    

			ImGui::Text("Bunny_modifier");
			ImGui::SliderFloat("horizontal", &g_rotation[1], -180.0f, 180.0f);
			ImGui::SliderFloat("vertical", &g_rotation[0], -180.0f, 180.0f);
			ImGui::ColorEdit3("diffuse colour", (float*)&bunny_color); // Edit 3 floats representing a color
			ImGui::InputInt("Light sources", &light_count, 0, 4);


			for (int i = 0; i < light_count; i++)
			{
				std::string lightText = std::string("Position light ") + std::to_string(i);

				ImGui::InputFloat3(lightText.c_str(), &light_position[i][0]);
				lightText = std::string("light_strength") + std::to_string(i);
				ImGui::SliderFloat(lightText.c_str(), &light_strength[i], 0.5f, 10.0f);

				lightText = std::string("Color light ") + std::to_string(i);
				ImGui::ColorEdit3(lightText.c_str(), &light_colour[i][0]);
			}


			ImGui::Text("Other");
			ImGui::SliderFloat("Reflect amount", &reflect_ratio, 0.0f, 1.0f);
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			//ImGui::Checkbox("Another Window", &show_another_window);

			//if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				//counter++;
			//ImGui::SameLine();
			//ImGui::Text("counter = %d", counter);

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
		glUniform1f(glGetUniformLocation(shader_program, refl), (float)reflect_ratio);
		glUniform4fv(glGetUniformLocation(shader_program, bc), 1, (GLfloat*)&bunny_color[0]);
		glUniform4fv(glGetUniformLocation(shader_program, f0v), 1, (GLfloat*)&f0[0]);
		glUniform1f(glGetUniformLocation(shader_program, klv), (float)kl_var);
		glUniform1f(glGetUniformLocation(shader_program, kgv), (float)kg_var);
		glUniform1f(glGetUniformLocation(shader_program, fv), (float)f_var);
		glUniform1f(glGetUniformLocation(shader_program, ss), (float)slider_scale);
		glUniform4fv(glGetUniformLocation(shader_program, ls), 1, (GLfloat*)&light_strength[0]);
		glUniform1i(glGetUniformLocation(shader_program, tex), 0);
		glUniform1i(glGetUniformLocation(shader_program, texEnv), 1);
		glUniform1i(glGetUniformLocation(shader_program, texn), 2);



#if !USE_OBJ
		{
			glUniform1f(glGetUniformLocation(shader_program, offset), -offset_value);
			glDrawElements(GL_TRIANGLES,
				sizeof(faces) / sizeof(unsigned int),
				GL_UNSIGNED_INT,
				0);
		}


		{

			glUniform1f(glGetUniformLocation(shader_program, offset), +offset_value);
			glDrawElements(GL_TRIANGLES,
				sizeof(faces) / sizeof(unsigned int),
				GL_UNSIGNED_INT,
				0);
		}


#else
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
#endif

		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//


		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.



		//-----------------------------------------------------------------------------------------------------------------------------------------------------------//



		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}








