#pragma once
#include "Component.h"


enum class ProjectionType
{
	Perspective, // 원근
	Orthographic, // 직교
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void UpdateMatrix();

	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }

	
	void SetViewMatrix();
	void SetProjectionMatrix();
	void SetVInvMatrix();



private:
	// [OBSOLATE]
	//Matrix _matView = Matrix::Identity;
	//Matrix _matProjection = Matrix::Identity;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;

public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
	static Matrix S_MatVInv;
};

