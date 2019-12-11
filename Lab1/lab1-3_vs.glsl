#version 400
in vec3 vp;
out vec3 pos;

void main () {
  gl_Position = vec4 (vp, 1.0);
  pos=vp+0.5;
 };
  