#pragma once

class ImGuiManager
{
	//DECLARE_SINGLE(ImGuiManager);
private:
	ImGuiManager() {}
public:
	static ImGuiManager* GetInstance()
	{
		static ImGuiManager instance;
		return &instance;
	}


public:
	void Init();
	void Update();
	void Render();

};

class ImGUIManager
{
};

