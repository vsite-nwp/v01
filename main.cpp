#include <windows.h>

enum { id_button1 = 1, id_button2 = 2};
void OnCreate(HWND hw) {
	CreateWindow("button","One", WS_VISIBLE | WS_CHILD,
		10, 10, 80, 25, hw, (HMENU)id_button1, NULL, NULL);
	CreateWindow("button", "Two", WS_VISIBLE | WS_CHILD,
	10, 50, 80, 25, hw, (HMENU)id_button2, NULL, NULL);

}
void OnCommand(HWND hw, int id) {
	switch (id) {
	case id_button1:
			MessageBox(hw,"Button 1 !",NULL, MB_OK);
			break;
	case id_button2:
		MessageBox(hw, "Button 2 !", NULL, MB_OK);
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
	COLORREF cl;
	cl= RGB(0, 255, 255); 
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(cl);
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
