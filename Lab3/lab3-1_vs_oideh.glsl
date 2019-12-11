#version 400

layout(location=0) in vec4 vp;
layout(location=1) in vec4 normal;
uniform mat4 mvp;
uniform mat4 mv;
out vec4 pos;
out vec4 n;

void main () {
	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Apply the model, view and projection transform to vertex positions and forward the position to the fragment shader using an appropriate "out" variable
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

gl_Position = mvp*vp;
pos=vp;
n=normal;

};
  
