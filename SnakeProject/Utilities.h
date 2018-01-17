#ifndef _Utilities_h
#define _Utilities_h

enum COLORS { WHITE_DEFAULT = 7,BLUE = 9, GREEN = 10 ,LIGHT_BLUE = 11, RED = 12,YELLOW = 14};

class Utilities
{
public:
	enum numOfDigits { ONE = 1, TWO, THREE };
	
	// Functions
	void ShowConsoleCursor(bool showFlag);
	static unsigned  CalculateNumDigits(unsigned num);
	static void setColor(int choosecolor);
	static void resetColor();
	static void ClearScreen();
};



#endif