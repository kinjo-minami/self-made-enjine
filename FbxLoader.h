#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <cassert>
#include <string>
#include "FBXModel.h"

class FbxLoader
{
private: //エイリアス
	using string = std::string;

public: //定数
	//モデル格納ルートパス
	static const string baseDirectory;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

public: //静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

public: //メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	FBXModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// ノード構成を解析
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	/// <param name="parent">親ノード</param>
	void ParseNodeRecursive(FBXModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FBXModel* model, FbxNode* fbxNode);

	/// <summary>
	/// 頂点座標読み取り
	/// </summary>
	void ParseMeshVertices(FBXModel* model, FbxMesh* fbxMesh);

	/// <summary>
	/// 面情報読み取り
	/// </summary>
	void ParseMeshFaces(FBXModel* model, FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	void ParseMaterial(FBXModel* model, FbxNode* fbxNode);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(FBXModel* model, const std::string& fullPath);

	/// <summary>
	/// ディレクトリを含んだファイルパスからファイル名を抽出する
	/// </summary>
	std::string ExtractFileName(const std::string& path);

	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	void ParseSkin(FBXModel* model, FbxMesh* fbxMesh);

private: //メンバ関数
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

private: //メンバ変数
	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;
};