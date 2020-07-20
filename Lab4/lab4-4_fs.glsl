#version 400
out vec4 o_fragment_color;

uniform sampler2D i_texture;
uniform bool i_display;
uniform uint i_frame_count;

uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform vec3 i_light_position;
uniform vec3 i_light_color;
uniform float i_focal_dist;

#define NUM_SPHERES 5
#define MAX_DEPTH 10
#define MAX_SAMPLES 10
#define PI 3.1415

struct Ray { vec3 origin, dir; float weight;};

//-------------------------------------------------------------------------//
// Keep a global stack of rays for recursion

Ray ray_stack[MAX_DEPTH];
int ray_stack_size = 0;

void push( Ray ray )
{
  if (ray_stack_size < MAX_DEPTH)
  {
    ray_stack[ray_stack_size] = ray;
    ray_stack_size++;
  }
  //else stack overflow -- silently ignore
}

Ray pop() // This is included for completeness, but actually isn't needed.
{
  if (ray_stack_size > 0)
  {
    ray_stack_size--;
    return ray_stack[ray_stack_size];
  }
  // else stack underflow -- silently ignore
}

//-------------------------------------------------------------------------//

struct Material{
  vec3 color_emission;
  vec3 color_diffuse;
  vec3 color_glossy;
  float roughness;
  float reflection;
  float transmission;
  float ior;  
};

struct Intersection
{
	vec3 point;
	vec3 normal;
	Material material;
};

struct Sphere {
  float radius;
  vec3 center;
  Material material;
};

struct Plane {
  float offset;
  vec3 normal;
  Material material;
};

struct Scene {

  Sphere spheres[NUM_SPHERES];
  Plane ground_plane[1];
  float sun_brightness;

};







Scene scene;


void init( )
{  
  scene.spheres[0].center = vec3(0, 0.3, 0.5) ; 
  scene.spheres[0].radius = 0.3;
  scene.spheres[0].material.color_diffuse = vec3( 0.3, 0.1, 0.1 );
  scene.spheres[0].material.color_glossy = vec3( 1 );
  scene.spheres[0].material.color_emission = vec3( 0 );
  scene.spheres[0].material.roughness = 100;
  scene.spheres[0].material.reflection = 0.3;
  scene.spheres[0].material.transmission = 0;
  scene.spheres[0].material.ior = 1;
  
  scene.spheres[1].center = vec3(0.8, 0.3, 0.8) ;
  scene.spheres[1].radius = 0.3;
  scene.spheres[1].material.color_diffuse = 0.5 * vec3( 0.0, 1.0, 0.0 );
  scene.spheres[1].material.color_glossy = vec3( 1 );
  scene.spheres[1].material.roughness = 10;
  scene.spheres[1].material.color_emission = vec3( 0 );
  scene.spheres[1].material.reflection = 0.15;
  scene.spheres[1].material.transmission = 0.8;
  scene.spheres[1].material.ior = 1.352;

  scene.spheres[2].center = vec3(0.55, 0.1, 0.2) ;
  scene.spheres[2].radius = 0.1;
  scene.spheres[2].material.color_diffuse = vec3( 0.0, 0.0, 0.0 );
  scene.spheres[2].material.color_glossy = vec3( 1 );
  scene.spheres[2].material.roughness = 1000;
  scene.spheres[2].material.color_emission = vec3( 3.6, 0, 0 );
  scene.spheres[2].material.reflection = 0.0;
  scene.spheres[2].material.transmission = 0;
  scene.spheres[2].material.ior = 1;

  scene.spheres[3].center = vec3(0.7, 0.8, -0.5) ;
  scene.spheres[3].radius = 0.8;
  scene.spheres[3].material.color_diffuse = 0.5 * vec3( 0.2, 0.2, 0.15 );
  scene.spheres[3].material.color_glossy = vec3( 1 );
  scene.spheres[3].material.roughness = 10;
  scene.spheres[3].material.color_emission = vec3( 0 );
  scene.spheres[3].material.reflection = 0.0;
  scene.spheres[3].material.transmission = 0;
  scene.spheres[3].material.ior = 1;

  scene.spheres[4].center = vec3(-0.65, 0.6, -1) ;
  scene.spheres[4].radius = 0.6;
  scene.spheres[4].material.color_diffuse = 0.5 * vec3( 0.5, 0.4, 0.25 );
  scene.spheres[4].material.color_glossy = vec3( 1 );
  scene.spheres[4].material.roughness = 5000;
  scene.spheres[4].material.color_emission = vec3( 0 );
  scene.spheres[4].material.reflection = 0.0;
  scene.spheres[4].material.transmission = 0;
  scene.spheres[4].material.ior = 1;

  scene.ground_plane[0].normal = vec3(0,1,0);
  scene.ground_plane[0].offset = 0;
  scene.ground_plane[0].material.color_diffuse = 1.0 * vec3( 0.6 );
  scene.ground_plane[0].material.color_glossy = vec3( 0 );
  scene.ground_plane[0].material.roughness = 0;
  scene.ground_plane[0].material.color_emission = vec3( 0 );
  scene.ground_plane[0].material.reflection = 1.0;
  scene.ground_plane[0].material.transmission = 0;
  scene.ground_plane[0].material.ior = 1;
}


// This function computes a nice-looking sky sphere, with a sun.
vec3 simple_sky(vec3 direction)
{
  float emission_sky = 1e-1 * i_light_color.r;
  float emission_sun = 10.0 * i_light_color.r*i_light_color.r;
  vec3 sky_color = vec3(0.35, 0.65, 0.85);
  vec3 haze_color = vec3(0.8, 0.85, 0.9);
  vec3 light_color = clamp(i_light_color,0,1);

  float sun_spread = 2500.0;
  float haze_spread = 1.3;
  float elevation = acos(direction.y);
    
  float angle = abs(dot(direction, normalize(i_light_position)));
  float response_sun = pow(angle, sun_spread);
  float response_haze = pow(elevation, haze_spread);

  vec3 sun_component = mix(emission_sky*sky_color, emission_sun*light_color,response_sun);
  vec3 haze_component = mix(vec3(0),  emission_sky*haze_color,response_haze);

  return (sun_component+haze_component);
}


// Ray-sphere intersection
float intersect(Ray ray, Sphere s) 
{
  //COPY YOUR CODE FROM 4.1 HERE
  float a = dot(ray.dir, ray.dir);
	vec3 v = ray.origin - s.center;
	float b = 2 * dot(ray.dir, v);
	float c = dot(v, v) - s.radius*s.radius;
	float x = sqrt(b*b - 4 * a * c);
	
	float t1 = (- b - x) / 2 * a;
	float t2 = (- b + x) / 2 * a;

	float t = t1;
	if(t2 < t1) t = t2;

	return t;
}

// Ray-plane intersection
float intersect(Ray ray, Plane p) 
{
  //COPY YOUR CODE FROM 4.1 HERE
  float t=(p.offset - dot(p.normal,ray.origin))/dot(p.normal,ray.dir);

	return t;
}

// Check for intersection of a ray and all objects in the scene
Intersection intersect( Ray ray)
{
  Intersection I;

  //COPY YOUR CODE FROM 4.1 HERE
  // Manage intersections
	float t = 1e32;
	int id = -1;
    
	//Check for intersection with spheres 
	// Find the closest intersection.
	for (int i = 0; i < NUM_SPHERES; ++i)
	{
		float d = intersect(ray, scene.spheres[i]);
		if (d>0 && d<=t)
		{
			t = d; 
			id = i;
		}
	}

	// YOUR CODE GOES HERE -------------------------------------------------------------------------------------
	// Populate I with all the relevant data.  `id` is the closest
	// sphere that was hit, and `t` is the distance to it.
	if(id != -1)
	{
		I.point = ray.origin + t * ray.dir;
		I.normal = normalize(I.point - scene.spheres[id].center);
		I.material = scene.spheres[id].material;
	}


	//Check for intersection with planes
	{
		float d = intersect(ray,scene.ground_plane[0]);
		if (d>0 && d<=t)
		{
			t=d;
			// YOUR CODE GOES HERE -------------------------------------------------------------------------------------
			// Populate I with all the relevant data.
			I.point = ray.origin + t * ray.dir;
			I.normal = scene.ground_plane[0].normal;
			I.material = scene.ground_plane[0].material;
			// Adding a procedural checkerboard texture:
			I.material.color_diffuse = (mod(floor(I.point.x) + floor(I.point.z),2.0) == 0.0) ?
			scene.ground_plane[0].material.color_diffuse :
			vec3(1.0) - scene.ground_plane[0].material.color_diffuse;
		}
	}

	//If no sphere or plane hit, we hit the sky instead
	if (t>1e20)
	{
		I.point = ray.dir*t;
		I.normal = -ray.dir;
		vec3 sky = simple_sky(ray.dir); // pick color from sk y function

		// Sky is all emission, no diffuse or glossy shading:
		I.material.color_diffuse = 0 * sky; 
		I.material.color_glossy = 0.0 * vec3( 1 );
		I.material.roughness = 1;
		I.material.color_emission = 0.3 * sky;
		I.material.reflection = 0.0;
		I.material.transmission = 0;
		I.material.ior = 1;

	}
	return I;
}


float seed = 0.;
float rand(void){return fract(sin(seed+=0.14) * 43758.5453123);}

vec3 random_direction(vec3 mean, float spread)
{
float r2 = spread*rand();
float phi = 2.0*PI*rand();
float sina = sqrt(r2);
float cosa = sqrt(1. - r2);
vec3 w = normalize(mean), u = normalize(cross(w.yzx, w)),v = cross(w, u);
return (u*cos(phi) + v*sin(phi)) * sina + w * cosa;
}




vec3 pathtrace(Ray ray) 
{
    vec3 color = vec3(0);
    vec3 absorbed= vec3(1);
    float f;
    for(int i=0; i < MAX_DEPTH; ++i)
    {
    f=rand();
    Intersection isec = intersect(ray);
    color +=  isec.material.color_emission*absorbed;
    absorbed *= isec.material.color_diffuse;
 
    
    if(f<isec.material.reflection)
    {
	   ray.dir = random_direction(reflect(ray.dir, isec.normal),0.001);
	   ray.origin = isec.point + ray.dir * 0.0001;
    }
    
    else if(f<isec.material.reflection+isec.material.transmission)
    {
       	  if(dot(ray.dir, isec.normal) > 0)
		  {
			ray.dir = refract(ray.dir, isec.normal, isec.material.ior / 1.0);
		  }
		  else
		  {
			ray.dir = refract(ray.dir, isec.normal, 1.0 / isec.material.ior);
		  }
          ray.dir=random_direction(ray.dir,0.001);


		  ray.origin = isec.point + ray.dir * 0.0001;
    }
    else
    {
    ray.origin = isec.point + isec.normal * 0.0001;
    ray.dir=random_direction(isec.normal,1);
    }
    }

  return color/MAX_DEPTH;
}


void main() {
  seed = i_global_time + i_window_size.y * gl_FragCoord.x /i_window_size.x + gl_FragCoord.y / i_window_size.y;
  vec2 tex_coords = gl_FragCoord.xy / i_window_size.xy;
  vec2 uv =  gl_FragCoord.xy - 0.5*i_window_size.xy;
  if(i_display)    
  {
    o_fragment_color = texture(i_texture,tex_coords);
  }    
  else
  {

    init();

    Ray ray;

    //COPY YOUR CODE FROM 4.1 HERE
    // Create a ray
		
	//crude zooming by pressing right mouse button
	float f_dist = i_focal_dist + i_focal_dist*i_mouse_state.w; 
	
	//basis for defining the image plane
	vec3 cx = i_right;
	vec3 cy = i_up;   
	vec3 cz = i_dir; 
	
	//scene.spheres[2].center = i_position; //Controll the red glowing sphere

	ray.origin = i_position;
	ray.weight = 1;

	// YOUR CODE GOES HERE -------------------------------------------------------------------------------------: 
	// Compute the correct ray direction using gl_fragCoord and the camera vectors above.
	ray.dir = normalize(cz*f_dist + uv.x*cx + uv.y*cy);

    // Push the ray noto the ray stack

    vec3 color=vec3(0);
    for(int i=0;i<MAX_SAMPLES;i++){
    color += pathtrace(ray); 
    

    // gamma corrected output color, and blended over several frames (good for path tracer)
    }
     o_fragment_color = (texture(i_texture,tex_coords)*i_frame_count + vec4( pow ( clamp(color.xyz/MAX_SAMPLES, 0., 1.), vec3(1./2.2)), 1.))/float(1+ i_frame_count); 
  }
}
