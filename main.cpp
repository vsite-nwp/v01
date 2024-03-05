#include <windows.h>

enum { id_button1 = 1, id_button2 };

void on_create(HWND hw)
{
	//create two child windows of type button
	CreateWindow("BUTTON", "One", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 50, 100, 30, hw, reinterpret_cast<HMENU>(id_button1), nullptr, nullptr);

	CreateWindow("BUTTON", "Two", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		50, 100, 100, 30, hw, reinterpret_cast<HMENU>(id_button2), nullptr, nullptr);
}

void on_command(HWND hw, int id)
{
	//show message box with text depending on which button was pressed
	LPCTSTR lpText = nullptr;
	LPCTSTR lpCaption = nullptr;

	switch (id)
	{
	case id_button1:
		lpText = "Pritisnut gumb One!";
		lpCaption = "Button One";
		break;
	case id_button2:
		lpText = "Pritisnut gumb Two!";
		lpCaption = "Button Two";
		break;
	default:
		break;
	}

	if (lpText && lpCaption)
	{
		MessageBox(hw, lpText, lpCaption, MB_OK | MB_ICONWARNING);
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
	wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 255)); //replace with cyan background
	wc.hbrBackground = hBrush;
	return ::RegisterClass(&wc);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR cmd_line, int show_flag)
{
	const auto class_name = "NWPClass";
	if (!register_class(hi, class_name))
		return 0;
	::CreateWindow(class_name, "NWP 1", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hi, nullptr);
	MSG msg;
	while (::GetMessage(&msg, nullptr, 0, 0))
		::DispatchMessage(&msg);
	return msg.wParam;
}
