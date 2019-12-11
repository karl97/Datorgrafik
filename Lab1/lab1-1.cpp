// GL Extension Wrangler
//https://www.opengl.org/sdk/libs/GLEW/
//What does GLEW do for you? What would it look like if i did its job manually?
//https://www.opengl.org/wiki/Load_OpenGL_Functions
#include <GL/glew.h>

// http://www.glfw.org/faq.html#what-is-glfw
//OpenGL doesn't have a way of creating window, this does it in an OS-independent way
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>

static void error_callback(int error, const char* description)
{
  std::cerr << description;
}

int main(int argc, char const *argv[])
{
  // start GL context and O/S window using the GLFW helper library
  
  glfwSetErrorCallback(error_callback);
  if( !glfwInit() )
    exit(EXIT_FAILURE);
  GLFWwindow* window = glfwCreateWindow (200, 200, "Hello OpenGL", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent (window);
                                  
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit ();
  
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  //Query renderer and version using glGetString;
  const GLubyte *renderer= glGetString(GL_RENDERER);
  const GLubyte *ver=glGetString(GL_VERSION);
  std::cout << "Renderer:" << std::endl << renderer<< std::endl;
  std::cout << "OpenGL version supported:" << std::endl << ver << std::endl;
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  while (!glfwWindowShouldClose(window)) {
	  glfwPollEvents();
  }


  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
