#version 440

out vec4 frag_colour;
in vec4 normal;
in vec4 position;

uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];

uniform vec4 eyepos;


vec4 diffuse_brdf(){
	return vec4(vec3(1.0 / 3.14), 1);
}

float constrain(float value)
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
float Xpos(float value)
{
	if(value <= 0.0)
	{
		return 0.0;
	}
	
	if(value > 0)
	{
		return 1.0;
	}

}

vec4 BP_brdf(float kl, float kg, float f, vec4 indir, vec4 normal, vec4 outdir){

	vec4 h=normalize(outdir+indir);

	return vec4(vec3(kl/3.14) + vec3(((8+f)/(8*3.14)))*kg*pow(dot(normal,h),f), 1);
}
/*
float f0(float index_of_refraction){
	float a=(index_of_refraction-1)/(index_of_refraction+1);

	return (a*a);
}*/

float F(vec4 normal,vec4 indir, float f0){

	float f=f0+(1-f0)*pow((1-constrain(dot(normal,indir))),5);
	return f;
}

float D(vec4 h,vec4 n ,float alpha){

float a=Xpos(dot(n,h))/(3.14*pow(alpha,2)*pow(dot(n,h),4));
float b=pow(dot(n,h),2)-1;
float c=pow(alpha,2)*pow(dot(n,h),2);

return a*exp(b/c);

}

float CT_brdf(vec4 indir, vec4 normal, vec4 outdir, float alpha,float f0){

	vec4 h=normalize(outdir+indir);
	float a=(2*dot(normal,h)*dot(normal,outdir))/dot(outdir,h);
	float b=(2*dot(normal,h)*dot(normal,indir))/dot(outdir,h);
	float G=min(min(1.0 , a) , b);
	
	float c=(4*abs(dot(normal,indir))*abs(dot(normal,outdir)));

	float Fs=(F(h,indir,f0)*G*D(h,normal,alpha)/c);

	return Fs;
	//return vec4(vec3(Fs),1);
}



void main () {

  frag_colour = vec4(0.5, 0.3, 0, 1.0);
  
  vec4 n = normalize(normal);
  vec4 outdir=normalize(position-eyepos);
  for (int l = 0; l < light_count; ++l )
  {
	vec4 l_in = normalize(light_position[l] - position);
	//frag_colour = frag_colour + light_colour[l] * BP_brdf(0.1, 0.9, 10, l_in, n, outdir) * constrain(dot(l_in, n));
	//frag_colour = frag_colour + light_colour[l] * BP_brdf(0.1, 0.9, 1000, l_in, n, outdir) * constrain(dot(l_in, n));
	//frag_colour = frag_colour + light_colour[l] * BP_brdf(1, 0, 10, l_in, n, outdir) * constrain(dot(l_in, n));
	frag_colour = frag_colour + light_colour[l] * 0.3 *diffuse_brdf() * constrain(dot(l_in, n));
	frag_colour = frag_colour + light_colour[l]*0.7* vec4(CT_brdf(l_in,n,outdir,0.025,1.0), CT_brdf(l_in,n,outdir,0.025,0.782), CT_brdf(l_in,n,outdir,0.025,0.344), 1) * constrain(dot(l_in, n));
 }
  
}
