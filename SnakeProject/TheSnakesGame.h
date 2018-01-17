#ifndef _TheSnakesGame_h
#define _TheSnakesGame_h

#include <iostream>
#include <string>
#include "Game.h"
#include <conio.h>
#include <Windows.h>
#include "Utilities.h"
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

enum DIFFICULTY { EASY=1, NORMAL, HARD };

class TheSnakesGame
{

	static char mainMenu();
public:
	enum MAIN_MENU { INSTRUCTIONS = '1', RUN_GAME = '2', EXIT = '9', EMPTY_CHOICE = '10' };

	void printAuthorsNames();
	void printDifficultyMenu();
	unsigned chooseDifficulty();
	void welcomeMessage();
	static void printInstructions();
	// Functions
	void run(MAIN_MENU runmode);
};

#endif