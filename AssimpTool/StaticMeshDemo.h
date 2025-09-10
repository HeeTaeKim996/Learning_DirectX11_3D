#pragma once
#include "Engine/IExecute.h"

class StaticMeshDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	void CreateTower(float scale);
	void CreateTank(float scale);


private:
	void CreateModelByName(wstring modelName, float scale, uint32 path);
	void Test(); // ※ IMGUI 이식 테스트

private:
	shared_ptr<Shader> _shader;
	shared_ptr<GameObject> _obj;
	shared_ptr<GameObject> _camera;



private:
	// ※ IMGUI 이식 테스트
	bool show_demo_window = true;
	bool show_another_window = false;
	Color clear_color = Color(0.f, 0.f, 0.f, 0.f);
};

