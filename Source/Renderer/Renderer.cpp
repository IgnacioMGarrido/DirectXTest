#include "Renderer/Renderer.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Window/Window.h"

namespace Core
{
	Renderer::Renderer(std::shared_ptr<Window> i_window)
		: m_window(i_window)
	{
		CreateDevice();
		CreateSwapChain();
		CreateRenderTargetView();
		CreateShaders();
		CreateInputLayout();
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

///////////////////// UTILS /////////////////////////////

	std::string GetProjectDirectory()
	{
		std::string fullpath(__FILE__);
		size_t found = fullpath.find_last_of("/\\");

		if (found != std::string::npos)
		{
			std::string projDir = fullpath.substr(0, found);
			
			found = projDir.find_last_of("/\\");
			if (found != std::string::npos)
			{
				return projDir.substr(0, found);
			}
		}
		return "";
	}

	std::string ReadShader(const std::string& i_filePath)
	{
		std::string filePath = GetProjectDirectory() + i_filePath;
		std::string outputSource;

		std::ifstream shaderFile(filePath);
		
		if(!shaderFile.is_open())
		{
			std::cerr << "Failed to read file: " << filePath << "\n";
			return "";
		}

		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		shaderFile.close();

		return buffer.str();
	}

//////////////////////////////////////////////////////////

	void Renderer::CreateShaders()
	{
		ComPtr<ID3DBlob> errorBlob;
		const std::string pixelShaderCode = ReadShader("\\Shaders\\PixelShader.hlsl");
		HRESULT pixelShaderResult =  D3DCompile( pixelShaderCode.c_str(), pixelShaderCode.length(), nullptr, nullptr, nullptr, "main", "ps_5_0", 
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_vertexShaderBlob, &errorBlob);

		if(FAILED(pixelShaderResult))
		{
			printf("Failed to compile Pixel Shader file: \n");
			printf("%s\n", (char*)errorBlob->GetBufferPointer());
			abort();
		}

		HRESULT hr = m_device->CreatePixelShader(m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
		if(FAILED(hr))
		{
			printf("Failed to cretate Pixel Shader\n");
			abort();
		}

		const std::string vertexShaderCode = ReadShader("\\Shaders\\VertexShader.hlsl");
		HRESULT vertexShaderResult = D3DCompile(vertexShaderCode.c_str(), vertexShaderCode.length(), nullptr, nullptr, nullptr, "main", "vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_vertexShaderBlob, &errorBlob);

		if (FAILED(vertexShaderResult))
		{
			printf("Failed to compile Vertex Shader file: \n");
			printf("%s\n", (char*)errorBlob->GetBufferPointer());
			abort();
		}

		hr = m_device->CreateVertexShader(m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
		if (FAILED(hr))
		{
			printf("Failed to cretate Vertex Shader\n");
			abort();
		}
		printf("Successfully created d3d11 PixelShader and VertexShader!\n");

	}

	void Renderer::CreateInputLayout()
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDes[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT result = m_device->CreateInputLayout(inputElementDes, 2, m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), m_inputLayout.GetAddressOf());
		if (FAILED(result)) 
		{
			printf("Failed to create InputLayout.");
			abort();
		}
	}

	void Renderer::SetPipeline()
	{
		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_VIEWPORT viewport = {};

		viewport.Width = static_cast<float>(m_window->GetSize().x);
		viewport.Height = static_cast<float>(m_window->GetSize().y);
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_deviceContext->RSSetViewports(1, &viewport);
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

	void Renderer::Draw(uint32_t i_indexCount)
	{
		m_deviceContext->DrawIndexed(i_indexCount, 0, 0);
	}
}