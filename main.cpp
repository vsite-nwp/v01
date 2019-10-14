#include <windows.h>

enum { id_button1 = 1, id_button2 };

void OnCreate(HWND hw) 
{
	// TODO: create two child windows of type button
	CreateWindow("BUTTON", "Button 1", WS_CHILD | WS_VISIBLE, 150, 100, 100, 50, hw, (HMENU)id_button1, 0, 0);
	CreateWindow("BUTTON", "Button 2", WS_CHILD | WS_VISIBLE, 150, 200, 100, 50, hw, (HMENU)id_button2, 0, 0);
}

void OnCommand(HWND hw, int id) 
{
	// TODO: show message box with text depending on which button was pressed
	if (id == id_button1)
		MessageBox(NULL, "First button", "First button message", MB_OK | MB_ICONEXCLAMATION);
	if (id == id_button2)
		MessageBox(NULL, "Second button", "Second button message", MB_OK | MB_ICONEXCLAMATION);
}

void OnDestroy() 
{
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
	wc.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(0,255,255)); // TODO: replace with cyan background

	return RegisterClass(&wc);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	char clsName[] = "NWPClass";

	if(!RegisterMyClass(hInstance, clsName))
		return 0;

	HWND hwnd = CreateWindow(clsName, "NWP 1",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
		NULL, NULL, hInstance, NULL); 

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);

	return msg.wParam;
}
