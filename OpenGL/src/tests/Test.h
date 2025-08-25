#pragma once

#include <string>
#include <vector>
#include <functional>

#include "spdlog/spdlog.h"

namespace test
{
	class Test
	{
	public:
		Test() = default;
		virtual ~Test() = default;

		virtual void OnUpdate(float deltaTime) { }
		virtual void OnRender() { }
		virtual void OnImGuiRender() { }
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		virtual void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			spdlog::info("Registering test {0}", name);
			_tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& _currentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> _tests;
	};
}