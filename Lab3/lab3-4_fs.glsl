#version 440

out vec4 frag_colour;
in vec4 teNormal;
in vec4 tePosition;
in vec2 teTexCoord;
in vec4 tangent;
in vec4 bitangent;

uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];

uniform vec4 eyepos;

uniform float window_width;
uniform float alpha_var;
uniform vec4 bunny_colour;
uniform vec4 f0_var;
uniform float kl_var;
uniform float kg_var;
uniform float f_var;
uniform float slider_scale;
uniform vec4 light_strength;
uniform float r;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform samplerCube texture_environment;

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

float Xpos(float value) //Has two states 0 or 1
{
	if(value <= 0.0)
	{
		return 0.0;
	}
	
	if(value > 0.0)
	{
		return 1.0;
	}

}

vec4 BP_brdf(float kl, float kg, float f, vec4 indir, vec4 normal, vec4 outdir, vec4 diffuse_colour) // the Blinn-Phong brdf 
{

	vec4 h=normalize(outdir+indir);

	return vec4(vec3(kl/3.14)*diffuse_colour.xyz + vec3(((8+f)/(8*3.14)))*kg*pow(dot(normal,h),f), 1);
}


float F(vec4 normal, vec4 indir, float f0) // The Fresnel reflectance
{
	return f0 + (1.0-f0) * pow((1.0 - constrain(dot(normal, indir))), 5);
}

float G(vec4 n, vec4 indir, vec4 outdir, vec4 h) // geometrical attenuation function
{
	float a=2.0*dot(n,h)*dot(n,outdir)/dot(h,outdir);
	float b=2.0*dot(n,h)*dot(n,indir)/dot(h,outdir);

	return min(min(1.0 ,a),b);
}

float D(vec4 n, vec4 m, float alpha) // the normal distrubution function
{
	float a=Xpos(dot(n, m)) / (3.14 * pow(alpha, 2) * pow(dot(n, m), 4));
	float b=(pow(dot(n,m), 2) - 1.0) / (pow(alpha, 2) * pow(dot(n, m), 2));
	return a*exp(b);
}

float fs(vec4 normal, vec4 indir, vec4 outdir, float f0, float alpha) // fspecular brdf term
{
	vec4 h = normalize(outdir + indir);
	float a = F(normal, indir, f0) * G(normal, indir, outdir, h) * D(normal, h, alpha);
	float b = 4.0 * abs(dot(normal, indir)) * abs(dot(normal, outdir));
	return a / b;
}


vec4 CT_brdf(vec4 normal, vec4 indir, vec4 outdir, vec4 f0, float alpha, float lval, float fval, vec4 diffuse_colour) //Cook-Torrance brdf
{
	vec4 fspec = vec4(fs(normal, indir, outdir, f0[0], alpha), fs(normal, indir, outdir, f0[1], alpha), fs(normal, indir, outdir, f0[2], alpha), 1.0);
	vec4 lambert = diffuse_colour*diffuse_brdf();
	return lval * lambert + fval * fspec;

}




void main () {
	frag_colour = vec4(1.0, 0.0, 1.0, 1.0);

	return;
  vec4 diffuse_colour = bunny_colour;
  vec4 tex = texture(texture_diffuse, teTexCoord);
  frag_colour = vec4(0);
  vec4 f0 = f0_var; 
  float alpha = alpha_var; // alpha parameter for D function
  vec4 n = normalize(teNormal);
  n.xy = n.xy * -1.0;


  vec4 tan = vec4(1, 0,0,0);
  vec4 bitan = vec4(0, 1,0,0);
  
  vec4 texn=normalize(vec4(texture(texture_normal, teTexCoord).xyz * 2 - 1, 0));
  mat4 TBN = mat4(normalize(tan), normalize(bitan), n, vec4(0,0,0,1));  n = normalize(TBN * texn);
  
  vec4 outdir = normalize(tePosition - eyepos);
  vec3 refl = reflect(outdir.xyz, n.xyz);
  vec4 texEnv = texture(texture_environment, refl);
  for (int l = 0; l < light_count; ++l )
  {
		vec4 l_in = normalize(light_position[l] - tePosition);
		if(gl_FragCoord.x > window_width/2)
		{
			frag_colour = frag_colour + light_strength[l] * light_colour[l] * BP_brdf(kl_var, kg_var, f_var, l_in, n, outdir, tex * diffuse_colour) * constrain(dot(l_in, n));
		}else
		{
			frag_colour = frag_colour + light_strength[l] * light_colour[l] * CT_brdf(n, l_in, outdir, f0, alpha, slider_scale, 1-slider_scale, tex * diffuse_colour)* constrain(dot(l_in, n));
		}
	
	}
	frag_colour = r * texEnv + (1.0 - r) * frag_colour;
	frag_colour = vec4((n.xyz+ 1.0) /2.0, 1.0);
}
