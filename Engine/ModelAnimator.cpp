#include "pch.h"
#include "ModelAnimator.h"
#include "Material.h"
#include "ModelMesh.h"
#include "Model.h"
#include "ModelAnimation.h"

ModelAnimator::ModelAnimator(shared_ptr<Shader> shader) : Super(ComponentType::Animator), _shader(shader)
{
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::Update()
{
	if (_model == nullptr)
		return;
	if (_texture == nullptr) // �ʱ�ȭ �۾����� �ϴ� �� �����ٵ�. �켱 ���ǿ��� Update���� �ϴ� Update ���� ó��
	{
		CreateTexture();
	}


	// GUI �� _keyframeDesc ���� (�ӽ�)
	{
		ImGui::InputInt("AnimIndex", &_keyframeDesc.animIndex);
		_keyframeDesc.animIndex %= _model->GetAnimationCount();
		ImGui::InputInt("CurrentFrame", (int*)&_keyframeDesc.currentFrame);
		_keyframeDesc.currentFrame %= _model->GetAnimationByIndex(_keyframeDesc.animIndex)->frameCount;
	}


	// �ִϸ��̼� ���� ������ ����
	RENDER->PushKeyframeData(_keyframeDesc);

	// SRV �� ���� ���� ����
	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());


	// Bones
	{
		BoneDesc boneDesc;

		vector<shared_ptr<ModelBone>>& bones = _model->GetBone();
		const uint32 boneCount = _model->GetBoneCount();
		for (uint32 i = 0; i < boneCount; i++)
		{
			shared_ptr<ModelBone> bone = bones[i];
			//boneDesc.transforms[i] = bone->transform;
			boneDesc.transforms[i] = bone->transform;
		}
		RENDER->PushBoneData(boneDesc);
	}



	// Transform
	{
		Matrix world = GetTransform()->GetSRT();
		RENDER->PushTransformData(TransformDesc{ world });

		vector<shared_ptr<ModelMesh>>& meshes = _model->GetMeshes();
		for (shared_ptr<ModelMesh>& mesh : meshes)
		{
			if (mesh->material)
			{
				mesh->material->Update();
			}


			// BoneIndex
			_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);


			uint32 stride = mesh->vertexBuffer->GetStride();
			uint32 offset = mesh->vertexBuffer->GetOffset();

			DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
			DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

			_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
		}
	}
}

void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	_model = model;

	vector<shared_ptr<Material>>& materials = _model->GetMaterials();
	for (shared_ptr<Material>& material : materials)
	{
		material->SetShader(_shader);
	}
}

void ModelAnimator::CreateTexture()
{
	if (_model->GetAnimationCount() == 0)
	{
		return;
	}

	_animTransforms.resize(_model->GetAnimationCount());

	for (uint32 i = 0; i < _model->GetAnimationCount(); i++)
	{
		CreateAnimationTransform(i);
	}

	// Create Texture
	{

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = _model->GetAnimationCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		/*	�� �ִϸ��̼� �����͸� �ѱ� �� ConstantBuffer �� �ƴ� Texture �� ���
			 - �ִϸ��̼��� Tranform �����ʹ� ���� ũ�Ƿ�, ConstantBuffer �� �뷮 �Ѱ� (64KB �̸� ����) �� ����
			 - ���� ConstantBuffer �� �ƴ� Texture �� ����� �ִϸ��̼� �����͸� �ַ� �ѱ�

			�� �ѱ�� ���
			 - Matrix4x4 (float) �� 16����Ʈ�̰�, DXGI_FORMAT_R32G32B32A32_FLOAT �� �ҽ�, ���ȼ��� �� 4����Ʈ
			   �� ���� �� ����. ���� 4���� �ȼ��� �ϳ��� Matrix ������ ���� �� ������,
			   ��� ���� Matrix �� ����������, desc.Width = MAX_MODEL_TRANFSORMS * 4  �� ���� �� ����
			 - ���������δ�, desc.Height = MAX_MODEL_KEYFRAMES �� �ش� ������ ��������, KEYFRAME ���� ��� ����
			 - �ռ� ����, ���η��� �� �ִϸ��̼ǿ����� ��, Ű�� �������� ���� �����͵���,
			   desc.ArraySize = _model->GetAnimationCount(); �� ��� �ִϸ��̼��� ������ ���� �� ����
		*/



		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		void* mallocPtr = ::malloc(pageSize * _model->GetAnimationCount());

		// ����ȭ�� �����͸� �����Ѵ�
		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			uint32 startOffset = c * pageSize;
			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++)
			{
				void* ptr = static_cast<void*>(pageStartPtr + f * dataSize);
				::memcpy(ptr, _animTransforms[c].transforms[f].data(), dataSize);
			}
		}


		// ���ҽ� �����
		vector<D3D11_SUBRESOURCE_DATA> subResource(_model->GetAnimationCount());

		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResource[c].pSysMem = ptr;
			subResource[c].SysMemPitch = dataSize;
			subResource[c].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = DEVICE->CreateTexture2D(&desc, subResource.data(), _texture.GetAddressOf());
		CHECK(hr);

		::free(mallocPtr);


		// Create SRV
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.MipLevels = 1;
			desc.Texture2DArray.ArraySize = _model->GetAnimationCount();

			HRESULT hr = DEVICE->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf());
			CHECK(hr);
		}
	}
}

void ModelAnimator::CreateAnimationTransform(uint32 index)
{
	vector<TransformBase> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS);

	shared_ptr<ModelAnimation> animation = _model->GetAnimationByIndex(index);

	for (uint32 f = 0; f < animation->frameCount; f++)
	{
		for (uint32 b = 0; b < _model->GetBoneCount(); b++)
		{
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(b);


			TransformBase animLocal;


			shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);
			if (frame != nullptr)
			{
				ModelKeyframeData& data = frame->transforms[f];
				animLocal = TransformBase(data.translation, data.rotation, data.scale);
			}
			else
			{
				// �̹� �⺻ �����ڷ� ó����				
			}


			// �� ���ǿ� �ٸ��� ����Ѵ�. ���� ���� TransformBase �� ��ü�Ͽ� �����
			int32 parentIndex = bone->parentIndex;

			TransformBase parentAnimWorld;
			if (parentIndex >= 0)
			{
				parentAnimWorld = tempAnimBoneTransforms[parentIndex];
			}

			tempAnimBoneTransforms[b] = animLocal.LocalToWorld(parentAnimWorld);
			

			TransformBase worldTransform(bone->transform);
			TransformBase invWorldTransform = worldTransform.Inverse();

			_animTransforms[index].transforms[f][b]
				= (invWorldTransform.GetSRT() * tempAnimBoneTransforms[b].GetSRT());



// DEBUG
#include "ETCUtils.h"
			if (f == 1 && b == 1)
			{
				FILE* file;
				::fopen_s(&file, "../AnimData7.csv", "w");

				::fprintf(file, "quaternion\n");
				shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);
				ModelKeyframeData& data = frame->transforms[f];
				::fprintf(file, "%f, %f, %f, %f\n",
					data.rotation.X, data.rotation.Y, data.rotation.Z, data.rotation.W);
				
				::fprintf(file, "1\n");
				ETCUtils_AddMyMatrixToCls(file, animLocal.GetSRT());

				::fprintf(file, "2\n");
				ETCUtils_AddMyMatrixToCls(file, parentAnimWorld.GetSRT());


				::fprintf(file, "invMat\n");
				myMatrix4x4 invMat = invWorldTransform.GetSRT();
				for (int i = 0; i < 4; i++)
				{
					::fprintf(file, "%f, %f, %f, %f \n",
						invMat[i][0], invMat[i][1], invMat[i][2], invMat[i][3]);
				}

				
				::fprintf(file, "AnimMat\n");
				myMatrix4x4 animMat = tempAnimBoneTransforms[b].GetSRT();
				for (int i = 0; i < 4; i++)
				{
					::fprintf(file, "%f, %f, %f, %f \n",
						animMat[i][0], animMat[i][1], animMat[i][2], animMat[i][3]);
				}


				::fprintf(file, "FinalMat\n");
				myMatrix4x4 finalMat = _animTransforms[index].transforms[f][b];
				for (int i = 0; i < 4; i++)
				{
					::fprintf(file, "%f, %f, %f, %f \n",
						finalMat[i][0], finalMat[i][1], finalMat[i][2], finalMat[i][3]
					);
				}

				fprintf(file, "\n");


				::fclose(file);



			}
		}
	}
}
