//Tesselation Evaluation Shader
#version 440

// We work with triangle patches
layout(triangles) in;

// Inputs from TCS
in vec3 tcPosition[];
in vec2 tcTexCoord[];

// Outputs, to Fragment Shader
out vec4 tePosition;
out vec4 teNormal;
out vec2 teTexCoord;
out vec4 tangent;
out vec4 bitangent;

out vec3 col;

// This is our displacement map
uniform sampler2D heightmap;

// We'll finally apply these uniforms now 

uniform mat4 modelViewMatrix;
uniform mat4 mvp;
uniform mat4 m;


vec4 gaussian_sample(const sampler2D tex, const vec2 uv, const float radius, const vec2 a, const vec2 b)
{
  //sampling is done along the uv directions, with attention to scale,
  //for non-square textures.
  vec2 ts = vec2(length(a), length(b))*radius;
  
  return	(1.0/4.0)  * texture(tex,uv) +
    (1.0/8.0)  * texture(tex,uv + a*ts.x) +
    (1.0/8.0)  * texture(tex,uv - a*ts.x) +
    (1.0/8.0)  * texture(tex,uv + b*ts.y) +
    (1.0/8.0)  * texture(tex,uv - b*ts.y) +
    (1.0/16.0) * texture(tex,uv + a*ts.x + b*ts.y) +
    (1.0/16.0) * texture(tex,uv + a*ts.x - b*ts.y) +
    (1.0/16.0) * texture(tex,uv - a*ts.x + b*ts.y) +
    (1.0/16.0) * texture(tex,uv - a*ts.x - b*ts.y);
}

//const float sample_offset = 0.02;
const float displacement_coef = -0.1;

void main()
{

  // 1. Compute interpolated Position and TexCoord using gl_TessCoord
  // with tcPosition and tcTexCoord, respectively.

	vec4 tmpPosition = vec4(gl_TessCoord.x * tcPosition[0] + gl_TessCoord.y * tcPosition[1] + gl_TessCoord.z * tcPosition[2], 1.0);
    
	teTexCoord = gl_TessCoord.x * tcTexCoord[0] + gl_TessCoord.y * tcTexCoord[1] + gl_TessCoord.z * tcTexCoord[2];


  // 2. Compute the normal for the triangle that connects tcPosition[]
	vec4 normal = vec4(normalize(cross(tcPosition[1]-tcPosition[0], tcPosition[2] - tcPosition[0])), 0.0);
	teNormal = m*normal;

  // 3. Compute the amount of displacement for the vertex by using the
  // sampler2D heightmap
  //vec4  gaussian_sample(heightmap, teTexCoord, const float radius, const vec2 a, const vec2 b)
  float d = displacement_coef * texture(heightmap,teTexCoord).x;
   tmpPosition += d*normal; 

  // 4a. Select 2 other texture coordinates and compute the
  // displacement at those points.

  
  // 4b. Use the three displaced points to compute a new normal, and
  // put it in teNormal;



  // 5. Now apply transformations  
  gl_Position = mvp * tmpPosition;
  tePosition = m*tmpPosition;
  col = gl_TessCoord;
}
