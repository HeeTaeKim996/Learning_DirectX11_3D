#pragma once

class Material;

struct ModelBone
{
	wstring name;
	int32 index;
	int32 parentIndex;
	shared_ptr<ModelBone> parent; // Cache

	Matrix transform; 
	// �� ȿ�� �ʹ� ������. ���� ���� ���� TransformBase �� �����ϸ� ������,
	//   AssimTool ���� �ѱ� �� AsBone ���� Matrix�� �ѱ�� ������.. ������ �� �ʹ� ��������, �ϴ� Matrix �� ���



	vector<shared_ptr<ModelBone>> children; // Cache
};

struct ModelMesh
{
	void CreateBuffers();


	wstring name;

	// Mesh
	shared_ptr<Geometry<ModelVertexType>> geometry = make_shared<Geometry<ModelVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;


	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache


	// Bones
	int32 boneIndex;
	shared_ptr<ModelBone> bone; // Cache
};



