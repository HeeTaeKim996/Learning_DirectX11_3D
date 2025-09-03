#include "pch.h"
#include "09. MeshDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "VertexData.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "ResourceManager.h"

void MeshDemo::Init()
{
	_shader = make_shared<Shader>(L"08. Mesh.fx");


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
		shared_ptr<Shader> shader = make_shared<Shader>(L"08. Mesh.fx");
		//_obj->GetMeshRenderer()->SetShader(shader);
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
}

void MeshDemo::Update()
{
	_camera->Update();
	_obj->Update();
}

void MeshDemo::Render()
{
	
}
