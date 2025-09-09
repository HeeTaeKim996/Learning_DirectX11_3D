#include "pch.h"
#include "CameraScript.h"
#include "Engine/InputManager.h"
#include "Engine/TimeManager.h"

void CameraScript::Start()
{
	_mousePos = INPUT->GetMousePos();
}

void CameraScript::Update()
{
	if (INPUT->GetButton(KEY_TYPE::W))
	{
		GetTransform()->AddPosition(GetTransform()->GetZAxis() * _speed * DT);
	}
	else if (INPUT->GetButton(KEY_TYPE::S))
	{
		GetTransform()->AddPosition(-GetTransform()->GetZAxis() * _speed * DT);
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		GetTransform()->AddPosition(GetTransform()->GetXAxis() * _speed * DT);
	}
	else if (INPUT->GetButton(KEY_TYPE::A))
	{
		GetTransform()->AddPosition(-GetTransform()->GetXAxis() * _speed * DT);
	}

	
	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		GetTransform()->AddPitchRotation(40.f * DT);	

	}
	else if (INPUT->GetButton(KEY_TYPE::E))
	{
		GetTransform()->AddPitchRotation(-40.f * DT);
	}

	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		GetTransform()->AddYawRotation(40.f * DT);
	}
	else if (INPUT->GetButton(KEY_TYPE::C))
	{
		GetTransform()->AddYawRotation(-40.f * DT);
	}

	
	static bool tempBool = false;
	if (tempBool == false)
	{
		_mousePos = INPUT->GetMousePos();
		tempBool = true;
	}
	// ※ 위 코드.. Init에서 Start 발동시킬 때에는 INPUT 에서 MpousesPose 못찾음. 추후 Start를 Init 말고 다른데서 처리한다면
	//   그 때 Start에서 처리시켜도 될듯. 우선은 임시로 여기서 이렇게 처리함

	POINT mousePos = INPUT->GetMousePos();
	float diffX = mousePos.x - _mousePos.x;
	float diffY = mousePos.y - _mousePos.y;
	_mousePos = mousePos;
	if (fabs(diffX) > 1e-0)
	{
		GetTransform()->AddYawRotation(-diffX * 0.4f);
	}
	if (fabs(diffY) > 1e-0)
	{
		GetTransform()->AddPitchRotation(-diffY * 0.4f);
	}



	if (INPUT->GetButton(KEY_TYPE::KEY_2))
	{
		
	}
}
