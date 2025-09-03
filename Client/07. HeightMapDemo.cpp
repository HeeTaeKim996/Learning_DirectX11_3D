#include "pch.h"
#include "07. HeightMapDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "VertexData.h"
#include "ResourceManager.h"

void HeightMapDemo::Init()
{
	_shader = make_shared<Shader>(L"06. Terrain.fx");


	// Textures
	_heightMap = RESOURCES->Load<Texture>(L"Height", L"..\\Resources\\Textures\\Terrain\\height.png");
	_texture = RESOURCES->Load<Texture>(L"Grass", L"..\\Resources\\Textures\\Terrain\\grass.jpg");


	const int32 width = _heightMap->GetSize().x;
	const int32 height = _heightMap->GetSize().y;


	const DirectX::ScratchImage& info = _heightMap->GetInfo();
	uint8* pixelBuffer = info.GetPixels();

	/*
		○ png 의 데이터 읽어오는 방법
		 - 위 읽어오는 _heightMap 의 png 를 파일탐색기에서 우클릭 속성 - 자세히 의 비트수준을 보면, 8비트 로 돼있다
		 - 8비트는, R8G8B8A8, R8, R32G32B32A32.. 등에서 나오는 그 8로, 위 8비트는 R8 을 의미
		 - R8 이기 때문에, uint8* pixelBuffer = info.GetPixels(); 로 읽어드릴 수 있다
		 - 만약 R8 이 아닌 컬러 데이터 R8G8B8 이라면, [R8.0][G8.0][B8.0][R8.1]... 형태로 돼있기 때문에, 
		   만약 R 데이터만 가져오려면, 1, 4, 7, 10 ... 등으로 데이터 접근 필요
	*/


	// Object
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateGrid(_geometry, width, height);

	// CPU
	{
		vector<VertexTextureData>& v = const_cast<vector<VertexTextureData>&>(_geometry->GetVertices());
		// ※ const_cast : const 로 좌측값으로 받지 못하게 만든 함수를, 강제로 좌측값으로 받게 만드는 함수

		for (int32 z = 0; z < height; z++)
		{
			for (int32 x = 0; x < width; x++)
			{
				int32 idx = width * z + x;

				uint8 height = pixelBuffer[idx] / 255.f * 25.f;
				v[idx].position.y = height; // 높이 보정

			}
		}
	}


	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());

	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTrasnform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	_camera->GetTransform()->SetPosition(Vec3(0.f, 5.f, 0.f));
	_camera->GetTransform()->SetRotation(Vec3(25.f, 0.f, 0.f));

}

void HeightMapDemo::Update()
{
	_camera->Update();
}

void HeightMapDemo::Render()
{
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());


	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
