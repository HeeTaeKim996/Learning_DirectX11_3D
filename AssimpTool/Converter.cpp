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



















/*------------------------
		ModelData
------------------------*/
void Converter::ExportModelData(wstring savePath)
{
	wstring finalPath = _modelPath + savePath + L".mesh";
	ReadModelData(_scene->mRootNode, -1, -1);
	ReadSkinData();

	{ // ����� �뵵 ��Ų������ CSV ���� ����� ( �Ʒ� WriteModelFile �� ���̳ʸ� �����ͷ� �ǵ����ʹ� ����� )

		FILE* file;
		::fopen_s(&file, "../Vertices.csv", "w");

		::fprintf(file, "Index, Name\n");
		for (const shared_ptr<asBone>& bone : _bones)
		{
			string name = bone->name;
			::fprintf(file, "%d,%s\n", bone->index, bone->name.c_str());
		}

		::fprintf(file, "\n");

		for (const shared_ptr<asMesh>& mesh : _meshes)
		{
			fprintf(file, "\n%s\n", mesh->name.c_str());
			fprintf(file,
				"posX, posY, posZ, index1, index2, index3, index4, weight1, weight2, weight3, weight4\n");
			for (UINT i = 0; i < mesh->vertices.size(); i++)
			{
				Vec3 p = mesh->vertices[i].position;
				Vec4 indices = mesh->vertices[i].blendIndices;
				Vec4 weights = mesh->vertices[i].blendWeights;

				::fprintf(file, "%f, %f, %f,", p.X, p.Y, p.Z);
				::fprintf(file, "%f, %f, %f, %f,", indices.X, indices.Y, indices.Z, indices.W);
				::fprintf(file, "%f, %f, %f, %f\n", weights.X, weights.Y, weights.Z, weights.W);
			}
		}

		::fclose(file);
	}


	WriteModelFile(finalPath);
}
void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	shared_ptr<asBone> bone = make_shared<asBone>();

	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	DirectX::SimpleMath::Matrix dxMat(node->mTransformation[0]); 
	 //�� SimpleMath Matrix �� float �ּ� �ϳ� �ָ�, ä���ִ� �ڵ� �ִµ�, myMatrix���� �ȸ�����, �̷��� ó��
	Matrix localTransform = MyMathUtils::SimpleMatrixToMyMatrix(dxMat);
	bone->transform = localTransform.Transpose(); // �� FBX �� col_major �� �ۼ��Ǳ⿡, Transpose �ʿ�
	// �� ���⼭ ��� SRT �� ���� Ʈ������

	Matrix parentWorld = Matrix::Identity;
	if (parent >= 0)
	{
		parentWorld = _bones[parent]->transform;
	}
	bone->transform = bone->transform * parentWorld; 
	// �� bone->transform �� ���� Ʈ�������� �ƴ�, ���� Ʈ�������� ����. FBX���� ���� transform �� ����Ʈ������ ����
	_bones.push_back(bone);




	// Mesh
	ReadMeshData(node, index);



	// ����Լ�
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

		const uint32 startVertex = mesh->vertices.size();

		// Vertex
		for (uint32 v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex
			VertexType vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vec3));

			// UV
			if (srcMesh->HasTextureCoords(0))
			{
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vec2));
				/*�� UV ���� ������ ����ϴ� �Ϳ� �����, [][] 2���� �迭������, ��κ� UV�� ���� ����ϱ� ������
					mTextureCoords[0][n] �� ���·� ���� �����Ǿ� ��� */
			}

			// Normal
			if (srcMesh->HasNormals())
			{
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vec3));
			}

			mesh->vertices.push_back(vertex);
		}

		// Index
		for (uint32 f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (uint32 k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices.push_back(face.mIndices[k] + startVertex);
			}
		}

		_meshes.push_back(mesh);
	}
}
void Converter::ReadSkinData()
{
	/* �� FBX ���� �������� ������ Weight �� ������ִ� �� �ƴ϶�, ������ Weight > 0 �� �������� ��ϵ��ֱ� ������,
		 �ڵ尡 �Ʒ��� ���� */

	for (uint32 i = 0; i < _scene->mNumMeshes; i++)
	{
		aiMesh* srcMesh = _scene->mMeshes[i];
		if (srcMesh->HasBones() == false)
		{
			continue;
		}

		shared_ptr<asMesh> mesh = _meshes[i];

		vector<asBoneWeight> tempVertexBoneWeights;
		tempVertexBoneWeights.resize(mesh->vertices.size());


		// Bone �� ��ȸ�ϸ鼭 ������ VertexId, Weight �� ���ؼ� ����Ѵ�
		for (uint32 b = 0; b < srcMesh->mNumBones; b++)
		{
			aiBone* srcMeshBone = srcMesh->mBones[b];
			uint32 boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());

			for (uint32 w = 0; w < srcMeshBone->mNumWeights; w++)
			{
				uint32 index = srcMeshBone->mWeights[w].mVertexId;
				float weight = srcMeshBone->mWeights[w].mWeight;

				// TODO
				tempVertexBoneWeights[index].AddWeights(boneIndex, weight);
			}
		}

		// ���� ��� ���
		for (uint32 v = 0; v < tempVertexBoneWeights.size(); v++)
		{
			tempVertexBoneWeights[v].Normalize();

			asBlendWeight blendWeight = tempVertexBoneWeights[v].GetBlendWeight();
			mesh->vertices[v].blendIndices = blendWeight.indices;
			mesh->vertices[v].blendWeights = blendWeight.weights;
		}
	}
		



}
void Converter::WriteModelFile(wstring finalPath)
{
	filesystem::path path = filesystem::path(finalPath);

	// ������ ������ �����
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	// Bone Data
	file->Write<uint32>(_bones.size());
	for (const shared_ptr<asBone>& bone : _bones)
	{
		file->Write<string>(bone->name);
		file->Write<int32>(bone->index);
		file->Write<int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}


	// Mesh Data
	file->Write<uint32>(_meshes.size());
	for (const shared_ptr<asMesh>& mesh : _meshes)
	{
		file->Write<string>(mesh->name);
		file->Write<int32>(mesh->boneIndex);
		file->Write<string>(mesh->materialName);

		// VertexDatas
		uint32 verticesSize = mesh->vertices.size();
		file->Write<uint32>(verticesSize);
		file->Write(&mesh->vertices[0], sizeof(VertexType) * verticesSize);

		// IndexDatas
		uint32 indicesSize = mesh->indices.size();
		file->Write<uint32>(indicesSize);
		file->Write(&mesh->indices[0], sizeof(uint32) * indicesSize);
	}

}





























/*-------------------
	 MaterialData
-------------------*/
void Converter::ExportMaterialData(wstring savePath)
{
	wstring finalPath = _texturePath + savePath + L".xml";
	ReadMaterialData();
	WriteMaterialData(finalPath);
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
			srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w); // �� ���⼭ w�� alpha �� �ƴ�, �ݻ籤�� ���� �����°����� ���

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
	// �� ������ Ȯ�����̴�, parent_path() �� Ȯ���ڰ� ��� ���ϸ�. �� �ڵ�� �ش� ������ �����ϴ� �ڵ� (�̹� �ִٸ� X)

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
		//string pathStr = saveFolder + fileName;
		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		/*	�� string �� + �� filesystem �� /
			 - ���� �ּ��� �޸� �κ����� pathStr �� �����, saveFolder�� ���� \ �� �ƴҽ�, ������ fileNAme �� �ܼ���
			   ��������, ���ϸ��� �̻�������.
			 - �ݸ� filesystem �� / ��, \ �� ������, \ �� �߰��Ͽ� ���� �� �����ϰ�, \ �� �̹� ������, + �� �����ϰ�
			   �۵��Ѵ�
			 - ���� filesystem �� / �� �ֿ�����
		*/

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
		- �� 3�б�� FBX �� ��� �ؽ��� ������ ���� ������ ������ PNG JPG DDS ���� �� �ϳ��� �����ϰ�, XML �� ��� ����
		  ����� ������ �ִ� ��� �ּҸ� string ���� �����ϴ� �Լ�
		
		- 3�б⺰�� ������ �����ϸ�,
		 1) FBX ���� ���� JPG �Ǵ� PNG ������ ��ġ�� ���
		  - ������ if(srcTexture) && if(srcTexture->mHeight == 0) 
		  - �ؽ��ķ� PNG, JPG �� ����Ǿ� ����� ���, mHeight �� ������ ���� ����Ʈ ����(0) �̹Ƿ�, mHeight == 0 �̶��,
		    ������ Ǯ���� ���� PNG JPG �����̹Ƿ�, FBX �� ����(PNG JPG)�� ���� ��ο� ������, ��θ� ����
		 2) (PNG, JPG) ������ ������ Ǯ�� ���·�, FBX ���� ���� ��ġ�� ���
		  - ������ if(srcTexture) && else
		  - PNG, JPG �� ������ Ǯ��, [r8.0] [g8.0] [b8.0] [r1.0] ... ������ �迭�� �����Ͱ� ����� ��쿡 �ش�
		  - �ش� ���, mHeight �� ���ǹǷ�, > 0 �� ��쿡 �ش�
		  - �迭 �����͸� CreateTexture2D�� �ؽ��ķ� ���� ��, CaptureTexture �� GPU �� �����͸� CPU �� ������ Img �� ���� ��,
		    SaveToDDSFile �� Img �� DDS (DirectX �� �̹��� ���� ����) �� �ش� ��ο� ���� �� ���� ��θ� ����
		 3) FBX ���� �ۿ� PNG JPG �� ��ġ�� ���
		  - ������ else 
		  - ���� ���� ���·�, FBX ���� �ۿ� PNG JPG �� ��ġ�� ���. (Mesh AI �� �̷� ���·� ��)
		  - CopyFileA �� ���� ����� JPG PNG �� ���ο� ���� ��ο� �����ϰ�, ��θ� ����

	
		�� �� 3��� ���, �ؽ��� ������ ������ �ƴ� ������ ���Ϸ� �����
		   ������ MeshAI ���� ���� ������Ʈ��, �������� �鿡 ���� ĥ�ؼ�, ���� �ؽ��Ŀ� �������̵�ż� ����ߴ� ���� �� 3�б�
		   �� �ش���� ���� ���·�, ������ �÷� ������ ��� �ִ� ��� 
		   ( asMesh::mColors ���� ���·� �����Ͱ� ���. ���� �ۼ� ���� ����ü�� �����Ƿ�, �׷� ���´� ���� ���ϴ� �� )
	

		��	FileUtils �� ���̳ʸ� �����͸� �а�/���� ���� ���� ���� Ŭ�����̴�. PNG, JPG Ŭ���� ���� ���̳ʸ� �����ͷ�,
			�б�, ���Ⱑ �����ϴ�
	*/
}


























/*----------------------
	  AnimationData
----------------------*/

void Converter::ExportAnimationData(wstring savePath, uint32 index/*=0*/)
{
	wstring finalPath = _modelPath + savePath + L".clip";
	assert(index < _scene->mNumAnimations);
	shared_ptr<asAnimation> animation = ReadAnimationData(_scene->mAnimations[index]);
	WriteAnimationData(animation, finalPath);
}
shared_ptr<asAnimation> Converter::ReadAnimationData(aiAnimation* srcAnimation)
{
	shared_ptr<asAnimation> animation = make_shared<asAnimation>();
	animation->name = srcAnimation->mName.C_Str();
	animation->frameRate = srcAnimation->mTicksPerSecond;
	animation->frameCount = (uint32)srcAnimation->mDuration + 1;
	/* �� mDuration �� ������ �� �� �ƴ� ������ ����.���� �ִϸ��̼��� ��ü �ð�(��) �� mDuration�� �ƴ�,
		  mDuration / mTickPerSecond �̴�.
		  frameCount �� + 1 �� �� ������ mDuration�� ���� ������ �ε����� 0�̱� ����
		  
		  �� ��Ȯ���� mDuration �� ������ �������� �ƴ� Tick �̶� �ϴµ�, ������ �̶�� ���信 �� ����� �� ���� */

	map<string, shared_ptr<vector<asKeyframeData>>> cacheAnimNodes;

	for (uint32 i = 0; i < srcAnimation->mNumChannels; i++)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// �ִϸ��̼� ��� ������ �Ľ�
		shared_ptr<vector<asKeyframeData>> keyframeDatas = ParseAnimationNode(animation, srcNode);

		// ���� ã�� ��� �߿� ���� �� �ð����� �ִϸ��̼� �ð� ����
		animation->duration = max(animation->duration, keyframeDatas->back().time);

		cacheAnimNodes[srcNode->mNodeName.C_Str()] = keyframeDatas;
	}

	ReadKeyframeData(animation, _scene->mRootNode, cacheAnimNodes);

	return animation;
}

shared_ptr<vector<asKeyframeData>> Converter::ParseAnimationNode(shared_ptr<asAnimation> animation, 
	aiNodeAnim* srcNode)
{
	shared_ptr<vector<asKeyframeData>> keyframeDatas = make_shared<vector<asKeyframeData>>();
	
	uint32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumRotationKeys),
		srcNode->mNumScalingKeys);

	for (uint32 k = 0; k < keyCount; k++)
	{
		asKeyframeData frameData;

		bool found = false;
		uint32 t = keyframeDatas->size();

		// Position
		if (::fabsf((float)srcNode->mPositionKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mPositionKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.translation, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// Rotation
		if (::fabsf((float)srcNode->mRotationKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			frameData.time = (float)key.mTime;

			frameData.rotation.X = key.mValue.x;
			frameData.rotation.Y = key.mValue.y;
			frameData.rotation.Z = key.mValue.z;
			frameData.rotation.W = key.mValue.w;

			found = true;
		}

		// Scale
		if (::fabsf((float)srcNode->mScalingKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.scale, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		if (found == true)
		{
			keyframeDatas->push_back(frameData);
		}
	}

	// Keyframe �÷��ֱ�
	if (keyframeDatas->size() < animation->frameCount)
	{
		uint32 count = animation->frameCount - keyframeDatas->size();
		asKeyframeData keyframe = keyframeDatas->back();

		for (uint32 n = 0; n < count; n++)
		{ 
			keyframeDatas->push_back(keyframe);
		}
	}

	return keyframeDatas;


	/*
		- ���� ��� ���� ��������, ������ ���� �ʴ´�. mPositionKeys || mRotationKeys || mScalingKeys �� �ϳ��� 
		  ��ҵ鰣�� mTime�� ���̰� 1�̸��̶��, ���Ŀ��� ��� ��ҵ��� ������ ���� �����ٵ�. if �� �ƴ� while�� �������
		  �ʾƵ� �Ǵ� ������ �ִٴ� ����
	*/
}

void Converter::ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* srcNode,
	map<string, shared_ptr<vector<asKeyframeData>>>& cache)
{
	shared_ptr<asKeyframe> keyframe = make_shared<asKeyframe>();
	keyframe->boneName = srcNode->mName.C_Str();

	shared_ptr<vector<asKeyframeData>> findKeyframes = cache[srcNode->mName.C_Str()];
	
	for (uint32 i = 0; i < animation->frameCount; i++)
	{
		asKeyframeData frameData;

		if (findKeyframes == nullptr)
		{
			frameData.time = (float)i;
			Matrix transform(srcNode->mTransformation[0]);
			transform = transform.Transpose();
			transform.Decompose(OUT frameData.scale, OUT frameData.rotation, OUT frameData.translation);
		}
		else
		{
			frameData = (*findKeyframes)[i];
			frameData.rotation = frameData.rotation.Inverse();
		}

		keyframe->transforms.push_back(frameData);
	}

	// �ִϸ��̼� Ű������ ä���
	animation->keyframes.push_back(keyframe);

	for (uint32 i = 0; i < srcNode->mNumChildren; i++)
	{
		ReadKeyframeData(animation, srcNode->mChildren[i], cache);
	}
} 

void Converter::WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath)
{
	filesystem::path path = filesystem::path(finalPath);

	// ������ ������ �����
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	file->Write<string>(animation->name);
	file->Write<float>(animation->duration);
	file->Write<float>(animation->frameRate);
	file->Write<uint32>(animation->frameCount);

	file->Write<uint32>(animation->keyframes.size());

	for (shared_ptr<asKeyframe> keyframe : animation->keyframes)
	{
		file->Write<string>(keyframe->boneName);

		file->Write<uint32>(keyframe->transforms.size());
		file->Write(&keyframe->transforms[0], sizeof(asKeyframeData) * keyframe->transforms.size());
	}
}

uint32 Converter::GetBoneIndex(const string& name)
{
	for (shared_ptr<asBone>& bone : _bones)
	{
		if (bone->name == name)
		{
			return bone->index;
		}
	}

	assert(0);
	return 0;
}
