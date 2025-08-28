#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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

namespace Renderer
{
    void Clear();
    void ClearColor(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
    void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
}