#version 400
in vec3 vp;
out vec3 pos;
uniform vec2 position_offset=vec2(0.0,0.0);

void main () {

  gl_Position = vec4 (vp.x+position_offset.x,vp.y+position_offset.y,vp.z, 1.0);
  pos=vp;
 };
  
