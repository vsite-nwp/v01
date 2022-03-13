#include <windows.h>

enum { id_button1 = 1, id_button2 };

void on_create(HWND hw) 
{
	
	CreateWindow("BUTTON", "one", WS_CHILD | WS_VISIBLE, 100, 100, 100, 30, hw, (HMENU)id_button1, 0, 0);
	CreateWindow("BUTTON", "two", WS_CHILD | WS_VISIBLE, 100, 150, 100, 30, hw, (HMENU)id_button2, 0, 0);
}

void on_command(HWND hw, int id)
{
	
	switch (id) {
	case 1:
		MessageBox(hw, "one", "NWP", MB_ICONWARNING);
		break;
	case 2:
		MessageBox(hw, "two", "NWP", MB_ICONWARNING);
		break;
	}
}

void on_destroy() 
{
	::PostQuitMessage(0);
}

LRESULT CALLBACK window_proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CREATE:
			on_create(hw);
			return 0;
		case WM_COMMAND:
			on_command(hw, LOWORD(wp));
			return 0;
		case WM_DESTROY:
			on_destroy();
			return 0;
	}
	return ::DefWindowProc(hw, msg, wp, lp);
}


int register_class(HINSTANCE hi, const char* name)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = window_proc;
	wc.lpszClassName = name;
	wc.hInstance = hi;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (::CreateSolidBrush(RGB(0, 255, 255))); 
	return ::RegisterClass(&wc);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR cmd_line, int show_flag)
{
	const auto class_name = "NWPClass";
	if (!register_class(hi, class_name))
		return 0;
	::CreateWindow(class_name, "NWP 1", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hi, 0);
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0))
		::DispatchMessage(&msg);
	return msg.wParam;
}
