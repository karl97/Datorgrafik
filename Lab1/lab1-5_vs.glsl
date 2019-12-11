#version 400
in vec3 vp;
out float zval;

void main () {
  gl_Position = vec4 (vec3(vp), 1.0);
zval=vp.z;
};
  