#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include"SpriteCommon.h"

class Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};

	struct ConstBufferData {
		XMFLOAT4 color; // �F (RGBA)
		XMMATRIX mat;   // �R�c�ϊ��s��
	};

protected:
	//���_�o�b�t�@;
	ComPtr<ID3D12Resource> vertBuff_;
	//���_�o�b�t�@�r���[;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//�萔�o�b�t�@;
	ComPtr<ID3D12Resource> constBuff_;
	// Z�����̉�]�p
	float rotation_ = 0.0f;
	// ���W
	XMFLOAT3 position_ = { 0,0,0 };
	// ���[���h�s��
	XMMATRIX matWorld_;
	// �F(RGBA)
	XMFLOAT4 color_ = { 1, 1, 1, 1 };
	// �e�N�X�`���ԍ�
	UINT texNumber_ = 0;
	// �傫��
	XMFLOAT2 size_ = { 100, 100 };
	// �A���J�[�|�C���g
	XMFLOAT2 anchorpoint_ = { 0.5f, 0.5f };
	// ���E���]
	bool isFlipX_ = false;
	// �㉺���]
	bool isFlipY_ = false;
	// �e�N�X�`��������W
	XMFLOAT2 texLeftTop_ = { 0, 0 };
	// �e�N�X�`���؂�o���T�C�Y
	XMFLOAT2 texSize_ = { 100, 100 };
	// ��\��
	bool isInvisible_ = false;

	SpriteCommon* SspriteCommon = nullptr;

public:
	Sprite(/*UINT texNumber, XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY*/);

	static Sprite* Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void TransferVertexBuffer();

	void Update();

	void SetPosition(const XMFLOAT3 position) { position_ = position; }

	void SetRotation(float rotation) { rotation_ = rotation; }

	void SetSize(XMFLOAT2 size) { size_ = size; }

	void Draw();

};