#include "TheSnakesGame.h"
using namespace std;

void main()
{
	Utilities().ShowConsoleCursor(false);
	TheSnakesGame theGame;
	theGame.welcomeMessage();
	theGame.run(TheSnakesGame::EMPTY_CHOICE);
}

