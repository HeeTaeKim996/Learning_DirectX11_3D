#pragma once
#include "Engine/IExecute.h"
#include "Engine/Geometry.h"

class GameObject;

class MeshDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	shared_ptr<Shader> _shader;


	shared_ptr<GameObject> _obj;
	shared_ptr<GameObject> _camera;
};

