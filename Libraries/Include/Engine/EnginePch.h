#pragma once
/*----------------------------------------------------------------------------------------------------
	- 속성 Include, Lib 이 매우 복잡.
	- 인프런 DX11강의의 섹션6. DirectX11 3D 의 35. 프로젝트 설정의 0분_15분정도까지가 속성 설정등 정리함
----------------------------------------------------------------------------------------------------*/

#if 1

#define WIN32_LEAN_AND_MEAN

#include "Types.h"
#include "Define.h"

// STL
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;

// WIN
#include <windows.h>
#include <assert.h>
#include <optional>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;


#include <FX11/d3dx11effect.h>


// Assim : 리소스 로드를 위한 외부 라이브러리
// ※ 외부 Assimp 라이브러리 다운 받는 간략한 과정(CMake 포함)은, 55.Assimp 강의 참조
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#pragma comment(lib, "Assimp/assimp-vc143-mtd.lib")


// Extern Libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#pragma comment(lib, "FX11/Effects11d.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#pragma comment(lib, "FX11/Effects11.lib")  // Libraries 의 FX11 에 있는 코드 (강의의 2D -> 3D로 넘어오며 추가)
#endif

// Managers
#include "Game.h"
#include "Graphics.h"
//#include "InputManager.h"
//#include "TimeManager.h"
//#include "ResourceManager.h"
//#include "RenderManager.h"


// Engine
#include "VertexData.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "IExecute.h"

#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "Mesh.h"



// MyMath
#include "MyMath/MyMath.h"
//#pragma comment(lib, "MyMath.lib")
#ifdef _DEBUG
#pragma comment(lib, "MyMath/Debug/MyMath.lib")
#else
#pragma comment(lib, "MyMath/Release/MyMath.lib")
#endif

#include "MyMathUtils.h"
#endif


















































#if 0 // First

#define WIN32_LEAN_AND_MEAN

#include "Types.h"
#include "Define.h"

// STL
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;

// WIN
#include <windows.h>
#include <assert.h>
#include <optional>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#include <FX11/d3dx11effect.h>

// Libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#pragma comment(lib, "FX11/Effects11d.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#pragma comment(lib, "FX11/Effects11.lib")
#endif

// Managers
#include "Game.h"
#include "Graphics.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"

#define CHECK(p)	assert(SUCCEEDED(p))
#define GAME		GET_SINGLE(Game)
#define GRAPHICS	GET_SINGLE(Graphics)
#define DEVICE		GRAPHICS->GetDevice()
#define DC			GRAPHICS->GetDeviceContext()
#define INPUT		GET_SINGLE(InputManager)
#define TIME		GET_SINGLE(TimeManager)
#define DT			TIME->GetDeltaTime()
#define RESOURCES	GET_SINGLE(ResourceManager)


// Engine
#include "VertexData.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "IExecute.h"

#endif // First