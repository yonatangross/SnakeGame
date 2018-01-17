#include "Utilities.h"
#include "Windows.h"

// Functions
void Utilities::ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
unsigned Utilities::CalculateNumDigits(unsigned num)
{
	if (num < 10) // Two digits indicator.
		return ONE;
	if (num < 100) // Three digits indicator.
		return TWO;
	return THREE;
}
void Utilities::ClearScreen()
{
	system("cls");
}
void Utilities::setColor(int  choosecolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), choosecolor); //FUNCTION OF COLOR
}
void Utilities::resetColor()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_DEFAULT);
}
