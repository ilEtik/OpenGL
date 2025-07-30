#pragma once

#include <GL/glew.h>

#ifdef DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define ASSERT(X)
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);