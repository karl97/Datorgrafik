#version 400
out vec4 frag_colour;
in vec3 pos;
uniform float modifier=0.0;


void main () {

  frag_colour = vec4 (pos.x+modifier, pos.y+modifier, pos.z+modifier, 1.0);
}
