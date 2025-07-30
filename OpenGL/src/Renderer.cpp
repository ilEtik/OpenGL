#include "Renderer.h"

#include <iostream>
#include <format>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << std::format("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line) << std::endl;
		return false;
	}

	return true;
}