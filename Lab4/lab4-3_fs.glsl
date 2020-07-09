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
#define NUM_TRIANGLES 4
#define MAX_DEPTH 20
#define MAX_SAMPLES 1

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

struct Triangle {
  vec3 corners[3];
  float offset;
  vec3 normal;
  Material material;
};

struct Scene {

  Sphere spheres[NUM_SPHERES];
  Plane ground_plane[1];
  Triangle triangles[NUM_TRIANGLES];
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
  scene.ground_plane[0].offset = -1;
  scene.ground_plane[0].material.color_diffuse = 1.0 * vec3( 0.6 );
  scene.ground_plane[0].material.color_glossy = vec3( 0 );
  scene.ground_plane[0].material.roughness = 0;
  scene.ground_plane[0].material.color_emission = vec3( 0 );
  scene.ground_plane[0].material.reflection = 0.0;
  scene.ground_plane[0].material.transmission = 0;
  scene.ground_plane[0].material.ior = 1;

  vec3 corner_0 = vec3( 0.0, 0, 0.5);
  vec3 corner_1 = vec3(+0.5, 1.0, 0.5);
  vec3 corner_2 = vec3(-0.5, 1.0, 0.5);
  vec3 corner_3 = vec3( 0.0, 1.0, 1.0);

  //Front triangle
  scene.triangles[0].corners[0] = corner_0;
  scene.triangles[0].corners[1] = corner_1;
  scene.triangles[0].corners[2] = corner_2;

  scene.triangles[0].material = scene.spheres[1].material;
  
  //Left Triangle
  scene.triangles[1].corners[0] = corner_0;
  scene.triangles[1].corners[1] = corner_3;
  scene.triangles[1].corners[2] = corner_1;
  scene.triangles[1].material = scene.spheres[1].material;

  //Right triangle
  scene.triangles[2].corners[0] = corner_1;
  scene.triangles[2].corners[1] = corner_2;
  scene.triangles[2].corners[2] = corner_3;
  scene.triangles[2].material = scene.spheres[1].material;

  //Top triangle
  scene.triangles[3].corners[0] = corner_0;
  scene.triangles[3].corners[1] = corner_2;
  scene.triangles[3].corners[2] = corner_3;
  scene.triangles[3].material = scene.spheres[1].material;
  

  for(int i = 0; i < NUM_TRIANGLES; i++)
  {
	scene.triangles[i].normal = normalize(cross(
	scene.triangles[i].corners[1] - scene.triangles[i].corners[0], //B - A
	scene.triangles[i].corners[2] - scene.triangles[i].corners[0])); //C - A

	vec3 A_to_Origo = vec3(0) - scene.triangles[i].corners[0];
	float dist = dot(A_to_Origo, scene.triangles[i].normal);

	scene.triangles[i].offset = -dist; //dot(Origo - A, normal)
  }
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

// Ray-triangle intersection
float intersect(Ray ray, Triangle t) 
{

	float intersect_distance = 1e32;
	Plane p;
	p.offset = t.offset;
	p.normal = t.normal;

	float plane_distance = intersect(ray, p);
	vec3 Q = ray.origin + ray.dir * plane_distance;

	vec3 BA = t.corners[1] - t.corners[0];
	vec3 CB = t.corners[2] - t.corners[1];
	vec3 AC = t.corners[0] - t.corners[2];
	
	vec3 QA = Q - t.corners[0];
	vec3 QB = Q - t.corners[1];
	vec3 QC = Q - t.corners[2];

	if( (dot(normalize(cross(BA, QA)), t.normal) >= 0.) && 
		(dot(normalize(cross(CB, QB)), t.normal) >= 0.) && 
		(dot(normalize(cross(AC, QC)), t.normal) >= 0.)) 
	{
		
		intersect_distance = plane_distance;
	}

	return intersect_distance;
}

// Check for intersection of a ray and all objects in the scene
Intersection intersect( Ray ray)
{
  Intersection I;

  //COPY YOUR CODE FROM 4.1 HERE
  // Manage intersections
	float t = 1e32;
	//Check triangle intersection
	{
		int id = -1;
		for (int i = 0; i < NUM_TRIANGLES; ++i)
		{
			float d = intersect(ray, scene.triangles[i]);
			if (d>0 && d<=t)
			{
				t = d; 
				id = i;
			}
		}

		if(id != -1)
		{
			I.point = ray.origin + t * ray.dir;
			I.normal = scene.triangles[id].normal;
			I.material = scene.triangles[id].material;
		}
	}
	
    
	//Check for intersection with spheres 
	{
		int id = -1;
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

vec3 BP_brdf(float kl, float kg, float f, vec3 indir, vec3 normal, vec3 outdir) // the Blinn-Phong brdf 
{

	vec3 h=normalize(outdir+indir);

	return vec3(kl/3.14) + vec3(((8+f)/(8*3.14)))*kg*pow(dot(normal,h),f);
}

vec3 raytrace() 
{
  vec3 ambient = 0.2 * simple_sky(vec3(0,1,0));   // global ambient light
  vec3 color = vec3(0);


  {
    // Iterate over all rays on the stack
    for(int ray_stack_pos=0; ray_stack_pos < ray_stack_size; ++ray_stack_pos)
    {

      Ray ray = ray_stack[ray_stack_pos];
      Intersection isec = intersect(ray);

      vec3 nl = isec.normal * sign(-dot(isec.normal, ray.dir)); 
      vec3 light_direction = normalize(i_light_position - isec.point);

      vec3 this_color = vec3(0);
            
      float reflectivity = isec.material.reflection;

      if (isec.material.transmission > 0)
      {
        // YOUR TASK: Create new ray, compute its position, its
        // direction (based on isec.material.ior) and its weight, and
        // call push(new_ray). In the next iteration of the for loop,
        // the new ray will be handled, so no recursive call to
        // raytrace() is required.

        // Optionally, compute what fraction should be reflected, and
        // send out a second ray in the reflection
        // direction. Otherwise, use the block below for specular
        // reflection.

	      Ray ray2 = ray;
	      ray2.weight = ray.weight * isec.material.transmission;
		  if(dot(ray.dir, isec.normal) > 0)
		  {
			ray2.dir = refract(ray.dir, isec.normal, isec.material.ior / 1.0);
		  }
		  else
		  {
			ray2.dir = refract(ray.dir, isec.normal, 1.0 / isec.material.ior);
		  }

		  ray2.origin = isec.point + ray2.dir * 0.0001;

		  push( ray2 );

      }
      
      if (isec.material.reflection > 0)
      {
        // YOUR TASK: Create new ray, compute its position, direction,
        // and weight, and call push(ray).

	      Ray ray2 = ray;
	      ray2.weight = ray.weight * isec.material.reflection;
		  ray2.dir = reflect(ray.dir, isec.normal);
		  ray2.origin = isec.point + ray2.dir * 0.0001;
		  
		  push( ray2 );

      }

      
      // Now handle non-specular scattering (i.e., the non-recursive case)
      {

		color += this_color;
        // YOUR TASK: Create a "shadow feeler" ray and check if this
        // point is visible from the (single) light source.
        // If it is in shadow, set a black (or dark "ambient") colour.
		Ray shadow_feeler;
		shadow_feeler.origin = isec.point + isec.normal * 0.0001;
		shadow_feeler.dir = normalize(i_light_position - isec.point);
		Intersection shadow_feeler_isec = intersect(shadow_feeler);

		vec3 light_in = shadow_feeler.dir;
		vec3 light_out = -normalize(ray.dir);
		if(length(shadow_feeler_isec.point) <= 1e20)
		{
			//In shadow: Color with ambient light
			color += ray.weight * ambient* (isec.material.color_diffuse * BP_brdf(1, 0, 0, light_in, isec.normal, light_out) * max(0, dot(light_in, isec.normal)) + isec.material.color_emission);
		}
		else
		{
			//Not in shadow: Show highlights
			color += ray.weight * (isec.material.color_diffuse * BP_brdf(1, 3.0, isec.material.roughness, light_in, isec.normal, light_out) * max(0, dot(light_in, isec.normal)) + isec.material.color_emission);
		}

        // YOUR TASK: If the point is not in shadow, compute the
        // colour here (using your BRDF, as before).
        
            
        

        
      }


    }
  }

  return color;
}

void main() {

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
    push( ray );
    
    vec3 color = raytrace(); 
    

    // gamma corrected output color, and blended over several frames (good for path tracer)
    o_fragment_color = (texture(i_texture,tex_coords)*i_frame_count + vec4( pow ( clamp(color.xyz/MAX_SAMPLES, 0., 1.), vec3(1./2.2)), 1.))/float(1+ i_frame_count); 

  }
}
