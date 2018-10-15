#include <windows.h>

enum { id_button1 = 1, id_button2 };


void OnCreate(HWND hw) {
	HWND hwnd_1 = CreateWindow("BUTTON",
		"First", WS_CHILD | WS_VISIBLE,
		400, 100, 100, 50,
		hw, (HMENU)1, NULL, NULL);
	HWND hwnd_2 = CreateWindow("BUTTON",
		"Second", WS_CHILD | WS_VISIBLE,
		400, 300, 100, 50,
		hw, (HMENU)2, NULL, NULL);

	ShowWindow(hwnd_1,SW_SHOW);
	UpdateWindow(hwnd_1);
	ShowWindow(hwnd_2,SW_SHOW);
	UpdateWindow(hwnd_2);
}

void OnCommand(HWND hw, int id) {
	switch (id) {
	case id_button1:
		id = MessageBox(hw, "neka poruka za prvi", "neki naslov za prvi", MB_OK);
		break;
	case id_button2:
		id = MessageBox(hw, "neka poruka za drugi", "neki naslov za drugi", MB_OK);
		break;
	}
}

void OnDestroy() {
	PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CREATE:
			OnCreate(hw);
			return 0;
		case WM_COMMAND:
			OnCommand(hw, LOWORD(wp));
			return 0;
		case WM_DESTROY:
			OnDestroy();
			return 0;
	}
	return DefWindowProc(hw, msg, wp, lp);
}


int RegisterMyClass(HINSTANCE hInstance, char* className)
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof wc);

	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 255));

	return RegisterClass(&wc);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	char clsName[] = "NWPClass";

	if(!RegisterMyClass(hInstance, clsName))
		return 0;

	HWND hwnd = CreateWindow(clsName, "NWP 1",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL); 

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);

	return msg.wParam;
}
