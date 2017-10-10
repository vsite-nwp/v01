#include <windows.h>

enum { id_button1 = 1, id_button2 = 2 };

void OnCreate(HWND hw) {
	HWND hwndButton = CreateWindow("BUTTON", "ONE",      
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10,  10,100,50,hw, (HMENU)id_button1, NULL,NULL);
	//buton 2
	HWND hwndButton2 = CreateWindow("BUTTON", "TWO",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD ,
		10, 100, 100, 50, hw, (HMENU)id_button2, NULL, NULL);

}

void OnCommand(HWND hw, int id) {
	char* buffer;
	switch (id) {
	case id_button1:
		buffer = "one";
		break;
	case id_button2:
		buffer = "two";
		break;
		
	}
	MessageBox(hw, buffer, "Msg title", MB_OK | MB_ICONWARNING);
	
	
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
