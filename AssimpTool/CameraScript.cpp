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
		GetTransform()->AddPitchRotation(80.f * DT);

	}
	else if (INPUT->GetButton(KEY_TYPE::E))
	{
		GetTransform()->AddPitchRotation(-80.f * DT);
	}

	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		GetTransform()->AddUnitYRotation(80.f * DT);
	}
	else if (INPUT->GetButton(KEY_TYPE::C))
	{
		GetTransform()->AddUnitYRotation(-80.f * DT);
	}


	{
#if 0
		static bool tempBool = false;
		if (tempBool == false)
		{
			_mousePos = INPUT->GetMousePos();
			tempBool = true;
		}

		POINT mousePos = INPUT->GetMousePos();
		float diffX = mousePos.x - _mousePos.x;
		float diffY = mousePos.y - _mousePos.y;
		_mousePos = mousePos;

		if (fabs(diffX) > 1e-0)
		{
			GetTransform()->AddUnitYRotation(-diffX * 0.4f);
		}
		if (fabs(diffY) > 1e-0)
		{
			GetTransform()->AddPitchRotation(-diffY * 0.4f);
		}
#endif
	}
	
}
