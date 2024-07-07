#include "Window/Window.h"
#include "Renderer/Renderer.h"

struct VertexData 
{
    XMFLOAT3 position;
    XMFLOAT4 color;
};

const VertexData vertices[] = 
{
    { XMFLOAT3(-0.5f, -0.5f, 0.0f), {XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}}},
    { XMFLOAT3( 0.0f, 0.5f,  0.0f), {XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}}},
    { XMFLOAT3( 0.5f, -0.5f, 0.0f), {XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}}}
};

const uint32_t indices[]
{
    0, 1, 2
};

int main()
{

	std::shared_ptr<Core::Window> window = std::make_shared<Core::Window>(Core::WindowSize(1280, 720), "D3D11 Test");
    window->Init();

	std::unique_ptr<Core::Renderer> renderer = std::make_unique<Core::Renderer>(window);


    // TODO: Move this to the renderer
    ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexData) * 3;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
    vertexSubresourceData.pSysMem = vertices;
    vertexSubresourceData.SysMemPitch = 0;
    vertexSubresourceData.SysMemSlicePitch = 0;

    renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());
	
    ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(uint32_t) * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	indexSubresourceData.pSysMem = indices;
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

	renderer->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, indexBuffer.GetAddressOf());

    uint32_t stride = sizeof(VertexData);
    uint32_t  offset = 0;

    renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    renderer->GetDeviceContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);



    while (window->IsVisible())
    {
        window->Run();

        renderer->ClearColor({1.f, 0.0f, 1.0f, 1.0f});
        renderer->SetPipeline();
        renderer->Draw(3);
        renderer->Present();
    }

    return 0;
}