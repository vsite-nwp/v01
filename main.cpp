#include <windows.h>

enum { id_button1 = 1, id_button2 };

void OnCreate(HWND hw) {
	
	HWND button1 = CreateWindow("BUTTON",
		"ONE",
		WS_CHILD | WS_VISIBLE,
		75, 50, 100, 50,
		hw,
		(HMENU)id_button1,
		0,
		NULL);
	HWND button2 = CreateWindow("BUTTON",
		"TWO",
		WS_CHILD | WS_VISIBLE,
		75, 150, 100, 50,
		hw,
		(HMENU)id_button2,
		0,
		NULL);
}

void OnCommand(HWND hw, int id) {
	
	int message;
	if (id == id_button1)
		message = MessageBox(hw, "ONE", "Button", MB_ICONWARNING | MB_OK);
	else
		message = MessageBox(hw, "TWO", "Button", MB_ICONWARNING | MB_OK);

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
	wc.hbrBackground = CreateSolidBrush(RGB(0,255,255)); // TODO: replace with cyan background

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
