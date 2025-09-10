#include "pch.h"
#include "StaticMeshDemo.h"
#include "Engine/GeometryHelper.h"
#include "Engine/Camera.h"
#include "Engine/GameObject.h"
#include "CameraScript.h"
#include "Engine/MeshRenderer.h"
#include "Engine/Mesh.h"
#include "Engine/Material.h"
#include "Engine/Model.h"
#include "Engine/ModelRenderer.h"
#include "Engine/ResourceManager.h"
#include "Engine/RenderManager.h"


void StaticMeshDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"16. ModelDemo.fx");

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());

	_camera->AddComponent(make_shared<CameraScript>());
	

	{ // 하나만 선택
		//CreateTower(1.f);
		//CreateTank(1.5f);
		CreateModelByName(L"Tank", 1.5f, 1);
	}

	RENDER->Init(_shader);
}

void StaticMeshDemo::Update()
{
	_camera->Update();
	RENDER->Update();

	{
		LightDesc lightDesc;
		lightDesc.ambient = Color(0.4f, 0.4f, 0.4f, 0.4f);
		lightDesc.diffuse = Color(1.f, 1.f, 1.f, 1.f);
		lightDesc.specular = Color(0.f, 0.f, 0.f, 0.f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		RENDER->PushLightData(lightDesc);
	}

	{
		_obj->Update();
	}

	Test(); // ※ IMGUI 이식 테스트
}

void StaticMeshDemo::Render()
{

}

void StaticMeshDemo::CreateTower(float scale)
{
	// CustomData -> Memory
	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Tower/Tower");
	m1->ReadMaterial(L"Tower/Tower");

	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 50));
	_obj->GetOrAddTransform()->SetScale(Vec3(scale, scale, scale));

	_obj->AddComponent(make_shared<ModelRenderer>(_shader));
	{
		_obj->GetModelRenderer()->SetModel(m1);
		//_obj->GetModelRenderer()->SetPass(1);
	}
}

void StaticMeshDemo::CreateTank(float scale)
{
	// CustomData -> Memory
	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Tank/Tank");
	m1->ReadMaterial(L"Tank/Tank");

	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 20));
	_obj->GetOrAddTransform()->SetScale(Vec3(scale, scale, scale));

	_obj->AddComponent(make_shared<ModelRenderer>(_shader));
	{
		_obj->GetModelRenderer()->SetModel(m1);
		_obj->GetModelRenderer()->SetPass(1);
	}
}

void StaticMeshDemo::CreateModelByName(wstring modelName, float scale, uint32 path)
{
	shared_ptr<class Model> m1 = make_shared<Model>();
	wstring modelPath = modelName + L"/" + modelName;

	m1->ReadModel(modelPath);
	m1->ReadMaterial(modelPath);

	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 20));
	_obj->GetOrAddTransform()->SetScale(Vec3(scale, scale, scale));

	_obj->AddComponent(make_shared<ModelRenderer>(_shader));
	{
		_obj->GetModelRenderer()->SetModel(m1);
		_obj->GetModelRenderer()->SetPass(path);
	}
}



void StaticMeshDemo::Test() // ※ IMGUI 이식 테스트
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// Rookiss
	{
		ImGui::Begin("Rookiss", nullptr,
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoMove
		);

		ImGui::Text("Rookiss Hello ImGui!");

		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}