#include "pch.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "Shader.h"
#include "Material.h"
#include "ModelMesh.h"

ModelRenderer::ModelRenderer(shared_ptr<Shader> shader) : Super(ComponentType::ModelRenderer), _shader(shader)
{
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::Update()
{
	if (_model == nullptr)
		return;

	// Bones
	{
		BoneDesc boneDesc;

		vector<shared_ptr<ModelBone>>& bones = _model->GetBone();
		const uint32 boneCount = _model->GetBoneCount();
		for (uint32 i = 0; i < boneCount; i++)
		{
			shared_ptr<ModelBone> bone = bones[i];
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

void ModelRenderer::SetModel(shared_ptr<Model> model)
{
	_model = model;

	vector<shared_ptr<Material>>& materials = _model->GetMaterials();
	for (shared_ptr<Material>& material : materials)
	{
		material->SetShader(_shader);
	}
}
