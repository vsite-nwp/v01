#pragma once
#include <map>
#include <vector>
#include <ostream>
#include <exception>
#include <windows.h>

namespace UI {
	using namespace std;
	//enums
	enum class GridLengthType {
		Pixel,
		Star
	};

	enum class VerticalAlignments {
		Top,
		Center,
		Bottom,
		Stretch
	};

	enum class HorizontalAlignments {
		Left,
		Center,
		Right,
		Stretch
	};

	//General classes

	template <class T>
	class Enumerable {
	public:

		//add at the back
		void Add(T* item) {
			vektor.push_back(item);
		}

		//remove item
		bool Remove(const T* item) {
			for (int i = 0; i < vektor.size(); ++i) {
				if (vektor[i] == item) {
					vektor.erase(vektor.begin() + i);
					return true;
				}
			}
			return false;
		}

		//remove at index
		bool RemoveAt(int i) {
			if (i >= vektor.size()) {
				return false;
			}
			else {
				vektor.erase(vektor.begin() + i);
				return true;
			}
		}


		//return count
		int Count() const {
			return vektor.size();
		}

		//return item
		T* operator[](int i) const {
			if (i < vektor.size()) {
				return vektor[i];
			}
			else {
				throw out_of_range("Index is out of range!");
			}
		}
		~Enumerable() {
			vektor.clear();
		}
	protected:
		vector<T*> vektor;
	};

	
	//Position structs

	struct Thickness {
		double Top = 0;
		double Left = 0;
		double Right = 0;
		double Bottom = 0;
		Thickness(double uniformThickness) {
			Top = uniformThickness;
			Left = uniformThickness;
			Right = uniformThickness;
			Bottom = uniformThickness;
		}
		Thickness() {
			Top = 0;
			Left = 0;
			Right = 0;
			Bottom = 0;
		}
	};

	struct WindowPosition {
		int x;
		int y;
		int Width;
		int Height;
		WindowPosition(int x, int y, int Width, int Height)
		{
			this->x = x;
			this->y = y;
			this->Width = Width;
			this->Height = Height;
		}
		WindowPosition()
		{
			this->x = 0;
			this->y = 0;
			this->Width = 0;
			this->Height = 0;
		}
		WindowPosition(HWND hw) {
			LPRECT r = new RECT();
			GetClientRect(hw, r);
			this->x = r->left;
			this->y = r->top;
			this->Width = r->right - r->left;
			this->Height = r->bottom - r->top;
		}
	};

	//UI element "helper" classes
	class GridLength {
	public:
		GridLength() {
			Length = 0;
			Type = GridLengthType::Pixel;
		}
		GridLength(double Length, GridLengthType Type) {
			this->SetLength(Length);
			this->Type = Type;
		}
		double GetLength() const {
			return Length;
		}
		void SetLength(double Length) {
			if (Length <= 0) {
				throw invalid_argument("Length must be greater than zero!");
			}
			else
				this->Length = Length;
		}
		GridLengthType Type;

	private:
		double Length;

	};
	

	class RowDefinition {
	public:
		RowDefinition(GridLength* height) {
			*Height = *height;
		}
		~RowDefinition() {
			delete Height;
		}
		GridLength* Height = new GridLength();
	};

	class ColumnDefinition {
	public:
		ColumnDefinition(GridLength* width) {
			*Width = *width;
		}
		GridLength* Width=new GridLength();
	};

	class RowDefinitionCollection :public Enumerable<RowDefinition> {
	public:
		//Row heights need to be calculated because of star GridLengths which represent ratio
		vector<double> RowHeightsInPixels(const WindowPosition& position) const {
			vector<double> redoviVisine;
			double sumPixelRows = CalculatePixelSum();
			double sumStarRows = CalculateStarSum();
			double PixelsForStars = (position.Height - sumPixelRows) < 0 ? 0 : (position.Height - sumPixelRows);
			for (int i = 0; i < Count(); ++i) {
				if (vektor[i]->Height->Type == GridLengthType::Pixel)
					redoviVisine.push_back(vektor[i]->Height->GetLength());
				else
					redoviVisine.push_back(vektor[i]->Height->GetLength() / sumStarRows * PixelsForStars);
			}
			return redoviVisine;
		}


	public:
		double CalculateStarSum() const {
			double sum = 0;
			for (int i = 0; i < vektor.size(); ++i)
				if (vektor[i]->Height->Type == GridLengthType::Star)
					sum += vektor[i]->Height->GetLength();
			return sum;
		}
		double CalculatePixelSum() const {
			double sum = 0;
			for (int i = 0; i < vektor.size(); ++i)
				if (vektor[i]->Height->Type == GridLengthType::Pixel)
					sum += vektor[i]->Height->GetLength();
			return sum;
		}
	};

	class ColumnDefinitionCollection :public Enumerable<ColumnDefinition> {
	public:
		vector<double> ColumnWidthsInPixels(const WindowPosition& position) const {
			vector<double> redoviVisine;
			double sumPixelRows = CalculatePixelSum();
			double sumStarRows = CalculateStarSum();
			double PixelsForStars = (position.Width - sumPixelRows) < 0 ? 0 : (position.Width - sumPixelRows);
			for (int i = 0; i < Count(); ++i) {
				if (vektor[i]->Width->Type == GridLengthType::Pixel)
					redoviVisine.push_back(vektor[i]->Width->GetLength());
				else
					redoviVisine.push_back(vektor[i]->Width->GetLength() / sumStarRows * PixelsForStars);
			}
			return redoviVisine;
		}
	public:
		double CalculateStarSum() const {
			double sum = 0;
			for (int i = 0; i < vektor.size(); ++i)
				if (vektor[i]->Width->Type == GridLengthType::Star)
					sum += vektor[i]->Width->GetLength();
			return sum;
		}
		double CalculatePixelSum() const {
			double sum = 0;
			for (int i = 0; i < vektor.size(); ++i)
				if (vektor[i]->Width->Type == GridLengthType::Pixel)
					sum += vektor[i]->Width->GetLength();
			return sum;
		}
	};

	//User interface base abstract class UIObject and ParentUIObject
	class UIObject {
	public:

		//public static members
		static map<int, UIObject*> AllInstances;
		int id = -1;

		//public members

		string name;
		HWND hw = nullptr;
		WindowPosition* ParentBox = new WindowPosition();
		WindowPosition* Position = new WindowPosition();

		//virtual methods

		virtual void LoadUIObject() = 0;


		//destructor
		~UIObject() {
			delete hw;
			delete Position;
			delete ParentBox;
			delete parent;
			delete Margin;
		}

		//static methods
		static UIObject* getInstanceById(int id) {
			return AllInstances[id];
		}

		//constructor
		UIObject(WindowPosition ParentBox, string name = "", HWND parent = nullptr, double height = 0, double width = 0, Thickness* margin = new Thickness(),
			HorizontalAlignments horizontalAlignment = HorizontalAlignments::Left, VerticalAlignments verticalAlignment = VerticalAlignments::Top, string className = "STATIC")
		{
			id = ++counter;
			this->name = name;
			this->parent = parent;
			*this->ParentBox = ParentBox;
			Height = height;
			Width = width;
			*Margin = *margin;
			HorizontalAlignment = horizontalAlignment;
			VerticalAlignment = verticalAlignment;
			CalculateObjectPosition();
			SetObjectPosition();
			hw = CreateWindow((LPCSTR)className.c_str(), (LPCSTR)name.c_str(), WS_VISIBLE | WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, (HMENU)(id), NULL, NULL);
			AllInstances.insert(pair<int, UIObject*>(id, this));
		}
		//public methods
		void SetParentBox(const WindowPosition ParentBox) {
			*this->ParentBox = ParentBox;
			CalculateObjectPosition();
			SetObjectPosition();
		}

		void SetObjectPosition() {
			SetWindowPos(hw, parent, Position->x, Position->y, Position->Width, Position->Height, SWP_NOZORDER);
		}

		void SetHorizontalAlignment(HorizontalAlignments HorizontalAlignment) {
			this->HorizontalAlignment = HorizontalAlignment;
			CalculateObjectPosition();
			SetObjectPosition();
		}
		void SetVerticalAlignment(VerticalAlignments VerticalAlignment) {
			this->VerticalAlignment = VerticalAlignment;
			CalculateObjectPosition();
			SetObjectPosition();
		}
		void SetHeight(double height) {
			this->Height = Height;
			CalculateObjectPosition();
			SetObjectPosition();
		}
		void SetWidth(double width) {
			this->Height = Height;
			CalculateObjectPosition();
			SetObjectPosition();
		}

		void CalculateObjectPosition() {
			if (HorizontalAlignment == HorizontalAlignments::Left) {
				Position->x = Margin->Left + ParentBox->x;
				Position->Width = Width;
			}
			else if (HorizontalAlignment == HorizontalAlignments::Center) {
				Position->x = Margin->Left + ParentBox->x;
				Position->Width = ParentBox->Width - Margin->Left - Margin->Right;
			}
			else if (HorizontalAlignment == HorizontalAlignments::Right) {
				Position->x = ParentBox->x + ParentBox->Width - Margin->Right - Position->Width;
				Position->Width = ParentBox->Width - Margin->Left - Margin->Right;
			}
			else if (HorizontalAlignment == HorizontalAlignments::Stretch) {
				//todo Implement Stretch
			}
			if (VerticalAlignment == VerticalAlignments::Top) {
				Position->y = ParentBox->y + Margin->Top;
				Position->Height = Height;
			}
			else if (VerticalAlignment == VerticalAlignments::Center) {
				Position->y = ParentBox->y + Margin->Top;
				Position->Height = ParentBox->Height - Margin->Top - Margin->Bottom;
			}
			else if (VerticalAlignment == VerticalAlignments::Bottom) {
				Position->y = ParentBox->y + ParentBox->Height - Margin->Bottom - Position->Width;
				Position->Height = Height;
			}
			else if (VerticalAlignment == VerticalAlignments::Stretch) {
				//todo Implement Stretch
			}
		}
	protected:
		double Height = 0;
		double Width = 0;
		Thickness* Margin = new Thickness();
		HorizontalAlignments HorizontalAlignment = HorizontalAlignments::Left;
		VerticalAlignments VerticalAlignment = VerticalAlignments::Top;
		HWND parent = nullptr;
		static int counter;
	};
	
	class ParentUIObject {
	public:
		std::vector<UI::UIObject*> Children;
		void LoadChildren() {
			for (int i = 0; i < Children.size(); ++i) {
				Children[i]->LoadUIObject();
			}
		};
		ParentUIObject() {}
		~ParentUIObject() {
			Children.clear();
		}
	};

	//operators
	bool operator<(const GridLength& gr1, const GridLength& gr2) {
		if (gr1.Type == gr2.Type) {
			return gr1.GetLength() < gr2.GetLength();
		}
		else if (gr1.Type == GridLengthType::Pixel) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator<(const RowDefinition& row1, const RowDefinition& row2) {
		return row1.Height < row2.Height;
	}

	struct less_UIObject : binary_function<const UI::UIObject*, const UI::UIObject*, bool>
	{
		bool  operator() (const  UI::UIObject* a, const UI::UIObject* b) const {
			return a->id < b->id;
		}
	};

	// User interface objects which inherit from UIObject

	//Grid contains rows and columns which can be defined as star length types or pixel length types
	//Columns example 1*,250Px,1* with grid width of 400 px will result in 75px,250Px,75px grid
	class Grid :public UIObject, public ParentUIObject {
	public:
		RowDefinitionCollection RowDefinitions;
		ColumnDefinitionCollection ColumnDefinitions;
		map<tuple<int, int>, WindowPosition> GridCells;
		static map<UIObject*, int, less_UIObject> Rows;
		static map<UIObject*, int, less_UIObject> Columns;
		static void SetRow(UI::UIObject* element, const int row) {
			if (Rows.find(element) != Rows.end()) {
				Rows[element] = row;
			}
			else {
				Rows.insert(pair<UI::UIObject*, int>(element, row));
			}
		}
		static void SetColumn(UI::UIObject* element, const int column) {
			if (Columns.find(element) != Columns.end()) {
				Columns[element] = column;
			}
			else {
				Columns.insert(pair<UI::UIObject*, int>(element, column));
			}
		}
		static int GetRow(UI::UIObject* element) {
			if (Rows.find(element) != Rows.end()) {
				return Rows[element];
			}
			else {
				return 0;
			}
		}
		static int GetColumn(UI::UIObject* element) {
			if (Columns.find(element) != Columns.end()) {
				return Columns[element];
			}
			else {
				return 0;
			}
		}
		Grid(WindowPosition ParentBox, string name = "", HWND parent = nullptr, double height = 0, double width = 0, Thickness* margin = new Thickness(),
			HorizontalAlignments horizontalAlignment = HorizontalAlignments::Left, VerticalAlignments verticalAlignment = VerticalAlignments::Top) :
			UIObject(ParentBox, name, parent, height, width, margin, horizontalAlignment, verticalAlignment,"STATIC") {
		}
		void LoadUIObject() {
			CalculateObjectPosition();
			SetWindowPos(hw, parent, Position->x, Position->y, Position->Width, Position->Height, SWP_NOZORDER);
		}
		void LoadChildren() {
			CalculateGridCells();
			for (int i = 0; i < Children.size(); i++) {
				int row = GetRow(Children[i]);
				int column = GetColumn(Children[i]);
				Children[i]->SetParentBox(GridCells[make_tuple(row,column)]);
				Children[i]->LoadUIObject();
			}
		}

	private:
		void CalculateGridCells() {
			this->GridCells.clear();
			vector<double> redoviVisine = RowDefinitions.RowHeightsInPixels(*Position);
			vector<double> stupciSirine = ColumnDefinitions.ColumnWidthsInPixels(*Position);
			double passedColumnsSum = 0;
			double passedRowsSum = 0;
			for (int i=0; i < stupciSirine.size(); ++i) {
				passedRowsSum = 0;
				for (int j=0; j < redoviVisine.size(); ++j) {
					WindowPosition* pos = new WindowPosition(Position->x+passedColumnsSum,Position->y+passedRowsSum, stupciSirine[i], redoviVisine[j]);
					passedRowsSum += redoviVisine[j];
					GridCells.insert(pair<tuple<int, int>, WindowPosition>(make_tuple(j, i), *pos));
				}
				passedColumnsSum += stupciSirine[i];
			}
		}
	};

	class MainWindow :UIObject, ParentUIObject {
		//not implemented
	};

	class Button :public UIObject {
	public:
		string message;
		Button(WindowPosition ParentBox, string name = "", HWND parent = nullptr, double height = 0, double width = 0, Thickness* margin = new Thickness(),
			HorizontalAlignments horizontalAlignment = HorizontalAlignments::Left, VerticalAlignments verticalAlignment = VerticalAlignments::Top) :
			UIObject(ParentBox, name, parent, height, width, margin, horizontalAlignment, verticalAlignment,"BUTTON") {
			message=name+ (id % 2 == 0 ? " OK" : " CANCEL");
		}
		void LoadUIObject() {
		}
	};


}

