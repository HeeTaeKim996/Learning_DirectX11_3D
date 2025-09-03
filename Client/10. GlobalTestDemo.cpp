#pragma once
#include "pch.h"
//#include "Define.h"
#include "Engine/Define.h"
#include "10. GlobalTestDemo.h"
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

void GlobalTestDemo::Init()
{
	_shader = make_shared<Shader>(L"09. GlobalTestShader.fx");

	//// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTrasnform();
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
		RESOURCES->Init();
		shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
		//_obj->GetMeshRenderer()->SetTexture(texture);
	}

	RENDER->Init(_shader);
}

void GlobalTestDemo::Update()
{
	_camera->Update();

	RENDER->Update();
	
	_obj->Update();
}

void GlobalTestDemo::Render()
{
	
}
