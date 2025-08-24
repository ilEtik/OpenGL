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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glm::vec2 resolution { 1280, 720 };

	GLFWwindow* window = glfwCreateWindow(resolution.x, resolution.y, "OpenGL", NULL, NULL);
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

	// ImGui Init
	{
		ImGui::CreateContext();
		
		ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		ImGui::StyleColorsDark();
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	GLCall(spdlog::info("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION)));

	std::unordered_map<std::string, std::string> textures =
	{
		{ "Rock", "res/textures/Rock062_1K-PNG/Rock062_1K-PNG_Color.png" },
		{ "Test-1k", "res/textures/Test-1k.png" },
		{ "Test-64", "res/textures/Test-64.png" },
	};

	float positions[] = {
		400.0f, 100.0f, 0.0f, 0.0f,
		800.0f, 100.0f, 1.0f, 0.0f,
		800.0f, 500.0f, 1.0f, 1.0f,
		400.0f, 500.0f, 0.0f, 1.0f,
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

		glm::mat4 proj = glm::ortho(0.0f, resolution.x, 0.0f, resolution.y, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, 0.0f, 0.0f));

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture(textures["Rock"]);
		texture.Bind();

		shader.SetUniform1i("u_Texture", 0);

		vertexArray.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		glm::vec3 translation(200.0f, 200.0f, 0.0f);

		while (!glfwWindowShouldClose(window))
		{
			Renderer::Clear();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 mvp = proj * view * model;

			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);

			Renderer::Draw(vertexArray, indexBuffer, shader);

			// ImGui New Frame
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}

			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, resolution.x);

			// ImGui Render Frame
			{
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow* backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				}
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}