#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK)
	{
		spdlog::critical("Glew Init failed with error: {0}", glewInitResult);
		glfwTerminate();
		return -1;
	}

	GLCall(spdlog::info("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION)));

	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	{
		VertexArray vertexArray;
		VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(indices, 6);

		Shader shader("res/shaders/Basic.shader");

		vertexArray.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		float r = 0.3f, g = 0.6f, b = 0.9f;
		float rIncrement = 0.01f, gIncrement = 0.01f, bIncrement = 0.01f;

		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_Color", r, g, b, 1.0f);

			vertexArray.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
				rIncrement = -0.02f;
			else if (r < 0.0f)
				rIncrement = 0.02f;
			r += rIncrement;

			if (g > 1.0f)
				gIncrement = -0.02f;
			else if (g < 0.0f)
				gIncrement = 0.02f;
			g += gIncrement;

			if (b > 1.0f)
				bIncrement = -0.02f;
			else if (b < 0.0f)
				bIncrement = 0.02f;
			b += bIncrement;

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}