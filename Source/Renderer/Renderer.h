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
	void SetPipeline();

	void ClearColor(XMFLOAT4 i_color) const;
	void Present() const;
	void Draw(uint32_t i_indexCount);

	//Temporal stuff
	ComPtr<ID3D11Device> GetDevice() const { return m_device; };
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return m_deviceContext; };

private:
	void CreateDevice();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateShaders();
	void CreateInputLayout();

private:
	std::shared_ptr<Window> m_window;
	ComPtr<ID3D11Device> m_device = nullptr;
	ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_swapchain = nullptr;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;

	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3DBlob> m_vertexShaderBlob = nullptr;
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
};

}