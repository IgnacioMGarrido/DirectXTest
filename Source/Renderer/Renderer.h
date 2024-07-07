#pragma once
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;


namespace Core
{

class Window;

class Renderer
{
public:
	explicit Renderer(std::shared_ptr<Window> i_window);
	~Renderer() = default;

	void CreateDevice();
	void CreateSwapChain();
	void CreateRenderTargetView();

	void ClearColor(XMFLOAT4 i_color) const;
	void Present() const;
private:
	std::shared_ptr<Window> m_window;
	ComPtr<ID3D11Device> m_device = nullptr;
	ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_swapchain = nullptr;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
};

}