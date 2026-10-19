#ifndef GL_PLATFORM_H
#define GL_PLATFORM_H

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
// GLFW cursor/scroll callbacks still use CPU-side doubles on the Web.
using GLdouble = double;
#else
#include <glad/glad.h>
#endif

#endif
