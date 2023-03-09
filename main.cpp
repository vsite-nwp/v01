#include <windows.h>

enum { id_button1 = 1, id_button2 };

void on_create(HWND hw) 
{
	// create two child windows of type button
	::CreateWindow("Button", "one", WS_CHILD | WS_VISIBLE, 500, 300, 100, 25, hw, (HMENU)id_button1, 0, 0);
	::CreateWindow("Button", "two", WS_CHILD | WS_VISIBLE, 500, 350, 100, 25, hw, (HMENU)id_button2, 0, 0);
}

void on_command(HWND hw, int id) 
{
	// show message box with text depending on which button was pressed
	LPCSTR value = "";
	switch (id) {
	case id_button1:
		value = "one";
		break;
	case id_button2:
		value = "two";
		break;
	}

	::MessageBox(hw, value, "NWP", MB_OK | MB_ICONEXCLAMATION);


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
	wc.hCursor = ::LoadCursor(0, IDC_CROSS);
	wc.hbrBackground = ::CreateSolidBrush(RGB(0, 255, 255));  // replace with cyan background
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
