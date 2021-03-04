#include <windows.h>

enum { id_button1 = 1, id_button2 };

void OnCreate(HWND hw) {

	CreateWindow("BUTTON", "one", WS_CHILD | WS_VISIBLE,
		10, 10, 80, 20,
		hw, (HMENU)id_button1, 0, NULL);

	CreateWindow("BUTTON", "two", WS_CHILD | WS_VISIBLE,
		10, 40, 80, 20,
		hw, (HMENU)id_button2, 0, NULL);
}

void OnCommand(HWND hw, int id) {
	switch (id) {

	case id_button1:
		MessageBox(hw, "one", NULL, MB_ICONWARNING | MB_OK);
		break;

	case id_button2:
		MessageBox(hw, "one", NULL, MB_ICONWARNING | MB_OK);
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
	case WM_CLOSE:

		if (IDNO != MessageBox(hw, "Really exit ?", "NWP 1", MB_YESNO | MB_ICONQUESTION))
			DestroyWindow(hw);

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
	COLORREF color = 0xFF00FFFF;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 255));

	return RegisterClass(&wc);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	char clsName[] = "NWPClass";

	if(!RegisterMyClass(hInstance, clsName))
		return 0;

	HWND hwnd = CreateWindow(clsName, "NWP 1",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		100, 100, 300, 200,
		NULL, NULL, hInstance, NULL); 

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);

	return msg.wParam;
}
