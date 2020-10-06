#include <windows.h>
#include <stdio.h>
#include "core/core.h"
#include "controller/controller.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PWSTR pCmdLine, int nCmdShow) {

	MSG  msg;
	WNDCLASSW wc = { 0 };
	wc.lpszClassName = L"Center";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, L"Mine Sweeper",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 300, 220, 0, 0, hInstance, 0);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	static HWND btns[64], status, toggle, reset;
	static char mode = 0;
	HWND *iter;
	int idx;
	char buf[50];
	switch (msg) {
	case WM_CREATE:
		status = CreateWindowW(L"static", L"Mines: 10", WS_CHILD | WS_VISIBLE, 0, 0, 160, 20, hwnd, 0, 0, 0);
		toggle = CreateWindowW(L"button", L"L", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 20, 50, 20, hwnd, 100, 0, 0);
		reset = CreateWindowW(L"button", L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 235, 20, 50, 20, hwnd, 200, 0, 0);
		iter = btns;
		idx = 0;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j, ++iter, ++idx) {
				*iter = CreateWindowW(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 60 + j * 20, 20 + i * 20, 20, 20, hwnd, idx, 0, 0);
			}
		}
		core_initialize();
		break;
	case WM_COMMAND:
		switch (HIWORD(wParam)) {
		case BN_CLICKED:
			idx = LOWORD(wParam);
			if (idx < 100) {
				if (mode) {
					core_mark(idx >> 3, idx & 7);
				} else {
					core_check(idx >> 3, idx & 7);
				}
				snprintf(buf, 49, "Mines: %d", mines);
				SetWindowText(status, buf);
				if (state) {
					controller_render(1, btns);
					MessageBox(hwnd, state == WIN ? "Win" : "Loss", "Game Over", 0);
					core_initialize();
					controller_render(0, btns);
					SetWindowText(status, "Mines: 10");
				} else {
					controller_render(0, btns);
				}
			} else {
				switch (idx) {
				case 100:
					mode = !mode;
					SetWindowText(toggle, mode ? "R" : "L");
					break;
				case 200:
					SetWindowText(status, "Mines: 10");
					core_initialize();
					controller_render(0, btns);
					break;
				}
					
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}