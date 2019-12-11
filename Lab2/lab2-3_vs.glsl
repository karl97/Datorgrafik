#version 400

layout(location=0) in vec4 vp; //vertex position
layout(location=1) in vec4 n; //vertex normal
uniform mat4 mvp;
uniform mat4 m; //model matrix
uniform float offset_value;
out vec4 position;
out vec4 normal;

void main () {
	
vec4 pos = (mvp*vp);
pos = pos / pos.w;
gl_Position = pos + vec4(offset_value, vec3(0));
position=m * vp;
normal=m * n;

};
  
