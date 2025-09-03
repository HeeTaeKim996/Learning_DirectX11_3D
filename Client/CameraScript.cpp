#include "pch.h"
#include "CameraScript.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"

void CameraScript::Start()
{

}

void CameraScript::Update()
{
	Vec3 pos = GetTransform()->GetPosition();
	if (INPUT->GetButton(KEY_TYPE::W))
	{
		pos += GetTransform()->GetLook() * _speed * DT;
	}
	else if (INPUT->GetButton(KEY_TYPE::S))
	{
		pos -= GetTransform()->GetLook() * _speed * DT;
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		pos += GetTransform()->GetRight() * _speed * DT;
	}
	else if (INPUT->GetButton(KEY_TYPE::A))
	{
		pos -= GetTransform()->GetRight() * _speed * DT;
	}
	GetTransform()->SetPosition(pos);


	Vec3 rotation = GetTransform()->GetLocalRotation();
	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		rotation.x += DT * 0.8f;
	}
	else if (INPUT->GetButton(KEY_TYPE::E))
	{
		rotation.x -= DT * 0.8f;
	}

	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		rotation.y += DT * 0.8f;
	}
	else if (INPUT->GetButton(KEY_TYPE::C))
	{
		rotation.y -= DT * 0.8f;
	}
	GetTransform()->SetRotation(rotation);
	
}
