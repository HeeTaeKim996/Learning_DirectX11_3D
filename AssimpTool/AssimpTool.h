#pragma once
#include "Engine/IExecute.h"

class AssimpTool : public IExecute
{

public:
	void Init() override;
	void Update() override;
	void Render() override;


private:
	void CreateModelData(wstring folerPath, wstring meshName);

	void CreateAnimationData(wstring folerPath, wstring animationName);
};

