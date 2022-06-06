
#include<vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "Object3d.h"
#include "Model.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"FbxLoader.h"
#include"ObjectFBX3d.h"
#include"DebugCamera.h"
//#include"fbxsdk.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region WindowsAPI初期化
	//FbxManager* fbxManager = FbxManager::Create();

	WinApp* winApp = nullptr;

	winApp = new WinApp();
	winApp->Initialize();

	MSG msg{};  // メッセージ
#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理

	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(winApp);
	DebugCamera* camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
	

	//Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);
	Object3d::StaticInitialize(dxCommon->GetDev());


	ObjectFBX3d::SetDevice(dxCommon->GetDev());

	ObjectFBX3d::SetCamera(camera);

	ObjectFBX3d::CreateGraphicsPipeline();

	FbxLoader::GetInstance()->Initalize(dxCommon->GetDev());

	//FbxLoader::GetInstance()->LoadModelFromFile("cube");

	FBXModel* fbxModel = nullptr;
	ObjectFBX3d* objectFBX3d = nullptr;
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	objectFBX3d = new ObjectFBX3d;
	objectFBX3d->Initialize();
	objectFBX3d->SetModel(fbxModel);
	
#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理


	SpriteCommon* spriteCommon = new SpriteCommon();
	spriteCommon->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	
	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	std::vector<Sprite*> sprites;
	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/

	for (int i = 0; i < 20; i++)
	{
		int texNum = rand() % 2;

		Sprite* sprite = Sprite::Create(spriteCommon, texNum, { 0,0 }, false, false);

		sprite->SetPosition({ (float)(rand() % 1280),(float)(rand() % 720),0 });

		//sprite->SetRotation((float)(rand() % 360));

		sprite->SetSize({ (float)(rand() % 400), (float)(rand() % 100) });

		sprite->TransferVertexBuffer();

		sprites.push_back(sprite);
		//sprite->SetPosition({ 500,300,0 });

	}


	Model* modelPost = Model::LoadFromOBJ("posuto");
	Model* modelChr = Model::LoadFromOBJ("chr_sword");

	Object3d* objPost = Object3d::Create();
	Object3d* objChr = Object3d::Create();


	objPost->SetModel(modelPost);
	objChr->SetModel(modelPost);

	objPost->SetPosition({ -10,0,-5 });
	objChr->SetPosition({ +10,0,+5 });

	//objPost->Update();
	//objChr->Update();

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	while (true)  // ゲームループ
	{
#pragma region ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理　ここから
		objectFBX3d->PlayAnimation();
		input->Update();

		const int cycle = 540; // 繰り返しの周期
		counter++;
		counter %= cycle; // 周期を超えたら0に戻る
		float scale = (float)counter / cycle; // [0,1]の数値

		scale *= 360.0f;
		objPost->SetModel(modelPost);
		objChr->SetModel(modelPost);
		if (input->TriggerKey(DIK_0)) // 数字の0キーが押されていたら
		{
			OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
		}

		float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

		if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
		{
			// 画面クリアカラーの数値を書き換える
			clearColor[1] = 1.0f;
			objPost->SetModel(modelChr);
			objChr->SetModel(modelChr);
		}

		// 座標操作
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{

		}


		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{

		}
		objectFBX3d->Update();
	/*	objPost->Update();
		objChr->Update();*/
		for (auto& sprite : sprites)
		{
			sprite->Update();
		}
		
		// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド

		dxCommon->PreDraw();

		//Object3d::PreDraw(dxCommon->GetCmdList());
		///*objPost->Draw();
		//objChr->Draw();*/
		//Object3d::PostDraw();
		objectFBX3d->Draw(dxCommon->GetCmdList());
		spriteCommon->PreDraw();
		//for (auto& sprite : sprites)
		//{
		//	sprite->Draw();
		//}
		
		// ４．描画コマンドここまで
		dxCommon->PostDraw();


	}
	// XAudio2解放
   // xAudio2.Reset();
	// 音声データ解放
   // SoundUnload(&soundData1);

	
#pragma region WindowsAPI後始末
	winApp->Finalize();
#pragma endregion WindowsAPI後始末
	FbxLoader::GetInstance()->Finalize();
//	delete fbxModel;
	delete objectFBX3d;

	delete input;
	delete winApp;
	delete spriteCommon;
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	delete modelPost;
	delete modelChr;
	delete objChr;
	delete objPost;
	return 0;
}