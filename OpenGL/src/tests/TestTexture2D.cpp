#include "TestTexture2D.h"

#include <unordered_map>
#include <string>

#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"

#include "Application.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

namespace test
{
	TestTexture2D::TestTexture2D()
		: _proj(glm::ortho(0.0f, resolution.x, 0.0f, resolution.y, -1.0f, 1.0f)),
			_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
			_translationA(200, 200, 0), _translationB(200, 400, 0)
	{
		std::unordered_map<std::string, std::string> textures =
		{
			{ "Rock", "res/textures/Rock062_1K-PNG/Rock062_1K-PNG_Color.png" },
			{ "Test-1k", "res/textures/Test-1k.png" },
			{ "Test-64", "res/textures/Test-64.png" },
		};

		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f,
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		_vertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		_vertexArray = std::make_unique<VertexArray>();
		_vertexArray->AddBuffer(*_vertexBuffer, layout);

		_indexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		_texture = std::make_unique<Texture>(textures["Rock"]);

		_shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		_shader->Bind();
		_shader->SetUniform1i("u_Texture", 0);
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		_texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), _translationA);
			glm::mat4 mvp = _proj * _view * model;

			_shader->Bind();
			_shader->SetUniformMat4f("u_MVP", mvp);
			Renderer::Draw(*_vertexArray, *_indexBuffer, *_shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), _translationB);
			glm::mat4 mvp = _proj * _view * model;

			_shader->Bind();
			_shader->SetUniformMat4f("u_MVP", mvp);
			Renderer::Draw(*_vertexArray, *_indexBuffer, *_shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &_translationA.x, 0.0f, resolution.x);
		ImGui::SliderFloat3("Translation B", &_translationB.x, 0.0f, resolution.x);
	}
}