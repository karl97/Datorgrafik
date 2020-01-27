#define USE_TEST_SHADER 0

#if USE_TEST_SHADER

#define FRAGMENT_SHADER_SOURCE "../Lab4/testShader_fs.glsl"
#define VERTEX_SHADER_SOURCE "../Lab4/lab4_vs.glsl"

#else

#define FRAGMENT_SHADER_SOURCE "../Lab4/lab4-1_fs.glsl"
#define VERTEX_SHADER_SOURCE "../Lab4/lab4_vs.glsl"


#endif