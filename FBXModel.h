#pragma once
#include<string>
#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>
#include<Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<fbxsdk.h>


class FBXModel
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;
	template <class T> using vector = std::vector<T>;
public:
	static const int MAX_BONE_INDICES = 4;

	friend class FbxLoader;
	struct Node
	{

		string name;

		XMVECTOR scaling = { 1,1,1,0 };

		XMVECTOR rotation = { 0, 0, 0, 0 };

		XMVECTOR translation = { 0,0,0,1 };

		XMMATRIX transform;

		XMMATRIX globalTransform;

		Node* parent = nullptr;

	};

	struct VertexPosNormalUvSkin
	{
		XMFLOAT3 pos;

		XMFLOAT3 normal;
		
		XMFLOAT2 uv;

		UINT boneIndex[MAX_BONE_INDICES];

		float boneWeight[MAX_BONE_INDICES];
	};

	struct Bone
	{
		std::string name;

		XMMATRIX invInitialPose;

		FbxCluster* fbxCluster;

		Bone(const std::string& name_) { this->name = name_; }
	};
	

	XMFLOAT3 ambient = { 1,1,1 };
	XMFLOAT3 diffuse = { 1,1,1 };
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};


private:

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> texBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:
	
	void CreateBuffers(ID3D12Device* device);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	FbxScene* GetFbxScene() { return fbxscene; }

	~FBXModel();
	vector<Bone>& GetBones() { return bones; }
private:

	string name;

	vector<Node> nodes;

	vector<Bone> bones;


	Node* meshNode = nullptr;

	vector<VertexPosNormalUvSkin> vertices;

	vector<unsigned short> indices;


	FbxScene* fbxscene = nullptr;




};