#define CRTDBG_MAP_ALLOC		//メモリリークを検出のため
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>	//メモリリークを検出のため
#include <crtdbg.h>	//メモリリークを検出のため
#include "graphics.h"

//関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

//グローバル変数
HINSTANCE hinst;
HDC hdc;
TCHAR ch = ' ';
RECT rect;
PAINTSTRUCT ps;
bool vkKeys[256];

//Graphicsポインタ
Graphics *graphics;

//定数
//const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "DirectX Window";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

// Windowsアプリケーションの開始点
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	//デバッグビルドの場合、メモリリークをチェック
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	MSG msg;
	HWND hwnd = NULL;

	//ウィンドウを作成
	if (!CreateMainWindow(hInstance, nCmdShow)){
		return 1;
	}
	
	try{
		//Graphicsオブジェクトを作成
		graphics = new Graphics;
		//Graphicsを初期化、GameErrorをスロー
		graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

		int done = 0;
		while (!done){
			//PeekMessage, Windowsメッセージがないかをチェックする非ブロックメソッド
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
				//終了メッセージを検知
				if (msg.message == WM_QUIT){
					done = 1;
				}
				// メッセージを出コードしてWinProcに渡す
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
	SAFE_DELETE(graphics);	//終了の前にメモリを解放
	return 0;

}


//ウィンドウイベントコールバック関数
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	case WM_DESTROY:		//Windowsにこのプログラムを終了するように伝える
		PostQuitMessage(0);
		return 0;
	case WM_CHAR:			//キーボードから文字が入力された場合
		switch (wParam){
		case ESC_KEY:		//プログラムを終了するキー
			PostQuitMessage(0);	//Windowsにこのプログラムを終了するように伝える
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
	//ウィンドウを作成
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