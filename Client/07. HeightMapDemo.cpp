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
		�� png �� ������ �о���� ���
		 - �� �о���� _heightMap �� png �� ����Ž���⿡�� ��Ŭ�� �Ӽ� - �ڼ��� �� ��Ʈ������ ����, 8��Ʈ �� ���ִ�
		 - 8��Ʈ��, R8G8B8A8, R8, R32G32B32A32.. ��� ������ �� 8��, �� 8��Ʈ�� R8 �� �ǹ�
		 - R8 �̱� ������, uint8* pixelBuffer = info.GetPixels(); �� �о�帱 �� �ִ�
		 - ���� R8 �� �ƴ� �÷� ������ R8G8B8 �̶��, [R8.0][G8.0][B8.0][R8.1]... ���·� ���ֱ� ������, 
		   ���� R �����͸� ����������, 1, 4, 7, 10 ... ������ ������ ���� �ʿ�
	*/


	// Object
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateGrid(_geometry, width, height);

	// CPU
	{
		vector<VertexTextureData>& v = const_cast<vector<VertexTextureData>&>(_geometry->GetVertices());
		// �� const_cast : const �� ���������� ���� ���ϰ� ���� �Լ���, ������ ���������� �ް� ����� �Լ�

		for (int32 z = 0; z < height; z++)
		{
			for (int32 x = 0; x < width; x++)
			{
				int32 idx = width * z + x;

				uint8 height = pixelBuffer[idx] / 255.f * 25.f;
				v[idx].position.y = height; // ���� ����

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
