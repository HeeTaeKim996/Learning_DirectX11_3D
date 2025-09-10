#pragma once

#if 1

/*
#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}
#define GET_SINGLE(classname)	classname::GetInstance()
*/


#define CHECK(p)	assert(SUCCEEDED(p))
#define GAME		Game::GetInstance()		
#define GRAPHICS	Graphics::GetInstance()
#define DEVICE		GRAPHICS->GetDevice()
#define DC			GRAPHICS->GetDeviceContext()
#define INPUT		InputManager::GetInstance()
#define TIME		TimeManager::GetInstance()
#define DT			TIME->GetDeltaTime()
#define RESOURCES	ResourceManager::GetInstance()
#define RENDER		RenderManager::GetInstance()
#define USE_I_M_G_U_I_I_N_D_X_1_1			ImGuiManager::GetInstance()

#endif 











































































#if 0 // First
#define DECLARE_SINGLE(classname)			\
private:									\
	classname() {}							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()
#endif // First