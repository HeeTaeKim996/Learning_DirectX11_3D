#pragma once
#include "Engine/Monobehaviour.h"

class CameraScript : public Monobehaviour
{
public:
	virtual void Start() override;
	virtual void Update() override;


private:
	float _speed = 10.f;

	POINT _mousePos;

};

