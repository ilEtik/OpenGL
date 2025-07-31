#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

	std::unordered_map<std::string, std::string> textures =
	{
		{ "Rock", "res/textures/Rock062_1K-PNG/Rock062_1K-PNG_Color.png" },
		{ "Test-1k", "res/textures/Test-1k.png" },
		{ "Test-64", "res/textures/Test-64.png" },
	};

	float positions[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	{
		VertexArray vertexArray;
		VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(indices, 6);

		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", proj);

		Texture texture(textures["Test-1k"]);
		texture.Bind();

		shader.SetUniform1i("u_Texture", 0);

		vertexArray.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		while (!glfwWindowShouldClose(window))
		{
			Renderer::Clear();

			shader.Bind();

			Renderer::Draw(vertexArray, indexBuffer, shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}