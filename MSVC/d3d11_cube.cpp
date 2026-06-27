#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

using namespace DirectX;

struct Vertex {
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct ConstantBuffer {
    XMMATRIX mWorldViewProj;
};

IDXGISwapChain*          g_SwapChain = nullptr;
ID3D11Device*            g_pd3dDevice = nullptr;
ID3D11DeviceContext*     g_pImmediateContext = nullptr;
ID3D11RenderTargetView*  g_pRenderTargetView = nullptr;
ID3D11VertexShader*      g_pVertexShader = nullptr;
ID3D11PixelShader*       g_pPixelShader = nullptr;
ID3D11InputLayout*       g_pVertexLayout = nullptr;
ID3D11Buffer*            g_pVertexBuffer = nullptr;
ID3D11Buffer*            g_pIndexBuffer = nullptr;
ID3D11Buffer*            g_pConstantBuffer = nullptr;

const char* g_ShaderCode = 
"cbuffer ConstantBuffer : register(b0) { matrix WorldViewProj; };\n"
"struct VS_OUTPUT { float4 Pos : SV_POSITION; float4 Color : COLOR; };\n"
"VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR) {\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Pos = mul(Pos, WorldViewProj);\n"
"    output.Color = Color;\n"
"    return output;\n"
"}\n"
"float4 PS(VS_OUTPUT input) : SV_Target { return input.Color; }\n";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"D3D11CubeWindowClass";
    RegisterClassW(&wc);
    HWND hWnd = CreateWindowW(L"D3D11CubeWindowClass", L"Cubo 3D - Direct3D 11 Nativo MSVC", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);

    DXGI_SWAP_CHAIN_DESC sd = {0};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &g_SwapChain, &g_pd3dDevice, nullptr, &g_pImmediateContext);

    ID3D11Texture2D* pBackBuffer = nullptr;
    g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

    D3D11_VIEWPORT vp = {0.0f, 0.0f, 800.0f, 600.0f, 0.0f, 1.0f};
    g_pImmediateContext->RSSetViewports(1, &vp);

    ID3DBlob* pVSBlob = nullptr;
    D3DCompile(g_ShaderCode, strlen(g_ShaderCode), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
    g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    g_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
    pVSBlob->Release();

    ID3DBlob* pPSBlob = nullptr;
    D3DCompile(g_ShaderCode, strlen(g_ShaderCode), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
    g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    pPSBlob->Release();

    Vertex vertices[] = {
        { XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(1, 0, 0, 1) }, { XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT4(0, 1, 0, 1) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(0, 0, 1, 1) }, { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1, 1, 0, 1) },
        { XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(1, 0, 1, 1) }, { XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT4(0, 1, 1, 1) },
        { XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT4(1, 1, 1, 1) }, { XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(0, 0, 0, 1) }
    };
    D3D11_BUFFER_DESC bd = { sizeof(vertices), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
    D3D11_SUBRESOURCE_DATA InitData = { vertices, 0, 0 };
    g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

    WORD indices[] = { 0,1,2, 0,2,3, 4,6,5, 4,7,6, 4,5,1, 4,1,0, 3,2,6, 3,6,7, 1,5,6, 1,6,2, 4,0,3, 4,3,7 };
    bd.ByteWidth = sizeof(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    InitData.pSysMem = indices;
    g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);

    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);

    MSG msg = {0};
    float t = 0.0f;
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            t += 0.005f;
            XMMATRIX mWorld = XMMatrixRotationY(t) * XMMatrixRotationX(t * 0.5f);
            XMMATRIX mView = XMMatrixLookAtLH(XMVectorSet(0.0f, 2.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
            XMMATRIX mProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);
            
            ConstantBuffer cb;
            cb.mWorldViewProj = XMMatrixTranspose(mWorld * mView * mProj);
            g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

            float ClearColor[4] = { 0.1f, 0.2f, 0.4f, 1.0f };
            g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

            UINT stride = sizeof(Vertex);
            UINT offset = 0;
            g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
            g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
            g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
            g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
            g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
            g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
            
            g_pImmediateContext->DrawIndexed(36, 0, 0);
            g_SwapChain->Present(1, 0);
        }
    }
    return (int)msg.wParam;
}