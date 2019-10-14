#include <windows.h>

enum { id_button1 = 1, id_button2 };

void OnCreate(HWND hw) {
	// TODO: create two child windows of type button

	HWND button1 = CreateWindow("BUTTON", "one", WS_CHILD | WS_VISIBLE, 100, 100, 80, 24, hw, 0, 0, 0);

	HWND button2 = CreateWindow("BUTTON", "two", WS_CHILD | WS_VISIBLE, 100, 150, 80, 24, hw, 0, 0, 0);
}

void OnCommand(HWND hw, int id) {
	// TODO: show message box with text depending on which button was pressed
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
	wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 255)); // TODO: replace with cyan background

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
