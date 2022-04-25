#pragma once
#include <windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>

#include "WinApp.h"

class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();
	//�L�[�̉������`�F�b�N
	bool PushKey(BYTE keyNumber);
	//�L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE keyNumber);
private:
	//windowsAPI
	WinApp* winApp = nullptr;

	ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	ComPtr<IDirectInput8> dinput = nullptr;


	BYTE key[256] = {};
	BYTE keyPre[256] = {};

};

