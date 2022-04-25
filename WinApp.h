#pragma once
#include <Windows.h>

class WinApp
{
public://�ÓI�����o�֐�
	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public://�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();
	//�I��
	void Finalize();
	//���b�Z�[�W����
	bool ProcessMessage();
	
	//getter
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() { return w.hInstance; }


public://�ÓI�����o�ϐ�

	// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��

private://�����o�ϐ�
	HWND hwnd = nullptr;
	WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
};

