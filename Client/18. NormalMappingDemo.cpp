#pragma once
#include "pch.h"
#include "18. NormalMappingDemo.h"
//#include "Define.h"
#include "Engine/Define.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "GameObject.h"
//#include "Transform.h"
#include "Texture.h"
#include "VertexData.h"
#include "MeshRenderer.h"
#include "Mesh.h"
//#include "RenderManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Material.h"
#include "TimeManager.h"

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
			//shared_ptr<Texture> texture
				//= RESOURCES->Load<Texture>(L"Veigar_Texture", L"..\\Resources\\Textures\\Veigar.jpg");
			material->SetDiffuseMap(texture);
		}
		{
			shared_ptr<Texture> normal 
				= RESOURCES->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Textures\\Leather_Normal.jpg");
			material->SetNormalMap(normal);
		}

		
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Color(1.f, 1.f, 1.f, 1.f);
		desc.diffuse = Color(1.f, 1.f, 1.f, 1.f);
		desc.specular = Color(1.f, 1.f, 1.f, 1.f);
		desc.emissive = Color(1.f, 0.f, 0.f, 1.f);

		RESOURCES->Add(L"Leather", material);


	}


	//// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTrasnform()->SetPosition(Vec3{ 0.f, 3.f, -15.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());


	 //Object
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTrasnform()->SetPosition(Vec3(0.f, 4.f, -8.f));
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
	_obj2->GetOrAddTrasnform()->SetPosition(Vec3{ 0, 0.f, 2.f });
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		shared_ptr<Material> material = RESOURCES->Get<Material>(L"Leather")->Clone();

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.emissive = Color(0.f, 0.f, 1.f, 1.f);
		
		_obj2->GetMeshRenderer()->SetMaterial(material);
	}






	{ // DEBUG

		_sun = make_shared<GameObject>();
		_sun->GetOrAddTrasnform();
		_sun->AddComponent(make_shared<MeshRenderer>());
		{
			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
			_sun->GetMeshRenderer()->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = RESOURCES->Get<Material>(L"Leather");
			_sun->GetMeshRenderer()->SetMaterial(material);
		}



		_earth = make_shared<GameObject>();
		_earth->GetOrAddTrasnform()->SetPosition(Vec3(7.f, 3.5f, 0));
		_earth->GetTransform()->AddLocalPitchRotation(10.f);
		_earth->GetTransform()->SetParent(*(_sun->GetTransform().get()));
		_earth->AddComponent(make_shared<MeshRenderer>());
		{
			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
			_earth->GetMeshRenderer()->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = RESOURCES->Get<Material>(L"Leather");
			_earth->GetMeshRenderer()->SetMaterial(material);
		}

		_moon = make_shared<GameObject>();
		_moon->GetOrAddTrasnform()->SetPosition(Vec3(10.f, 3.5f, 0));
		//_moon->GetTransform()->SetScale(myVec3(0.5f, 0.5f, 0.5f));
		_moon->GetTransform()->SetParent(*(_earth->GetTransform().get()));
		_moon->AddComponent(make_shared<MeshRenderer>());
		{
			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
			_moon->GetMeshRenderer()->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = RESOURCES->Get<Material>(L"Leather");
			_moon->GetMeshRenderer()->SetMaterial(material);
		}
	}
















	RENDER->Init(_shader);
}

void NormalMappingDemo::Update()
{
	_camera->Update();

	{ // DEBUG

		/*if (INPUT->GetButton(KEY_TYPE::KEY_1))
		{
			_obj2->GetTransform()->AddYawRotation(0.66f);
		}
		if (INPUT->GetButton(KEY_TYPE::KEY_2))
		{
			_obj2->GetTransform()->AddYawRotation(-0.66f);
		}

		if (INPUT->GetButton(KEY_TYPE::KEY_3))
		{
			_obj2->GetTransform()->AddPosition(myVec3(5.f * DT, 0, 0));
		}
		if (INPUT->GetButton(KEY_TYPE::KEY_4))
		{
			_obj2->GetTransform()->AddPosition(myVec3(-5.f * DT, 0, 0));
		}*/

		if (INPUT->GetButton(KEY_TYPE::KEY_2))
		{
			_obj->GetTransform()->AddUnitZRotation(DT * 50.f);
		}
		if (INPUT->GetButton(KEY_TYPE::KEY_3))
		{
			_obj->GetTransform()->AddUnitYRotation(DT * 50.f);
		}
		if (INPUT->GetButton(KEY_TYPE::KEY_4))
		{
			_obj->GetTransform()->AddUnitXRotation(DT * 50.f);
		}


	}


	RENDER->Update();
	
	{
		LightDesc lightDesc;
		lightDesc.ambient = Color(0.5f, 0.5f, 0.5f, 0.5f);
		lightDesc.diffuse = Color(1.f, 1.f, 1.f, 1.f);
		lightDesc.specular = Color(1.f, 1.f, 1.f, 1.f);
		Vec3 lightDirection = Vec3(1.f, 0.f, 1.f);
		lightDirection.Normalize();
		lightDesc.direction = lightDirection;
		RENDER->PushLightData(lightDesc);
	}

	{
		MaterialDesc desc;
		desc.ambient = Color(0.2f, 0.2f, 0.2f, 0.2f);
		desc.diffuse = Color(1.f, 1.f, 1.f, 1.f);
		desc.specular = Color(1.f, 1.f, 1.f, 1.f);
		desc.emissive = Color(0.3f, 0.f, 0.f, 0.5f);
		RENDER->PushMaterialData(desc);

		_obj->Update();
	}

	{
		MaterialDesc desc;
		desc.ambient = Color(0.2f, 0.2f, 0.2f, 0.2f);
		desc.diffuse = Color(1.f, 1.f, 1.f, 1.f);
		desc.specular = Color(1.f, 1.f, 1.f, 1.f);
		desc.emissive = Color(0.f, 0.3f, 0.f, 0.5f);
		RENDER->PushMaterialData(desc);

		_obj2->Update();
	}




	{ // DEBUG

		_sun->GetTransform()->AddRollRotation(50.f * DT);
		//_sun->GetTransform()->AddYawRotation(50.f * DT);
		_sun->Update();

		//_sun->GetTransform()->SetPosition(Vec3(0, 0, -2.f));
		//_sun->GetTransform()->SetRotation(myVec3(13.f, 24.f, 20.f));

		if (INPUT->GetButton(KEY_TYPE::KEY_1))
		{
			Transform* transform = _sun->GetTransform().get();
			myVec3 pos = transform->GetPosition();
			//myMatrix4x4 W = transform->GetSRT();
			myMatrix4x4 W = transform->GetSRT();

			myMatrix4x4 V = Camera::S_MatView;
			myMatrix4x4 P = Camera::S_MatProjection;

			myMatrix4x4 WV = W * V;
			myMatrix4x4 WVP = WV * P;

			myVec4 finalPos = Vec4(pos, 1) *  (W * V * P);
			float ndcX = finalPos.X / finalPos.W;
			float ndcY = finalPos.Y / finalPos.W;
			
			myVec3 euler = transform->GetEulerAngles();
			WCHAR str[8000];
			swprintf_s(str, 8000,
				L"Pos : %.3f / %.3f / %.3f \n"
				L"W\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"V\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"P\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"WV\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"WVP\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"%.3f / %.3f / %.3f / %.3f\n"
				L"FinPos\n"
				L"%.3f %.3f %.3f %.3f\n"
				L"NDCPos\n"
				L"%.3f %.3f\n"
				"%6.3f"
				,

				static_cast<float>(pos.X), static_cast<float>(pos.Y), static_cast<float>(pos.Z),



				static_cast<float>(W[0][0]), static_cast<float>(W[0][1]),
				static_cast<float>(W[0][2]), static_cast<float>(W[0][3]),

				static_cast<float>(W[1][0]), static_cast<float>(W[1][1]),
				static_cast<float>(W[1][2]), static_cast<float>(W[1][3]),

				static_cast<float>(W[2][0]), static_cast<float>(W[2][1]),
				static_cast<float>(W[2][2]), static_cast<float>(W[2][3]),

				static_cast<float>(W[3][0]), static_cast<float>(W[3][1]),
				static_cast<float>(W[3][2]), static_cast<float>(W[3][3]),



				V[0][0], V[0][1], V[0][2], V[0][3],
				V[1][0], V[1][1], V[1][2], V[1][3],
				V[2][0], V[2][1], V[2][2], V[2][3],
				V[3][0], V[3][1], V[3][2], V[3][3],


				static_cast<float>(P[0][0]), static_cast<float>(P[0][1]),
				static_cast<float>(P[0][2]), static_cast<float>(P[0][3]),

				static_cast<float>(P[1][0]), static_cast<float>(P[1][1]),
				static_cast<float>(P[1][2]), static_cast<float>(P[1][3]),

				static_cast<float>(P[2][0]), static_cast<float>(P[2][1]),
				static_cast<float>(P[2][2]), static_cast<float>(P[2][3]),

				static_cast<float>(P[3][0]), static_cast<float>(P[3][1]),
				static_cast<float>(P[3][2]), static_cast<float>(P[3][3]),



				WV[0][0], WV[0][1], WV[0][2], WV[0][3],
				WV[1][0], WV[1][1], WV[1][2], WV[1][3],
				WV[2][0], WV[2][1], WV[2][2], WV[2][3],
				WV[3][0], WV[3][1], WV[3][2], WV[3][3],

				WVP[0][0], WVP[0][1], WVP[0][2], WVP[0][3],
				WVP[1][0], WVP[1][1], WVP[1][2], WVP[1][3],
				WVP[2][0], WVP[2][1], WVP[2][2], WVP[2][3],
				WVP[3][0], WVP[3][1], WVP[3][2], WVP[3][3],

				finalPos.X, finalPos.Y, finalPos.Z, finalPos.W,

				ndcX, ndcY,


				888.001f
			
			);
			MessageBox(NULL, str, NULL, MB_OK);
		}



		_earth->GetTransform()->AddYawRotation(150.f * DT);
		_earth->Update();


		_moon->Update();
	}
}

void NormalMappingDemo::Render()
{
	
}
