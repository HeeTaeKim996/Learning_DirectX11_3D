#include "pch.h"
#include "RenderManager.h"
//#include "Camera.h"
#include "Camera.h"

void RenderManager::Init(shared_ptr<Shader> shader)
{
	_shader = shader;

	_globalBuffer = make_shared<ConstantBuffer<GlobalDesc>>();
	_globalBuffer->Create();
	_globalEffectBuffer = _shader->GetConstantBuffer("GlobalBuffer");

	_transformBuffer = make_shared<ConstantBuffer<TransformDesc>>();
	_transformBuffer->Create();
	_transformEffectBuffer = _shader->GetConstantBuffer("TransformBuffer");

	_lightBuffer = make_shared<ConstantBuffer<LightDesc>>();
	_lightBuffer->Create();
	_lightEffectBuffer = _shader->GetConstantBuffer("LightBuffer");

	_materialBuffer = make_shared<ConstantBuffer<MaterialDesc>>();
	_materialBuffer->Create();
	_materialEffectBuffer = _shader->GetConstantBuffer("MaterialBuffer");

}

void RenderManager::Update() // TEMP
{
	PushGlobalData(Camera::S_MatView, Camera::S_MatProjection, Camera::S_MatVInv);
}

void RenderManager::PushGlobalData(const Matrix& view, const Matrix& projection, const Matrix& VInv)
	// ※ 강의에는 VInv를 view.Invert로 하는데.. 효율 너무 안좋아보여서, 이렇게 매개변수로 받게 처리함
{
	_globalDesc.V = view;
	_globalDesc.P = projection;
	_globalDesc.VP = view * projection;
	_globalDesc.VInv = VInv;
	_globalBuffer->CopyData(_globalDesc);
	_globalEffectBuffer->SetConstantBuffer(_globalBuffer->GetComPtr().Get());
}

void RenderManager::PushTransformData(const TransformDesc& desc)
{
	_transformDesc = desc;
	_transformBuffer->CopyData(_transformDesc);
	_transformEffectBuffer->SetConstantBuffer(_transformBuffer->GetComPtr().Get());
}

void RenderManager::PushLightData(const LightDesc& desc)
{
	_lightDesc = desc;
	_lightBuffer->CopyData(_lightDesc);
	_lightEffectBuffer->SetConstantBuffer(_lightBuffer->GetComPtr().Get());
}

void RenderManager::PushMaterialData(const MaterialDesc& desc)
{
	_materialDesc = desc;
	_materialBuffer->CopyData(_materialDesc);
	_materialEffectBuffer->SetConstantBuffer(_materialBuffer->GetComPtr().Get());
}
	