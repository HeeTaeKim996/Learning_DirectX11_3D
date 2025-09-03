#pragma once
#include "pch.h"
#include "18. NormalMappingDemo.h"
//#include "Define.h"
#include "Engine/Define.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "VertexData.h"
#include "MeshRenderer.h"
#include "Mesh.h"
//#include "RenderManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Material.h"

void NormalMappingDemo::Init()
{
	RESOURCES->Init();

	_shader = make_shared<Shader>(L"15. NormalMapping.fx");


	// Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		{
			material->SetShader(_shader);
		}
		{
			shared_ptr<Texture> texture
				= RESOURCES->Load<Texture>(L"Leather_Texture", L"..\\Resources\\Textures\\Leather.jpg");
			material->SetDiffuseMap(texture);
		}
		{
			shared_ptr<Texture> normal 
				= RESOURCES->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Textures\\Leather_Normal.jpg");
			material->SetNormalMap(normal);
		}
		
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(1.f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		desc.emissive = Vec4(1.f, 0.f, 0.f, 1.f);

		RESOURCES->Add(L"Leather", material);


	}


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
		shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		shared_ptr<Material> material = RESOURCES->Get<Material>(L"Leather");
		_obj->GetMeshRenderer()->SetMaterial(material);
	}


	// Object2
	_obj2 = make_shared<GameObject>();
	_obj2->GetOrAddTrasnform()->SetPosition(Vec3{ 0.5f, 0.f, 2.f });
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		shared_ptr<Material> material = RESOURCES->Get<Material>(L"Leather")->Clone();

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.emissive = Vec4(0.f, 0.f, 1.f, 1.f);
		
		_obj2->GetMeshRenderer()->SetMaterial(material);
	}



	RENDER->Init(_shader);
}

void NormalMappingDemo::Update()
{
	_camera->Update();
	RENDER->Update();
	
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f, 1.f, 1.f, 1.f);
		Vec3 lightDirection = Vec3(1.f, 0.f, 1.f);
		lightDirection.Normalize();
		lightDesc.direction = lightDirection;
		RENDER->PushLightData(lightDesc);
	}

	{
		MaterialDesc desc;
		desc.ambient = Vec4(0.2f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		desc.emissive = Color(0.3f, 0.f, 0.f, 0.5f);
		RENDER->PushMaterialData(desc);

		_obj->Update();
	}

	{
		MaterialDesc desc;
		desc.ambient = Vec4(0.2f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		desc.emissive = Color(0.f, 0.3f, 0.f, 0.5f);
		RENDER->PushMaterialData(desc);

		_obj2->Update();
	}
}

void NormalMappingDemo::Render()
{
	
}
