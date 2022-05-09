#pragma once
#include<string>
#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>



class FBXModel
{
private:
//	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

public:

	friend class FbxLoader;
	struct Node
	{

		std::string name;

		XMVECTOR scaling = { 1,1,1,0 };

		XMVECTOR rotation = { 0, 0, 0, 0 };

		XMVECTOR translation = { 0,0,0,1 };

		XMMATRIX transform;

		XMMATRIX globalTransform;

		Node* parent = nullptr;

	};

	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;

		XMFLOAT3 normal;
		
		XMFLOAT2 uv;
	};

	XMFLOAT3 ambient = { 1,1,1 };
	XMFLOAT3 diffuse = { 1,1,1 };
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};


private:

	std::string name;

	std::vector<Node> nodes;

	Node* meshNode = nullptr;

	std::vector<VertexPosNormalUv> vertices;

	std::vector<unsigned short> indices;

};