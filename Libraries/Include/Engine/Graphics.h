#pragma once

#if 1
class Graphics
{
	//DECLARE_SINGLE(Graphics);
private:
	Graphics() {}

public:
	static Graphics* GetInstance()
	{
		static Graphics instance;
		return &instance;
	}

public:
	void Init(HWND hwnd);

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStancilView();
	void SetViewport();

private:
	HWND _hwnd = {};

	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// DSV
	ComPtr<ID3D11Texture2D> _depthStencilTexture;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	/*
		- ���̰� ������ RasterizerState ���� �ڵ� ó��. (S, T, 1-S-T), ������������ ����ؼ� ó���Ǵµ�

		�� Stencil
		 - Depth �� ���̰����� �ش� �ȼ��� �׸��� ������ �����Ѵٸ�, Stencil �� �ش� �ȼ��� Ư�� ������ �ο��ؼ�
		   Ư�� ó�� �뵵�� ���
		 - ex) UI �������� ���� �׿� ������ ��� ����. �ſ� ����� �׸��� ���� ������ ������, �ش� ������ ����Ͽ�
		       �ſ� ������ ��..
	*/

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
};
#endif

















































#if 0 // First
class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void Init(HWND hWnd);

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();


private:
	HWND _hwnd = {};

	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
};
#endif // First