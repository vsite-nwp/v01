#include <windows.h>
#include <map>
#include <vector>
#include <ostream>
#include <exception>
#include "Header.h"
#include <string>


//enum { id_button1 = 1, id_button2 };

//static class members initialization
int UI::UIObject::counter = 0;
std::map<UI::UIObject*, int, UI::less_UIObject> UI::Grid::Rows = {};
std::map<UI::UIObject*, int, UI::less_UIObject> UI::Grid::Columns = {};
std::map<int,UI::UIObject*> UI::UIObject::AllInstances = {};

using namespace UI;
void OnCreate(HWND hw) {
	Thickness* Margin = new Thickness(20);
	Grid* glavniGrid = new Grid(*(new WindowPosition(hw)), "GlavniGrid", hw, 0, 0, Margin, HorizontalAlignments::Center, VerticalAlignments::Center);
	
	//defining columns
	glavniGrid->ColumnDefinitions.Add(new ColumnDefinition(new GridLength(1, GridLengthType::Star)));
	glavniGrid->ColumnDefinitions.Add(new ColumnDefinition(new GridLength(200, GridLengthType::Pixel)));
	glavniGrid->ColumnDefinitions.Add(new ColumnDefinition(new GridLength(1, GridLengthType::Star)));

	//defining rows
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(25, GridLengthType::Pixel)));
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));	
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));
	glavniGrid->RowDefinitions.Add(new RowDefinition(new GridLength(1, GridLengthType::Star)));


	for (int i = 0; i < glavniGrid->RowDefinitions.Count(); ++i) {
		for (int j = 0; j < glavniGrid->ColumnDefinitions.Count(); ++j) {
			string& naziv="row:"+to_string(i)+"column:"+to_string(j);
			LPCSTR naziv2 =(LPCSTR) naziv.c_str();
			Button* btn = new Button(*(new WindowPosition(hw)), naziv2, hw, 0, 0, new Thickness(5), HorizontalAlignments::Center, VerticalAlignments::Center);
			Grid::SetRow(btn, i);
			Grid::SetColumn(btn, j);
			glavniGrid->Children.push_back(btn);
		}
	}

	//Loading UIObject and Children
	glavniGrid->LoadUIObject();
	glavniGrid->LoadChildren();


	//HWND button1 = CreateWindow("BUTTON", "OK", WS_VISIBLE | WS_CHILD, 10, 10, 100, 100, hw, (HMENU)id_button1, NULL, NULL);
	//HWND button2 = CreateWindow("BUTTON", "CANCEL", WS_VISIBLE | WS_CHILD, 10, 110, 100, 100, hw, (HMENU)id_button2, NULL, NULL);
}


void OnCommand(HWND hw, int id) {
	LPSTR lpstr = new char[25];
	GetWindowTextA(hw, lpstr, 25);
	//sender je button jer samo oni reagiraju na OnCommand tako da možemo direktni cast koristiti
	Button* btn = dynamic_cast<Button*>(UIObject::getInstanceById(id));
	UINT tip = id % 2 == 0 ? MB_OK | MB_ICONINFORMATION : MB_OKCANCEL | MB_ICONWARNING;
	MessageBox(hw,(LPCSTR) btn->message.c_str(), lpstr,tip );
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

	if (!RegisterMyClass(hInstance, clsName))
		return 0;

	HWND hwnd = CreateWindow(clsName,"NWP 1", WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
		100, 100, 1000, 500,
		NULL, NULL, hInstance, NULL);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);

	return msg.wParam;
}