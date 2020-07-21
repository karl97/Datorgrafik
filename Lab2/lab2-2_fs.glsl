#version 440

out vec4 frag_colour;
in vec4 normal;
in vec4 position;

uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];

vec4 diffuse_brdf() // lambertian diffuse lightning
{
	return vec4(vec3(1.0 / 3.14), 1);
}

float constrain(float value) // constrains values between 0-1
{
	if(value < 0.0)
	{
		return 0.0;
	}
	
	if(value > 1.0)
	{
		return 1.0;
	}

	return value;
}

void main () {

  frag_colour = vec4(0);
  vec4 n = normalize(normal);
  for (int l = 0; l < light_count; ++l )
  {
	vec4 l_in = normalize(light_position[l] - position);
	frag_colour = frag_colour + vec4(1, 1, 0, 1) * light_colour[l] *diffuse_brdf() * vec4(vec3(1 + constrain(dot(l_in, n))), 1);
  }
  frag_colour = frag_colour / light_count;
}
