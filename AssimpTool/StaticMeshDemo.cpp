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
	

	CreateTower();
	//CreateTank();

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
}

void StaticMeshDemo::Render()
{

}

void StaticMeshDemo::CreateTower()
{
	// CustomData -> Memory
	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Tower/Tower");
	m1->ReadMaterial(L"Tower/Tower");

	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 50));
	_obj->GetOrAddTransform()->SetScale(Vec3(1.0f, 1.0f, 1.0f));

	_obj->AddComponent(make_shared<ModelRenderer>(_shader));
	{
		_obj->GetModelRenderer()->SetModel(m1);
		_obj->GetModelRenderer()->SetPass(1);
	}
}

//void StaticMeshDemo::CreateTank()
//{
//	// CustomData -> Memory
//	shared_ptr<class Model> m1 = make_shared<Model>();
//	m1->ReadModel(L"Tank/Tank");
//	m1->ReadMaterial(L"Tank/Tank");
//
//	_obj = make_shared<GameObject>();
//	_obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 50));
//	_obj->GetOrAddTransform()->SetScale(Vec3(1.f, 1.f, 1.f));
//
//	_obj->AddComponent(make_shared<ModelRenderer>(_shader));
//	{
//		_obj->GetModelRenderer()->SetModel(m1);
//		_obj->GetModelRenderer()->SetPass(1);
//	}
//}
