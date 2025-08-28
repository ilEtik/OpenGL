#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui.h"

namespace test
{
	TestClearColor::TestClearColor()
		: _clearColor { 0.2f, 0.3f, 0.8f, 1.0f }
	{

	}

	void TestClearColor::OnRender()
	{
		Renderer::ClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[2]);
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", _clearColor);
	}
}