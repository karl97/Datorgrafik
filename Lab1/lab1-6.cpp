  // GL Extension Wrangler
#include <GL/glew.h>
#include <cmath>
// http://www.glfw.org/faq.html#what-is-glfw
//OpenGL doesn't have a way of creating window, this does it in an OS-independent way
#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// You can store the rotation angles here, for example
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
float g_rotation[2] = {0,0};


void MUL_4x4 (float (*C)[4], const float (*A)[4], const float (*B)[4])
{
	for (int i = 0; i<4; i++) {
		for (int j=0; j < 4; j++) {
			C[i][j] = 0;
			for (int k=0; k < 4;k++) {
				C[i][j] += A[j][k] * B[k][i];
			}
		}

	}
  //computes C = A x B
}

void invertMatrix (float (*C)[4], float (*A)[4])
{
  //computes C = A^(-1) for a transformation matrix
	for (int j = 0; j < 4;j++) {
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
 
  if(isCompiled == GL_FALSE)
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
		std::string vertex_shader_str = readFile("../Lab1/lab1-6_vs.glsl");
		std::string fragment_shader_str = readFile("../Lab1/lab1-6_fs.glsl");
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

  if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {g_rotation[0]++;}
  if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )  {g_rotation[0]--; }
  if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )   {g_rotation[1]++; }
  if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ) {g_rotation[1]--; }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


int main(int argc, char const *argv[])
{
  // start GL context and O/S window using the GLFW helper library
  
  glfwSetErrorCallback(error_callback);
  if( !glfwInit() )
    exit(EXIT_FAILURE);
  
  GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Icosahedron", NULL, NULL);
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
  int w_height = 600;
  int w_width = 800;

  if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
	}
  glfwMakeContextCurrent (window);
                                  
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit ();

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Set up geometry, VBO, EBO, VAO
  float points[] = {
	 -0.5f, -0.5f, -0.5f,
	 -0.5f, -0.5f, +0.5f,
	 -0.5f, +0.5f, -0.5f,
	 -0.5f, +0.5f, +0.5f,

	 +0.5f, -0.5f, -0.5f,
	 +0.5f, -0.5f, +0.5f,
	 +0.5f, +0.5f, -0.5f,
	 +0.5f, +0.5f, +0.5f,
  };
  
  
  unsigned short faces[] = {
		0, 2, 6,
		0, 6, 4,

		0, 3, 2,
		0, 1, 3,

		2, 3, 7,
		2, 7, 6,

		6, 7, 5,
		6, 5, 4,

		3, 1, 5,
		3, 5, 7,

		1, 0, 4,
		1, 4, 5
  };


  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  unsigned int EBO;
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// load and compile shaders  "../lab1-6_vs.glsl" and "../lab1-6_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  std::string vertex_shader_str = readFile("../Lab1/lab1-6_vs.glsl");
  std::string fragment_shader_str = readFile("../Lab1/lab1-6_fs.glsl");
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
 
  float n=1.0;
  float f=100.0;
  float a=-(f+n)/(f-n);
  float b=-(2*f*n) / (f - n);
  
  float tx = 0;
  float ty = 0;
  float tz = 0;

  const GLchar* mat = "matrix";
  
  while (!glfwWindowShouldClose(window))
  {
	 

	 
	  
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // Define the projection matrix, rotation matrices, model matrix, etc. The variable names and code structure is a simple suggestion, you may improve on it!
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//


   
	GLfloat rotate_x[4][4] = {  {1, 0, 0,tx },
								{0, cos(g_rotation[0]),-sin(g_rotation[0]),ty},
								{0, sin(g_rotation[0]),cos(g_rotation[0]),tz}, 
								{0, 0, 0, 1} };
    GLfloat rotate_y[4][4]= { {cos(g_rotation[1]), 0, sin(g_rotation[1]),tx },
								{0, 1, 0, ty},
								{-sin(g_rotation[1]), 0, cos(g_rotation[1]),tz},
								{0, 0, 0, 1} };
    
	GLfloat modelMatrix[4][4] = {};
    MUL_4x4(modelMatrix, rotate_x, rotate_y);
    
    GLfloat viewMatrix[4][4]= { {1, 0, 0, 0},
								{0, 1, 0, 0},
								{0, 0, 1, 2},
								{0, 0, 0, 1}};
	GLfloat inverseViewMatrix[4][4] = {};
    invertMatrix(inverseViewMatrix, viewMatrix);
    
    GLfloat modelViewMatrix[4][4];
    MUL_4x4(modelViewMatrix, inverseViewMatrix, modelMatrix);
    
	GLfloat projectionMatrix[4][4] = {  {9.0/16.0, 0, 0, 0},
										{0, 1, 0, 0},
										{0, 0, a, b},
										{0, 0, -1, 0} };

    GLfloat modelViewProjectionMatrix[4][4];
    MUL_4x4(modelViewProjectionMatrix,modelViewMatrix,projectionMatrix);
    
    
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // Send your modelViewProjection matrix to your vertex shader as a uniform varable
	
	glUniformMatrix4fv(glGetUniformLocation(shader_program, mat), 1, GL_TRUE, (GLfloat*)modelViewProjectionMatrix);

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    
    // update other events like input handling 
    glfwPollEvents ();
    
    // clear the drawing surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // Issue an appropriate glDraw*() command.
	glDrawElements(GL_TRIANGLES,
		sizeof(faces),
		GL_UNSIGNED_SHORT,
		0);

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//

    glfwSwapBuffers (window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

