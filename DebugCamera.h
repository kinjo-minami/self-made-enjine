#pragma once
#include "Camera.h"
#include "Input.h"


class DebugCamera : public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	DebugCamera(int window_width, int window_height);

	// 更新
	void Update() override;

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

private:
	// 入力クラスのポインタ
	Input* input;
	// カメラ注視点までの距離
	float distance = 20;
	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
};

