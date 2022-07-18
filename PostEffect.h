#pragma once
#include "Sprite.h"
#include"SpriteCommon.h"
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include"Input.h"
class PostEffect :
    public Sprite
{
private:
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMMATRIX = DirectX::XMMATRIX;

private:
    static const float clearColor[4];
public:
    PostEffect();

    void Initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

    void Draw();
    
    void PreDrawScene(ID3D12GraphicsCommandList* CmdList);
    void PostDrawScene(ID3D12GraphicsCommandList* CmdList);

    void Update();

    void CreateGraphicsPipelineState();


private:
    SpriteCommon* spCommon;
    Input* input;
    ComPtr<ID3D12Resource> texBuff[2];
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    ComPtr<ID3D12Resource> depthBuff;
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    ComPtr<ID3D12PipelineState> pipelineState;
    ComPtr<ID3D12RootSignature> rootSiginature;

};

