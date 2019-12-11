#version 400
out vec4 frag_colour;
in vec3 pos;

void main () {

  frag_colour = vec4 (pos.x, pos.y, pos.z, 1.0);
}
