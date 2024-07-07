#include "Renderer/Renderer.h"
#include "Window/Window.h"
#include <cstdio>
namespace Core
{
	Renderer::Renderer(std::shared_ptr<Window> i_window)
		: m_window(i_window)
	{
		CreateDevice();
		CreateSwapChain();
		CreateRenderTargetView();
	}

	void Renderer::CreateDevice()
	{
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

		HRESULT result = D3D11CreateDevice( nullptr,
											D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
											nullptr,
											0,
											featureLevels,
											1, D3D11_SDK_VERSION,
											m_device.GetAddressOf(),
											nullptr,
											m_deviceContext.GetAddressOf());

		if (FAILED(result))
		{
			printf("Failed to create d3d11 device\n");
			abort();
		}

		printf("successfully created d3d11 device context\n");


	}
	
	void Renderer::CreateSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC swapchainDescriptor = {};
		swapchainDescriptor.BufferCount = 1;
		swapchainDescriptor.BufferDesc.Width = m_window->GetSize().x;
		swapchainDescriptor.BufferDesc.Height = m_window->GetSize().y;
		swapchainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDescriptor.BufferDesc.RefreshRate.Numerator = 60;
		swapchainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
		swapchainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDescriptor.OutputWindow = m_window->GetWindowNativeHandler();
		swapchainDescriptor.SampleDesc.Count = 1;
		swapchainDescriptor.SampleDesc.Quality = 0;
		swapchainDescriptor.Windowed = TRUE;

		ComPtr<IDXGIDevice> dxgiDevice;
		m_device.As(&dxgiDevice);

		ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		ComPtr<IDXGIFactory> dxgiFactory;
		dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	
		HRESULT result = dxgiFactory->CreateSwapChain(m_device.Get(), &swapchainDescriptor, m_swapchain.GetAddressOf());
		if (FAILED(result))
		{
			printf("Failed to create d3d11 Swapchain!\n");
			abort();
		}

		printf("successfully created d3d11 device Swapchain!\n");

	}
	
	void Renderer::CreateRenderTargetView()
	{
		ComPtr<ID3D11Texture2D> colorBuffer;
		m_swapchain->GetBuffer(0, IID_PPV_ARGS(&colorBuffer));
		
		HRESULT result = m_device->CreateRenderTargetView(colorBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());

		if (FAILED(result))
		{
			printf("Failed to create d3d11 RenderTargetView!\n");

		}

		printf("successfully created d3d11 RenderTargetView!\n");
	}
	
	void Renderer::ClearColor(XMFLOAT4 i_color) const
	{
		const float color[4] = { i_color.x, i_color.y, i_color.z, i_color.w };
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
	}
	
	void Renderer::Present() const
	{
		m_swapchain->Present(1, 0);
	}
}