#pragma once
#include "Engine/IExecute.h"

class TweenDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	

private:
	void CreateKachujin();
	void CreateModelByName(wstring modelName, float scale, uint32 path);
	
private:
	shared_ptr<Shader> _shader;
	shared_ptr<GameObject> _obj;
	shared_ptr<GameObject> _camera;

};

