#include "pch.h"
#include "Converter.h"
#include <filesystem>
#include "Engine/Utils.h"
#include "Engine/tinyxml2.h"
#include "Engine/FileUtils.h"
#include "Engine/MyMathUtils.h"

Converter::Converter()
{
	_importer = make_shared<Assimp::Importer>();

}

Converter::~Converter()
{

}

void Converter::ReadAssetFile(wstring file)
{
	wstring fileStr = _assetPath + file;

	auto p = std::filesystem::path(fileStr);
	assert(std::filesystem::exists(p));

	_scene = _importer->ReadFile(
		Utils::ToString(fileStr),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	assert(_scene != nullptr);
}

void Converter::ExportModelData(wstring savePath)
{
	wstring finalPath = _modelPath + savePath + L".mesh";
	ReadModelData(_scene->mRootNode, -1, -1);
	WriteModelFile(finalPath);
}
void Converter::ExportMaterialData(wstring savePath)
{
	wstring finalPath = _texturePath + savePath + L".xml";
	ReadMaterialData();
	WriteMaterialData(finalPath);
}

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	shared_ptr<asBone> bone = make_shared<asBone>();

	bone->index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	DirectX::SimpleMath::Matrix dxMat(node->mTransformation[0]); 
	// ※ SimpleMath Matrix 는 float 주소 하나 주면, 채워주는 코드 있는데, myMatrix에는 안만들었어서, 이렇게 처리
	Matrix localTransform = SimpleMatrixToMyMatrix(dxMat);
	bone->transform = localTransform.Transpose(); // ※ FBX 는 col_major 로 작성되기에, Transpose 필요
	// ※ 여기서 얻는 SRT 는 로컬 트랜스폼

	Matrix parentWorld = Matrix::Identity;
	if (parent >= 0)
	{
		parentWorld = _bones[parent]->transform;
	}
	bone->transform = bone->transform * parentWorld; // bone->transform 에 로컬 트랜스폼이 아닌, 월드 트랜스폼을 저장
	_bones.push_back(bone);




	// Mesh
	ReadMeshData(node, index);



	// 재귀함수
	for (uint32 i = 0; i < node->mNumChildren; i++)
	{
		ReadModelData(node->mChildren[i], _bones.size(), index);
	}
}
void Converter::ReadMeshData(aiNode* node, int32 bone)
{
	if (node->mNumMeshes < 1) return;
	
	shared_ptr<asMesh> mesh = make_shared<asMesh>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		uint32 index = node->mMeshes[i];
		const aiMesh* srcMesh = _scene->mMeshes[index];

		// MaterialName
		const aiMaterial* material = _scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();



		for (uint32 v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex
			VertexType vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vec3));

			// UV
			if (srcMesh->HasTextureCoords(0))
			{
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vec2));
				/*※ UV 맵을 여러장 사용하는 것에 대비해, [][] 2차원 배열이지만, 대부분 UV를 한장 사용하기 때문에
					mTextureCoords[0][n] 의 형태로 거의 고정되어 사용 */
			}

			// Normal
			if (srcMesh->HasNormals())
			{
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vec3));
			}

			mesh->vertices.push_back(vertex);
		}
	}
}
void Converter::WriteModelFile(wstring finalPath)
{

}

void Converter::ReadMaterialData()
{
	for (uint32 i = 0; i < _scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = _scene->mMaterials[i];

		shared_ptr<asMaterial> material = make_shared<asMaterial>();

		material->name = srcMaterial->GetName().C_Str();


		{
			aiColor3D color;
			// Ambient
			srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
			material->ambient = Color(color.r, color.g, color.b, 1.f);

			// Diffuse
			srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			material->diffuse = Color(color.r, color.g, color.b, 1.f);

			// Sepcular
			srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			material->specular = Color(color.r, color.g, color.b, 1.f);
			srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w); // ※ 여기서 w는 alpha 가 아닌, 반사광의 강도 제곱승값으로 사용

			// Emissive
			srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			material->emissive = Color(color.r, color.g, color.b, 1.f);
		}

		{
			aiString file;

			// Diffuse Texture
			srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			material->diffuseFile = file.C_Str();

			// Specular Texture
			srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
			material->specularFile = file.C_Str();

			// Normal Texture
			srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
			material->normalFile = file.C_Str();
		}

		_materials.push_back(material);
	}

}
void Converter::WriteMaterialData(wstring finalPath)
{
	filesystem::path path = filesystem::path(finalPath);

	filesystem::create_directory(path.parent_path()); 
	// ※ 최종이 확장자이니, parent_path() 는 확장자가 담긴 파일명. 위 코드는 해당 파일을 생성하는 코드 (이미 있다면 X)

	string folder = path.parent_path().string();

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	for (shared_ptr<asMaterial> material : _materials)
	{
		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		tinyxml2::XMLElement* element = nullptr;

		element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->normalFile).c_str());
		node->LinkEndChild(element);


		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->emissive.x);
		element->SetAttribute("G", material->emissive.y);
		element->SetAttribute("B", material->emissive.z);
		element->SetAttribute("A", material->emissive.w);
		node->LinkEndChild(element);
	}

	document->SaveFile(Utils::ToString(finalPath).c_str());
}
string Converter::WriteTexture(string saveFolder, string file)
{
	string fileName = filesystem::path(file).filename().string();
	string folderName = filesystem::path(saveFolder).filename().string();

	const aiTexture* srcTexture = _scene->GetEmbeddedTexture(file.c_str());
	if (srcTexture)
	{
		string pathStr = saveFolder + fileName;

		if (srcTexture->mHeight == 0)
		{
			shared_ptr<FileUtils> file = make_shared<FileUtils>();
			file->Open(Utils::ToWString(pathStr), FileMode::Write);
			file->Write(srcTexture->pcData, srcTexture->mWidth);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = srcTexture->mWidth;
			desc.Height = srcTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = srcTexture->pcData;

			ComPtr<ID3D11Texture2D> texture;
			HRESULT hr = DEVICE->CreateTexture2D(&desc, &subResource, texture.GetAddressOf());
			CHECK(hr);

			DirectX::ScratchImage img;
			::CaptureTexture(DEVICE.Get(), DC.Get(), texture.Get(), img);

			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, Utils::ToWString(fileName).c_str());
			CHECK(hr);
		}
	}
	else
	{
		string originStr = (filesystem::path(_assetPath) / folderName / file).string();
		Utils::Replace(originStr, "\\", "/");

		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		::CopyFileA(originStr.c_str(), pathStr.c_str(), false);
	}

	return fileName;

	/*
		- 총 3분기로 FBX 에 담긴 텍스쳐 정보를 새로 지정한 폴더에 PNG JPG DDS 파일 중 하나로 복사하고, XML 에 담기 위한
		  복사된 파일이 있는 경로 주소를 string 으로 리턴하는 함수
		
		- 3분기별로 나누어 설명하면,
		 1) FBX 파일 내에 JPG 또는 PNG 파일이 위치한 경우
		  - 조건은 if(srcTexture) && if(srcTexture->mHeight == 0) 
		  - 텍스쳐로 PNG, JPG 로 압축되어 저장된 경우, mHeight 가 계산되지 않은 디펄트 상태(0) 이므로, mHeight == 0 이라면,
		    압축이 풀리기 전인 PNG JPG 상태이므로, FBX 내 파일(PNG JPG)을 저장 경로에 복사후, 경로만 리턴
		 2) (PNG, JPG) 파일의 압축이 풀린 상태로, FBX 파일 내에 위치한 경우
		  - 조건은 if(srcTexture) && else
		  - PNG, JPG 의 압축이 풀려, [r8.0] [g8.0] [b8.0] [r1.0] ... 형태의 배열로 데이터가 저장된 경우에 해당
		  - 해당 경우, mHeight 가 계산되므로, > 0 인 경우에 해당
		  - 배열 데이터를 CreateTexture2D로 텍스쳐로 만든 후, CaptureTexture 로 GPU 의 데이터를 CPU 로 가져와 Img 로 만든 후,
		    SaveToDDSFile 로 Img 를 DDS (DirectX 의 이미지 저장 형식) 로 해당 경로에 저장 및 저장 경로를 리턴
		 3) FBX 파일 밖에 PNG JPG 가 위치한 경우
		  - 조건은 else 
		  - 가장 흔한 형태로, FBX 파일 밖에 PNG JPG 가 위치한 경우. (Mesh AI 도 이런 형태로 줌)
		  - CopyFileA 로 기존 경로의 JPG PNG 를 새로운 저장 경로에 복사하고, 경로를 리턴

	
		※ 위 3경로 모두, 텍스쳐 정보가 정점이 아닌 별도의 파일로 저장됨
		   이전에 MeshAI 에서 받은 오브젝트에, 땜빵으로 면에 색을 칠해서, 기존 텍스쳐에 오버라이드돼서 사용했던 경우는 위 3분기
		   에 해당되지 않은 형태로, 정점에 컬러 정보가 담겨 있는 경우 
		   ( asMesh::mColors 같은 형태로 데이터가 담김. 현재 작성 기준 구조체에 없으므로, 그런 형태는 지원 안하는 듯 )
	
	*/
}