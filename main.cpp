#include <windows.h>

enum { id_button1 = 1, id_button2 };

void on_create(HWND hw) 
{
	// TODO: create two child windows of type button
	::CreateWindow("BUTTON", "one", WS_CHILD | WS_VISIBLE, 50, 50, 100, 30, hw, (HMENU)id_button1, 0, 0);
	::CreateWindow("BUTTON", "two", WS_CHILD | WS_VISIBLE, 50, 90, 100, 30, hw, (HMENU)id_button2, 0, 0);
}

void on_command(HWND hw, int id) 
{
	// TODO: show message box with text depending on which button was pressed
	switch (id) {
	case 1:
		::MessageBox(hw, "one", "NWP", MB_ICONEXCLAMATION | MB_OK);
		break;
	case 2:
		::MessageBox(hw, "two", "NWP", MB_ICONEXCLAMATION | MB_OK);
		break;
	}
}

void on_destroy() 
{
	::PostQuitMessage(0);  // API procedura koja šalje poruku WM_QUIT na što GetMessage vraæa 0 i program završava.
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
	return ::DefWindowProc(hw, msg, wp, lp);  // N.B. DefWindowProc odraðuje svo defaultno ponašanje za sve poruke koje nismo definirali u switch caseu; osigurava da svaka poruka bude obraðena.
}

int register_class(HINSTANCE hi, const char* name)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = window_proc;  // Pointer na funkciju koja je zadužena za baratanje prozorima ove klase prozora. Eno je iznad.
	wc.lpszClassName = name;  // Naziv klase prozora.
	wc.hInstance = hi;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	//wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));  // TODO: replace with cyan background
	wc.hbrBackground = ::CreateSolidBrush(RGB(0, 255, 255)); // Cyan background
	return ::RegisterClass(&wc);  // RegisterClass API funkcija prima strukturu WNDCLASS koju smo popunili s informacijama koje su Windowsima dovoljne za registraciju nove klase.
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR cmd_line, int show_flag)
{
	const auto class_name = "NWPClass";  // Definicija stringa koji želimo da bude naziv naše klase prozora.
	if (!register_class(hi, class_name))  // Ako je register_class vratila  0, prekini program, ako vrati nešto što je !0 - uspjeli smo registrirati novu klasu. Onda ako pozovemo CreateWindow s tim stringom, Windowsi æe znati što treba.
		return 0;
	::CreateWindow(class_name, "NWP 1", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 250, 250, 0, 0, hi, 0);
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0))  // Message loop. Èekamo na poruku od OS-a. Kad doðe poruka uzimamo ju iz message queuea (FIFO) sa GetMessage.
		::DispatchMessage(&msg);  // DispatchMessage provjerava koje je klase taj prozor i poziva našu window funkciju za tu klasu.
	return msg.wParam;
}
