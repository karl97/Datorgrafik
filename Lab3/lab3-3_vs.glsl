#version 400

layout(location=0) in vec4 vp; //vertex position
layout(location=1) in vec4 n; //vertex normal
layout(location=2) in vec2 uv; //UV coordinates
layout(location=3) in vec3 tan; //tangent
layout(location=4) in vec3 bitan; //bitangent
uniform mat4 mvp;
uniform mat4 m; //model matrix
uniform float offset_value;
out vec4 position;
out vec4 normal;
out vec2 uv_coords;
out vec4 tangent;
out vec4 bitangent;

void main () {
	
vec4 pos = (mvp*vp);
pos = pos / pos.w;
gl_Position = pos + vec4(offset_value, vec3(0));
position=m * vp;
normal=m * vec4(n.xyz, 0);
uv_coords = uv;
tangent = m * vec4(tan, 0);
bitangent = m * vec4(bitan, 0);
};
  
