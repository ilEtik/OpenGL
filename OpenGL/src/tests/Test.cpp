#include "Test.h"

#include "imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		: _currentTest(currentTestPointer)
	{

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : _tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				spdlog::info("Test {0} was clicked", test.first);
				_currentTest = test.second();
			}
		}
	}
}