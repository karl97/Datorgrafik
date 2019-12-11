#version 400

layout(location=0) in vec4 vp;
uniform mat4 matrix;
out vec4 pos;

void main () {
	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Apply the model, view and projection transform to vertex positions and forward the position to the fragment shader using an appropriate "out" variable
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
gl_Position = matrix*vp;
pos=vp+0.5;
;

};
  