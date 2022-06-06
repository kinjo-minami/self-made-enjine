#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include<string>

#include"FBXModel.h"
class FbxLoader
{
private:
	using string = std::string;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	template <class T> using vector = std::vector<T>;
	template <class T> using list = std::list<T>;


public:
	static const string baseDirectory;

	static const string defaultTextureFileName;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	void Initalize(ID3D12Device* device);

	void Finalize();

	//void LoadModelFromFile(const string& modelName);
	FBXModel* LoadModelFromFile(const string& modelName);

	void ParseNodeRecursive(FBXModel* fbxModel, FbxNode* fbxNode, FBXModel::Node* parent = nullptr);

	//メッシュ
	void ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode);

	void ParseMeshVertices(FBXModel* fbxModel, FbxMesh* fbxMesh);

	void ParseMeshFaces(FBXModel* fbxModel, FbxMesh* fbxMesh);

	void ParseMaterial(FBXModel* fbxModel, FbxNode* fbxNode);

	void LoadTexture(FBXModel* fbxModel, const std::string& fullpath);

	static void ConvertMatrixFromFbx(XMMATRIX* dst, const FbxMatrix& src);

	void ParseSkin(FBXModel* fbxModel, FbxMesh* fbxMesh);


	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);


private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

private:

	ID3D12Device* device = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImporter = nullptr;

};