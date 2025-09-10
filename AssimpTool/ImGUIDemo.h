#pragma once
#include "Engine/IExecute.h"

class ImGuiDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	void Test();

private:
	bool show_demo_window = true;
	bool show_another_window = false;
	Color clear_color = Color(0.f, 0.f, 0.f, 0.f);
};

