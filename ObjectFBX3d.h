#pragma once
#include "FBXModel.h"
#include "Camera.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class ObjectFBX3d
{
protected: 
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: 
	static const int MAX_BONES = 32;

public: 
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: 
	static void SetDevice(ID3D12Device* device) { ObjectFBX3d::device = device; }
	static void SetCamera(Camera* camera) { ObjectFBX3d::camera = camera; }
	static void CreateGraphicsPipeline();

public: 
	void Initialize();

	void Update();

	void SetModel(FBXModel* model) { this->model = model; }

	void Draw(ID3D12GraphicsCommandList* cmdList);

	const XMFLOAT3& GetPosition() { return position; }

	void SetPosition(XMFLOAT3 position) { this->position = position; }

	const XMFLOAT3& GetScale() { return scale; }

	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	const XMFLOAT3& GetRotation() { return rotation; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	void PlayAnimation(bool isLoop = true);

	void StopAnimation();

	const bool& GetIsAnimation() { return isPlay; }

protected: 
	
	ComPtr<ID3D12Resource> constBufferTransform;
	
	XMFLOAT3 scale = { 1, 1, 1 };
	
	XMFLOAT3 rotation = { 0, 0, 0 };
	
	XMFLOAT3 position = { 0, 0, 0 };
	
	XMMATRIX matWorld;
	
	FBXModel* model = nullptr;
	
	ComPtr<ID3D12Resource> constBufferSkin;
	
	FbxTime frameTime;
	
	FbxTime startTime;
	
	FbxTime endTime;
	
	FbxTime currentTime;
	
	bool isPlay = false;
	
	bool isLoop = false;

private: 
	
	static ID3D12Device* device;
	
	static Camera* camera;
	
	static ComPtr<ID3D12RootSignature> rootsignature;
	
	static ComPtr<ID3D12PipelineState> pipelinestate;
};

