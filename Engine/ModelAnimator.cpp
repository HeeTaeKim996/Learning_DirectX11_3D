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
	if (_texture == nullptr) // 초기화 작업에서 하는 게 좋을텐데. 우선 강의에서 Update에서 하니 Update 에서 처리
	{
		CreateTexture();
	}


	// GUI 로 _keyframeDesc 조작 (임시)
	{
		ImGui::InputInt("AnimIndex", &_keyframeDesc.animIndex);
		_keyframeDesc.animIndex %= _model->GetAnimationCount();
		ImGui::InputInt("CurrentFrame", (int*)&_keyframeDesc.currentFrame);
		_keyframeDesc.currentFrame %= _model->GetAnimationByIndex(_keyframeDesc.animIndex)->frameCount;
	}


	// 애니매이션 현재 프레임 정보
	RENDER->PushKeyframeData(_keyframeDesc);

	// SRV 를 통해 정보 전달
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

		/*	□ 애니매이션 데이터를 넘길 때 ConstantBuffer 가 아닌 Texture 를 사용
			 - 애니매이션의 Tranform 데이터는 양이 크므로, ConstantBuffer 는 용량 한계 (64KB 미만 가능) 가 있음
			 - 따라서 ConstantBuffer 가 아닌 Texture 를 사용해 애니매이션 데이터를 주로 넘김

			○ 넘기는 방법
			 - Matrix4x4 (float) 은 16바이트이고, DXGI_FORMAT_R32G32B32A32_FLOAT 로 할시, 한픽셀에 총 4바이트
			   를 넣을 수 있음. 따라서 4개의 픽셀에 하나의 Matrix 정보를 담을 수 있으니,
			   모든 모델의 Matrix 는 가로축으로, desc.Width = MAX_MODEL_TRANFSORMS * 4  로 담을 수 있음
			 - 세로축으로는, desc.Height = MAX_MODEL_KEYFRAMES 로 해당 가로의 정보들을, KEYFRAME 별로 모두 담음
			 - 앞선 가로, 세로로의 한 애니매이션에서의 모델, 키별 정보들을 담은 데이터들을,
			   desc.ArraySize = _model->GetAnimationCount(); 로 모든 애니매이션의 정보를 담을 수 있음
		*/



		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		void* mallocPtr = ::malloc(pageSize * _model->GetAnimationCount());

		// 파편화된 데이터를 조립한다
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


		// 리소스 만들기
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
				// 이미 기본 생성자로 처리됨				
			}


			// ※ 강의와 다르게 사용한다. 직접 만든 TransformBase 로 대체하여 사용함
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
