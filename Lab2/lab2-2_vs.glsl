#version 400

layout(location=0) in vec4 vp; //vertex position
layout(location=1) in vec4 n; //vertex normal
uniform mat4 mvp;
uniform mat4 m; //model matrix
out vec4 position;
out vec4 normal;

void main () {
	
gl_Position = mvp*vp;
position=m * vp;
normal=m * n;

};
  
