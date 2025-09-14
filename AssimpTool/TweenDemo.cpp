#include "pch.h"
#include "TweenDemo.h"
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
#include "Engine/ModelAnimator.h"

void TweenDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"18. TweenDemo.fx");

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());

	_camera->AddComponent(make_shared<CameraScript>());
	

	{ // 하나만 선택
		//CreateModelByName(L"Kachujin", 1.5f, 1);
		//CreateModelByName(L"Tank", 1.5f, 1);
		CreateKachujin();
	}

	RENDER->Init(_shader);
}

void TweenDemo::Update()
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

	//Test(); // ※ IMGUI 이식 테스트
}

void TweenDemo::Render()
{

}

void TweenDemo::CreateKachujin()
{
	shared_ptr<class Model> m1 = make_shared<Model>();
	wstring modelPath = L"Kachujin/Kachujin";

	m1->ReadModel(modelPath);
	m1->ReadMaterial(modelPath);
	m1->ReadAnimation(L"Kachujin/Idle");
	m1->ReadAnimation(L"Kachujin/Run");
	m1->ReadAnimation(L"Kachujin/Slash");

	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 20));
	float scale = 0.1f;
	_obj->GetOrAddTransform()->SetScale(Vec3(scale, scale, scale));

	_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	{
		_obj->GetModelAnimator()->SetModel(m1);
		_obj->GetModelAnimator()->SetPass(0);
	}
}

void TweenDemo::CreateModelByName(wstring modelName, float scale, uint32 path)
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

