#pragma once

#include "Test.h"

namespace test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();

		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		float _clearColor[4];
	};
}