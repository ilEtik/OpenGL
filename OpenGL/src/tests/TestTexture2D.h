#pragma once

#include "Test.h"

#include <memory>

#include "glm/glm.hpp"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();

		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> _vertexArray;
		std::unique_ptr<VertexBuffer> _vertexBuffer;
		std::unique_ptr<IndexBuffer> _indexBuffer;
		std::unique_ptr<Shader> _shader;
		std::unique_ptr<Texture> _texture;

		glm::mat4 _proj, _view;
		glm::vec3 _translationA;
		glm::vec3 _translationB;
	};
}