#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>

#include"FBXModel.h"
#include"FbxLoader.h"
#include"Camera.h"


class ObjectFBX3d
{
private:
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	template <class T> using vector = std::vector<T>;

public:
	struct ConstBufferDataTransform
	{
		//XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	static const int MAX_BONES = 32;
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
private:
	
	static ComPtr<ID3D12RootSignature> rootsignature;

	static ComPtr<ID3D12PipelineState> pipelineState;


public:

	static void SetDevice(ID3D12Device* device) { ObjectFBX3d::device = device; }
	
	static void SetCamera(Camera* camera) { ObjectFBX3d::camera = camera;}

	static void CreateGraphicsPipeline();

public:
	//メンバ変数
	void Initialize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FBXModel* fbxModel) { this->FbxModel = fbxModel; }

	void PlayAnimation();

private:
	ComPtr<ID3D12Resource> constBuffTransform;
	ComPtr<ID3D12Resource> constBuffSkin;

	static ID3D12Device* device;

	static Camera* camera;

private:

	XMFLOAT3 scale = { 1.0,1.0,1.0 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,1 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;

	FBXModel* FbxModel = nullptr;

private:
	
	FbxTime frameTime;

	FbxTime startTime;
	
	FbxTime endTime;

	FbxTime curentTime;
	
	bool isPlay = false;

};