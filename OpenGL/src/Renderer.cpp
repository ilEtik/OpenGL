#include "Renderer.h"

#include "spdlog/spdlog.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		spdlog::error("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line);
		return false;
	}

	return true;
}

namespace Renderer
{
	void Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader)
	{
		shader.Bind();
		vertexArray.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}