#define USE_TEST_SHADER 0

#if USE_TEST_SHADER

#define FRAGMENT_SHADER_SOURCE "../Lab4/testShader_fs.glsl"
#define VERTEX_SHADER_SOURCE "../Lab4/lab4_vs.glsl"

#else
#if defined(LAB4_1)
#define FRAGMENT_SHADER_SOURCE "../Lab4/lab4-1_fs.glsl"

#elif defined(LAB4_2)
#define FRAGMENT_SHADER_SOURCE "../Lab4/lab4-2_fs.glsl"

#elif defined(LAB4_3)
#define FRAGMENT_SHADER_SOURCE "../Lab4/lab4-3_fs.glsl"

#elif defined(LAB4_4)
#define FRAGMENT_SHADER_SOURCE "../Lab4/lab4-4_fs.glsl"

#endif

#define VERTEX_SHADER_SOURCE "../Lab4/lab4_vs.glsl"


#endif