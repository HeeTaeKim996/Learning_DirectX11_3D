#pragma once
#include "Monobehaviour.h"

class CameraScript : public Monobehaviour
{
public:
	virtual void Start() override;
	virtual void Update() override;

	float _speed = 10.f;



};

