#include "TheSnakesGame.h"

// CTOR/DTOR

void TheSnakesGame::run(MAIN_MENU runmode)
{
	char userChoice;
	if (runmode == EMPTY_CHOICE)
		userChoice = mainMenu();
	else
		userChoice = runmode;

	Utilities().ClearScreen();
	if (userChoice == INSTRUCTIONS) // need to update Instruction
	{
		printInstructions();
		_getch();
		Sleep(200);
		Utilities().ClearScreen(); // Clear Screen.
		run(EMPTY_CHOICE);
	}
	else if (userChoice == RUN_GAME)
	{
		unsigned missionNum = 0, difficulty;
		Game game;
		difficulty = chooseDifficulty();
		game.run(0, difficulty);
	}
	else if (userChoice == EXIT)
	{
		int x_printing_location = (SCREEN_WIDTH / 2 - 1) - (strlen("Thank You and Good Bye =]\n") / 2);
		int y_printing_location = SCREEN_HEIGHT / 2 - 1;
		gotoxy(x_printing_location, y_printing_location);
		cout << "Thank You and Good Bye =]\n";
		gotoxy(x_printing_location, y_printing_location+1);
	}
	else
	{
		cout << "incorrect Input, please enter again";
		for (size_t i = 0; i < 3; i++)
		{
			cout << '.';
			Sleep(500);
		}
		Utilities::ClearScreen(); // Clear Screen.
		run(EMPTY_CHOICE);
	}
}

char TheSnakesGame::mainMenu()
{
	char userSelection;
	string yourString;
	int x_printing_location = SCREEN_WIDTH / 2 -1  - strlen("Present Instruction\n") / 2;
	int y_printing_location = (SCREEN_HEIGHT - 1) / 2 - 1;

	Utilities::setColor(LIGHT_BLUE);
	gotoxy(x_printing_location, y_printing_location);
	cout << "< 1 > Present Instruction\n";
	gotoxy(x_printing_location, y_printing_location + 1);
	cout << "< 2 > Play Game\n";
	gotoxy(x_printing_location, y_printing_location + 2);
	cout << "< 9 > Exit" << endl;
	Utilities::resetColor();
	userSelection = _getch();
	Sleep(200);
	return  userSelection;
}

// Print
void TheSnakesGame::welcomeMessage()
{
	cout
		<< "                      __    __    __    __" << endl
		<< "                     /  \\  /  \\  /  \\  /  \\ " << endl
		<< "____________________/  __\\/  __\\/  __\\/  __\\_____________________________" << endl
		<< "___________________/  /__/  /__/  /__/  /________________________________" << endl
		<< "                   | / \\   / \\   / \\   / \\  \\____ " << endl
		<< "                   |/   \\_/   \\_/   \\_/   \\    o \\ " << endl
		<< "                                           \\_____/--<" << endl;
	gotoxy(0, SCREEN_HEIGHT - 1);
	printAuthorsNames();
	Sleep(2000);
	system("cls");
}

void TheSnakesGame::printInstructions()
{
	Utilities::setColor(RED);
	// Print Instructions designed text.
	cout << " __ __ _ ____ ____ ____ _  _  ___ ____ __ __  __ _ ____" << endl
		<< "(  (  ( / ___(_  _(  _ / )( \\/ __(_  _(  /  \\(  ( / ___)" << endl
		<< " )(/    \\___ \\ )(  )   ) \\/ ( (__  )(  )(  O /    \\___ \\" << endl
		<< "(__\\_)__(____/(__)(__\\_\\____/\\___)(__)(__\\__/\\_)__(____/" << endl << endl << endl;
	Utilities::resetColor();

	cout << "In Each level There is a mission to solve\n"
		"random numbers will be printing on the board.\n"
		"Each player have 1 snake.\n"
		"Their set of keys:\n"
		"First Player Keys:  a,w,d,x and z for shooting,Length: 3,Color: ";
	Utilities::setColor(BLUE);
	cout << "Blue\n";
	Utilities::resetColor();
	cout << "Second Player Keys:  j,i,l,m and n for shooting ,Length: 3,Color: ";
	Utilities::setColor(GREEN);
	cout << "Green\n";
	Utilities::resetColor();

	cout << "A Snake can pass to the other side of the screen if nothing disturb it.\n"
		"If the Snakes Head eats the correct number, the snake grows by 1.\n"
		"Otherwise, The other snake grows by 1.\nUntil Reaching 12."
		"When Reaching 12, The Game is Over.\n"
		"Each level, The player receives 5 shots,\n"
		"They can hit a number or a player, use them wisely!\n"
		"When a player gets shot, he disappears for 5 turns.\n"
		"When a number gets hit, he disappears.\n\n\n"
		"Press any key for Main Menu\n";
}

void TheSnakesGame::printAuthorsNames()
{
	Sleep(1000);
	PlaySound(TEXT("Typing1.wav"), nullptr, SND_NODEFAULT);
	cout << "   B";
	Sleep(30);
	cout << "Y";
	Sleep(30);
	PlaySound(TEXT("Typing1.wav"), nullptr, SND_NODEFAULT);
	cout << ":";
	Sleep(30);
	PlaySound(TEXT("Typing1.wav"), nullptr, SND_NODEFAULT);
	cout << " ";
	Sleep(30);
	cout << "E";
	Sleep(30);
	cout << "L";
	Sleep(30);
	cout << "A";
	Sleep(30);
	cout << "D";
	Sleep(30);
	cout << " ";
	Sleep(30);
	PlaySound(TEXT("Typing1.wav"), nullptr, SND_NODEFAULT);
	cout << "A";
	Sleep(30);
	cout << "N";
	Sleep(30);
	PlaySound(TEXT("Typing1.wav"), nullptr, SND_NODEFAULT);
	cout << "D";
	Sleep(30);
	cout << " ";
	Sleep(30);
	cout << "Y";
	Sleep(30);
	cout << "O";
	Sleep(30);
	cout << "N";
	Sleep(30);
	cout << "I";
	Sleep(30);
	PlaySound(TEXT("Typing1.wav"), nullptr, SND_NODEFAULT);
}

void TheSnakesGame::printDifficultyMenu()
{
	int x_printing_location = SCREEN_WIDTH / 2 -1 - strlen("< 1 > Easy\n") / 2;
	int y_printing_location = (SCREEN_HEIGHT - 1) / 2 - 1;

	Utilities::setColor(LIGHT_BLUE);
	gotoxy(x_printing_location, y_printing_location);
	cout << "< 1 > Easy\n";
	gotoxy(x_printing_location, y_printing_location + 1);
	cout << "< 2 > Normal\n";
	gotoxy(x_printing_location, y_printing_location + 2);
	cout << "< 3 > Hard" << endl;
	Utilities::resetColor();
}

unsigned TheSnakesGame::chooseDifficulty()
{
	char userChoice;

	printDifficultyMenu();
	userChoice = _getch() - 48;
	
	switch (userChoice)
	{
	case EASY:
		break;
	case NORMAL:
		break;
	case HARD:
		break;
	default:
		cout << "Incorrect input, Normal was selected for you";
		for (size_t i = 0; i < 3; i++)
		{
			cout << '.';
			Sleep(500);
		}
		userChoice = NORMAL;
		break;
	}
	Sleep(200);
	return  userChoice;
}
