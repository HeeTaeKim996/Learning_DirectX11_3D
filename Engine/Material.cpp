#pragma once;
#include "pch.h"
#include "Material.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		더이상 _shader 들 고 있을 필요 없지 않나. ModelRenderer 가 shader로 그리고, Material 을 Texture 별 버퍼를 들고 잇는데
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
Material::Material() : Super(ResourceType::Material)
{

}

Material::~Material()
{
}

void Material::SetShader(shared_ptr<Shader> shader)
{
	_shader = shader;

	_diffuseEffectBuffer = shader->GetSRV("DiffuseMap");
	_normalEffectBuffer = shader->GetSRV("NormalMap");
	_specularEffectBuffer = _shader->GetSRV("SpecularMap");

}

void Material::Update()
{
	if (_shader == nullptr)
	{
		return;
	}

	RENDER->PushMaterialData(_desc);

	if (_diffuseMap)
	{
		_diffuseEffectBuffer->SetResource(_diffuseMap->GetComPtr().Get());
	}
	if (_normalMap)
	{
		_normalEffectBuffer->SetResource(_normalMap->GetComPtr().Get());
	}
	if (_specularMap)
	{
		_specularEffectBuffer->SetResource(_specularMap->GetComPtr().Get());
	}
}

shared_ptr<Material> Material::Clone()
{
	//return make_shared<Material>(*this);

	shared_ptr<Material> material = make_shared<Material>();

	material->_desc					= _desc;
	material->_shader				= _shader;
	material->_diffuseMap			= _diffuseMap;
	material->_normalMap			= _normalMap;
	material->_specularMap			= _specularMap;
	material->_diffuseEffectBuffer	= _diffuseEffectBuffer;
	material->_normalEffectBuffer	= _normalEffectBuffer;
	material->_specularEffectBuffer = _specularEffectBuffer;

	return material;
}
