#pragma once
#include "pch.h"
//#include "Define.h"
#include "Engine/Define.h"
#include "15. EmissiveDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "VertexData.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "RenderManager.h"
#include "ResourceManager.h"

void EmissiveDemo::Init()
{
	RESOURCES->Init();

	_shader = make_shared<Shader>(L"13. Lighting_Emissive.fx");

	//// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTrasnform()->SetPosition(Vec3{ 0.f, 0.f, -10.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());


	 //Object
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTrasnform();
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		//_obj->GetMeshRenderer()->SetShader(_shader);
	}
	{
		shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
		//_obj->GetMeshRenderer()->SetTexture(texture);
	}


	// Object2
	_obj2 = make_shared<GameObject>();
	_obj2->GetOrAddTrasnform()->SetPosition(Vec3{ 0.5f, 0.f, 2.f });
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		//_obj2->GetMeshRenderer()->SetShader(_shader);
	}
	{
		shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
		//_obj2->GetMeshRenderer()->SetTexture(texture);
	}



	RENDER->Init(_shader);
}

void EmissiveDemo::Update()
{
	_camera->Update();
	RENDER->Update();
	
	{
		Vec4 materialEmissive(1.0f, 0.f, 0.f, 1.f);
		_shader->GetVector("MaterialEmissive")->SetFloatVector((float*)&materialEmissive);

		_obj->Update();
	}

	{
		Vec4 materialEmissive(0.0f, 1.f, 0.f, 1.f);
		_shader->GetVector("MaterialEmissive")->SetFloatVector((float*)&materialEmissive);

		_obj2->Update();
	}
}

void EmissiveDemo::Render()
{
	
}
