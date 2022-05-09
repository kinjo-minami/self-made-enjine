#include "FbxLoader.h"
#include <DirectXTex.h>


using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resources/";

const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}


void FbxLoader::Initalize(ID3D12Device* device)
{
	assert(fbxManager == nullptr);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);

	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");

}

void FbxLoader::Finalize()
{
	fbxImporter->Destroy();

	fbxManager->Destroy();

}

void FbxLoader::LoadModelFromFile(const string& modelName)
{

	const string directoryPath = baseDirectory + modelName + "/";

	const string fileName = modelName + ".fbx";

	const string fullpath = directoryPath + fileName;

	if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
	{
		assert(0);
	}
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

	fbxImporter->Import(fbxScene);

	FBXModel* fbxModel = new FBXModel();
	fbxModel->name = modelName;

	int nodeCount = fbxScene->GetNodeCount();

	fbxModel->nodes.reserve(nodeCount);

	ParseNodeRecursive(fbxModel, fbxScene->GetRootNode());

	fbxScene->Destroy();

}

void FbxLoader::ParseNodeRecursive(FBXModel* fbxModel, FbxNode* fbxNode, FBXModel::Node* parent)
{

	string name = fbxNode->GetName();

	fbxModel->nodes.emplace_back();

	FBXModel::Node& node = fbxModel->nodes.back();

	node.name = fbxNode->GetName();

	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	node.rotation = { (float)rotation[0],(float)rotation[1], (float)rotation[2], 0.0f };
	node.scaling = { (float)scaling[0],(float)scaling[1], (float)scaling[2], 0.0f };
	node.translation = { (float)translation[0],(float)translation[1], (float)translation[2], 1.0f };

	/*node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);*/

	for (int i = 0; i < 3; i++)
	{
		node.rotation.m128_f32[i] = XMConvertToRadians(node.rotation.m128_f32[i]);
	}

	XMMATRIX matScaling, matRotation, matTranslotion;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslotion = XMMatrixTranslationFromVector(node.translation);

	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslotion;

	node.globalTransform = node.transform;
	if (parent)
	{
		node.parent = parent;

		node.globalTransform *= parent->globalTransform;
	}

	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			fbxModel->meshNode = &node;
			ParseMesh(fbxModel, fbxNode);
		}
	}

	//子ノードに対して再起呼び出し
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
	}

}

void FbxLoader::ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode)
{
	//ノードのメッシュ取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	//頂点座標読み取り
	ParseMeshVertices(fbxModel, fbxMesh);

	//面を構成するデータ読み取り
	ParseMeshFaces(fbxModel, fbxMesh);

	//マテリアル読み取り
	ParseMaterial(fbxModel, fbxNode);



}

void FbxLoader::ParseMeshVertices(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;

	const int controlPointCount = fbxMesh->GetControlPointsCount();

	FBXModel::VertexPosNormalUv vert{};

	fbxModel->vertices.resize(controlPointCount, vert);

	//FBXメッシュの頂点座標取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	for (int i = 0; i < controlPointCount; i++)
	{
		FBXModel::VertexPosNormalUv& vertex = vertices[i];

		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}

}

void FbxLoader::ParseMeshFaces(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;
	auto& indices = fbxModel->indices;

	assert(indices.size() == 0);

	const int polygonCount = fbxMesh->GetPolygonCount();

	const int textureUVCount = fbxMesh->GetTextureUVCount();

	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	for (int i = 0; i < polygonCount; i++)
	{
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		for (int j = 0; j < polygonSize; j++)
		{
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			FBXModel::VertexPosNormalUv& vertex = vertices[index];
			FbxVector4 normal;

			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{

				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}

			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmppedUv;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmppedUv))
				{
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}

			}

			if (j < 3)
			{
				indices.push_back(index);
			}
			else
			{
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);

			}

		}

	}



}

void FbxLoader::ParseMaterial(FBXModel* fbxModel, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();

	if (materialCount > 0)
	{
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		bool textureLoaded = false;

		if (material)
		{
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

				FbxPropertyT<FbxDouble3>ambient = lambert->Ambient;
				fbxModel->ambient.x = (float)ambient.Get()[0];
				fbxModel->ambient.y = (float)ambient.Get()[1];
				fbxModel->ambient.z = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				fbxModel->diffuse.x = (float)diffuse.Get()[0];
				fbxModel->diffuse.y = (float)diffuse.Get()[1];
				fbxModel->diffuse.z = (float)diffuse.Get()[2];

			}
			const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
				if (texture)
				{
					const char* filepath = texture->GetFileName();

					string path_str(filepath);
					string name = ExtractFileName(path_str);

					LoadTexture(fbxModel, baseDirectory + fbxModel->name + "/" + name);
					textureLoaded = true;
				}
			}


		}

		if (!textureLoaded)
		{
			LoadTexture(fbxModel, baseDirectory + defaultTextureFileName);
		}

	}


}

void FbxLoader::LoadTexture(FBXModel* fbxModel, const std::string& fullpath)
{
	HRESULT result = S_FALSE;

	TexMetadata& metadata = fbxModel->metadata;
	ScratchImage& scratchImg = fbxModel->scratchImg;

	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);

	if (FAILED(result))
	{
		assert(0);
	}

}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;

	pos1 = path.rfind('\\');

	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

