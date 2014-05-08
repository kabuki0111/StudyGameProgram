#define CRTDBG_MAP_ALLOC		//���������[�N�����o�̂���
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>	//���������[�N�����o�̂���
#include <crtdbg.h>	//���������[�N�����o�̂���
#include "graphics.h"

//�֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

//�O���[�o���ϐ�
HINSTANCE hinst;
HDC hdc;
TCHAR ch = ' ';
RECT rect;
PAINTSTRUCT ps;
bool vkKeys[256];

//Graphics�|�C���^
Graphics *graphics;

//�萔
//const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "DirectX Window";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

// Windows�A�v���P�[�V�����̊J�n�_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	//�f�o�b�O�r���h�̏ꍇ�A���������[�N���`�F�b�N
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	MSG msg;
	HWND hwnd = NULL;

	//�E�B���h�E���쐬
	if (!CreateMainWindow(hInstance, nCmdShow)){
		return 1;
	}
	
	try{
		//Graphics�I�u�W�F�N�g���쐬
		graphics = new Graphics;
		//Graphics���������AGameError���X���[
		graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

		int done = 0;
		while (!done){
			//PeekMessage, Windows���b�Z�[�W���Ȃ������`�F�b�N�����u���b�N���\�b�h
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
				//�I�����b�Z�[�W�����m
				if (msg.message == WM_QUIT){
					done = 1;
				}
				// ���b�Z�[�W���o�R�[�h����WinProc�ɓn��
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}else{
				graphics->showBackbuffer();
			}
			SAFE_DELETE(graphics);
			return msg.wParam;
		}
	}catch (const GameError &err){
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}catch (...){
		MessageBox(NULL, "Unknown error occurred in game.", "Error", MB_OK);
	}
	SAFE_DELETE(graphics);	//�I���̑O�Ƀ����������
	return 0;

}


//�E�B���h�E�C�x���g�R�[���o�b�N�֐�
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	case WM_DESTROY:		//Windows�ɂ��̃v���O�������I������悤�ɓ`����
		PostQuitMessage(0);
		return 0;
	case WM_CHAR:			//�L�[�{�[�h���當�������͂��ꂽ�ꍇ
		switch (wParam){
		case ESC_KEY:		//�v���O�������I������L�[
			PostQuitMessage(0);	//Windows�ɂ��̃v���O�������I������悤�ɓ`����
			break;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow){
	WNDCLASSEX wcx;
	HWND hwnd;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;

	if (RegisterClassEx(&wcx) == 0){
		return false;
	}
	//�E�B���h�E���쐬
	hwnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	if (!hwnd){
		return FALSE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return true;
}