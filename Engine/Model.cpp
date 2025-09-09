#include "pch.h"
#include "Model.h"
#include "Utils.h"
#include "FileUtils.h"
#include "tinyxml2.h"
#include <filesystem>
#include "Material.h"
#include "ModelMesh.h"
#include "ResourceManager.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::ReadMaterial(wstring fileName)
{
	wstring fullPath = _texturePath + fileName + L".xml";
	filesystem::path parentPath = filesystem::path(fullPath).parent_path();

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = document->LoadFile(Utils::ToString(fullPath).c_str());
	assert(error == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement* root = document->FirstChildElement();
	tinyxml2::XMLElement* materialNode = root->FirstChildElement();

	while (materialNode)
	{
		shared_ptr<Material> material = make_shared<Material>();

		tinyxml2::XMLElement* node = nullptr;

		node = materialNode->FirstChildElement();
		material->SetName(Utils::ToWString(node->GetText()));

		// Diffuse Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				shared_ptr<Texture> texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetDiffuseMap(texture);
			}
		}

		// Specular Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				shared_ptr<Texture> texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetSpecularMap(texture);
			}
		}

		// Normal Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				shared_ptr<Texture> texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetNormalMap(texture);
			}
		}


		// Ambient
		{
			node = node->NextSiblingElement();
			
			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");

			material->GetMaterialDesc().ambient = color;
		}

		// Diffuse
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");

			material->GetMaterialDesc().diffuse = color;
		}

		// Specular
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");

			material->GetMaterialDesc().specular = color;
		}

		// Emissive
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");

			material->GetMaterialDesc().emissive = color;
		}

		_materials.push_back(material);

		// NextMaterial
		materialNode = materialNode->NextSiblingElement();
	}

	BindCacheInfo();
}

void Model::ReadModel(wstring fileName)
{
	wstring fullPath = _modelPath + fileName + L".mesh";

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	// Bones
	{
		const uint32 count = file->Read<uint32>();
		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ModelBone> bone = make_shared<ModelBone>();

			bone->name = Utils::ToWString(file->Read<string>());
			bone->index = file->Read<int32>();
			bone->parentIndex = file->Read<int32>();
			bone->transform = file->Read<Matrix>();

			_bones.push_back(bone);
		}
	}


	// Mesh
	{
		const uint32 count = file->Read<uint32>();
		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();

			mesh->name = Utils::ToWString(file->Read<string>());
			mesh->boneIndex = file->Read<int32>();
			mesh->materialName = Utils::ToWString(file->Read<string>());

			// VertexData
			{
				const uint32 count = file->Read<uint32>();
				vector<ModelVertexType> vertices;
				vertices.resize(count);

				file->Read((void*)vertices.data(), sizeof(ModelVertexType) * count); // 강의 코드랑 다름 (수정)
				mesh->geometry->AddVertices(vertices);
			}

			// IndexData
			{
				const uint32 count = file->Read<uint32>();

				vector<uint32> indices;
				indices.resize(count);

				file->Read((void*)indices.data(), sizeof(uint32) * count);
				mesh->geometry->AddIndices(indices);
			}

			mesh->CreateBuffers();

			_meshes.push_back(mesh);
		}
	}

	BindCacheInfo();
}

shared_ptr<Material> Model::GetMaterialByName(const wstring& name)
{
	for (const shared_ptr<Material>& material : _materials)
	{
		if (material->GetName() == name)
		{
			return material;
		}
	}

	return nullptr;
}

shared_ptr<ModelMesh> Model::GetMeshByName(const wstring& name)
{
	for (const shared_ptr<ModelMesh>& mesh : _meshes)
	{
		if (mesh->name == name)
		{
			return mesh;
		}
	}

	return nullptr;
}


shared_ptr<ModelBone> Model::GetBoneByName(const wstring& name)
{
	for (const shared_ptr<ModelBone>& bone : _bones)
	{
		if (bone->name == name)
		{
			return bone;
		}
	}

	return nullptr;
}

void Model::BindCacheInfo()
{
	for (const shared_ptr<ModelMesh>& mesh : _meshes)
	{
		if (mesh->material == nullptr) // Mesh 에 material 이 할당되지 않았다면, 캐싱
		{
			mesh->material = GetMaterialByName(mesh->materialName);
		}
		if (mesh->bone == nullptr) // Mesh 에 Bone 이 할당되지 않았다면, 캐싱
		{
			mesh->bone = GetBoneByIndex(mesh->boneIndex);
		}
	}


	// Bone 계층 정보 채우기
	if (_root == nullptr && _bones.size() > 0)
	{
		_root = _bones[0];

		for (const shared_ptr<ModelBone>& bone : _bones)
		{
			if (bone->parentIndex >= 0)
			{
				bone->parent = _bones[bone->parentIndex];
				bone->parent->children.push_back(bone);
			}
			else
			{
				bone->parent = nullptr;
			}
		}
	}
}
