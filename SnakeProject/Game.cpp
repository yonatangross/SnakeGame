#include "Game.h"
#include "Mission.h"
#include "Utilities.h"
#include <fstream>

// CTOR/DTOR
Game::~Game() //DESTRUCTOR
{
	for (size_t i = 0; i < NUM_OF_MISSIONS; i++)
		delete missions[i];
	for (size_t i = 0; i < NUM_OF_MONSTERS; i++)
		delete monsters[i];
}

//Missions Handle
void Game::setMissions(unsigned difficulty)
{
	string filename = GetFileName(difficulty);
	ifstream inFile(filename);
	list<string> missionFormats;
	string inputLine;

	while (getline(inFile, inputLine))
		missions.push_back(GetMission(inputLine));

	inFile.close();
}
string Game::GetFileName(unsigned difficulty)
{
	string fileName;
	switch (difficulty)
	{
	case EASY: fileName = "easyMissions.txt"; break;
	case NORMAL: fileName = "normalMissions.txt"; break;
	case HARD: fileName = "hardMissions.txt"; break;
	default:break;
	}
	return fileName;
}
void Game::InterpretToken(const string& token, unsigned counter, string& mission_name, string& description, string& num1_str, string& num2_str)
{
	switch (counter)
	{
	case 0:
		mission_name = token;
		break;
	case 1:
		description = token;
		break;
	case 2:
		num1_str = token;
		break;
	case 3:
		num2_str = token;
		break;
	default: break;
	}
}
Mission* Game::GetMission(string missionFormat)
{
	string delimiter = ",";
	size_t pos = 0;
	string token, missionName, MissionDescription, num1Str, num2Str;
	int num1 = -1, num2 = -1;
	unsigned counter = 0;

	while ((pos = missionFormat.find(delimiter)) != string::npos) {
		token = missionFormat.substr(0, pos);
		InterpretToken(token, counter, missionName, MissionDescription, num1Str, num2Str);
		missionFormat.erase(0, pos + delimiter.length());
		counter++;
	}
	if (!num1Str.empty()) num1 = stoi(num1Str);
	if (!num2Str.empty()) num2 = stoi(num2Str);
	return MakeMissionFromParamaters(missionName, MissionDescription, num1, num2);
}
Mission* Game::MakeMissionFromParamaters(const string& missionName, const string& missionDescription, int num1, int num2)
{
	if (num1 < 0 || num1 > 169) num1 = 1;
	if (num2 < 0 || num2 > 169) num2 = 10;

	Mission *newMission = nullptr;
	if (missionName == "Prime")
		newMission = new Mission(missionDescription, Prime);
	else if (missionName == "DivisionInX")
	{
		if (num1 == 0) num1 = 1;
		newMission = new Mission(missionDescription, DivisionInX, num1, -1);
	}
	else if (missionName == "Multiplication")
		newMission = new Mission(missionDescription, Multiplication, num1, -1);
	else if (missionName == "SquareRoot")
		newMission = new Mission(missionDescription, SquareRoot);
	else if (missionName == "DivisionWithReminder")
	{
		if (num1 == 0) num1 = 1;
		newMission = new Mission(missionDescription, DivisionWithReminder, num1, num2);
	}
	else if (missionName == "squareOfX")
	{
		if (num1 > 13 || num1 < -13) num1 = 13;
		newMission = new Mission(missionDescription, squareOfX, num1, -1);
	}
	else if (missionName == "EquationMission")
	{
		newMission = new Mission(missionDescription, EquationMission);
		newMission->CreateEquationMission();
	}
	else if (missionName == "NumUnderX")
	{
		if (num1 == 0) num1 = 169;
		newMission = new Mission(missionDescription, numUnderX, num1, 0);
	}
	else if (missionName == "NumBetweenXY")
	{
		if (num2 < num1) swap(num1, num2);
		newMission = new Mission(missionDescription, numBetweenXY, num1, num2);
	}
	else if (missionName == "NumFollowingX")
	{
		if (num1 < -1 || num1 > 168) num1 = 168;
		newMission = new Mission(missionDescription, numFollowingX, num1, 0);
	}
	return newMission;
}

//Game Handling
void Game::run(unsigned int missionNumber, unsigned difficulty)
{
	Board newBoard;
	replaySuitCase replaySuit{};
	unsigned int numOfSteps = 0;
	bool gameOver = false;
	static bool ExitGame = false, replay = false, suitCaseLoaded = false, backToReplay = false;

	for (auto snake : snakes)
		snake.resetShots();
	setMissions(difficulty);
	setMonsters();
	gotoxy(0, 0);
	missions[missionNumber]->printInstructions();

	while (!gameOver)
	{
		char keyPressed = 0, keyAfterEsc;
		if (replay)
		{
			LoadSuitCase(replaySuit, missionNumber, newBoard, suitCaseLoaded, numOfSteps); //TODO:MISSIONS equations are different at replay
			GetKeyPressedFromList(replaySuit, keyPressed);

			if (keyPressed == ESC) //END OF REPLAY
			{
				replay = false;
				resetLists(replaySuit, replay);
				handleSideMenu(missionNumber, ExitGame, replay, suitCaseLoaded, newBoard, keyAfterEsc);
				if (replay)	backToReplay = true;
				else if (keyPressed == ESC) replaySuit.keyboardHits.pop_front();
			}
		}
		else
		{
			saveSuitCase(replaySuit, newBoard, numOfSteps);			    //SAVE GAME DATA FOR REPLAY
			getInput(keyPressed, missionNumber, ExitGame, replay, suitCaseLoaded, newBoard, keyAfterEsc);
			if (replay)	backToReplay = true;
			if (ExitGame) break; //get out of the loop - Because the user want to Exit the game
			if (keyPressed == ESC && keyAfterEsc != REPLAY_MISSION);
			else replaySuit.keyboardHits.push_front(keyPressed);//SAVE the user input to the list
		}
		if (!backToReplay)
		{//IF IT IS A SECOND REPLAY OR SO - DONT EXECUTE THIS STEP AND WAIT FOR THE KEYBOARD INPUT FROM THE LIST
			if (ExecuteStep(missionNumber, newBoard, numOfSteps, gameOver, keyPressed, replaySuit, replay)) break;//Break if GameOver
			handleMovement(numOfSteps, newBoard, missionNumber, gameOver, replay, replaySuit);
		}
		backToReplay = false;
	}
	handleEnd(ExitGame);
}
void Game::reguliseInput(char &key_pressed, unsigned int missionNumber)
{

	if (key_pressed < 'A' || key_pressed > 'z') //NOT ENGLISH
	{
		while (key_pressed < 'A' || key_pressed > 'z')
		{
			system("cls");
			gotoxy(SCREEN_WIDTH / 2 - strlen("UNVALID INPUT PLEASE CHANGE LANGUAGE To ENGLISH AND ENTER A KEY") / 2, SCREEN_HEIGHT / 2);
			cout << "UNVALID INPUT PLEASE CHANGE LANGUAGE TO ENGLISH AND ENTER A KEY";
			key_pressed = _getch();
			system("cls");
		}
		missions[missionNumber]->printInstructions();
	}
	else									   //ENGLISH -MABEY CAPS LOCK
		key_pressed = char(tolower(key_pressed));

}
bool Game::HandleMission(unsigned &missionNumber, bool &gameOver, Board& newBoard, replaySuitCase& replaySuit, unsigned &numOfSteps, bool replay)
{
	if (gameOver) {		//Snake Length is 12
		ShowMessageEndOfGame();
		return true;			//Break The For Loop
	}
	incMissionNum(missionNumber);
	prepareNextMission(newBoard, missions[missionNumber], replaySuit, numOfSteps, replay);
	return false;
}
bool Game::ExecuteStep(unsigned &missionNumber, Board &newBoard, unsigned numOfSteps, bool& gameOver, char keyPressed, replaySuitCase &replaySuit, bool replay)
{
#define SPEED 200
	bool missionEnd = false;

	Sleep(SPEED); //SNAKE SPEED
	for (size_t i = 0; i < Snake::NUM_OF_SNAKES; ++i)
	{
		snakes[i].handleShots(newBoard, snakes, keyPressed, monsters);
		if (HandleMonsters(newBoard, gameOver, missionNumber))
		{
			missionEnd = true;
			break; //Mission End
		}

		if (snakes[i].active)
		{
			Point headPoint;
			if (numOfSteps > 1)		//OTHERWISE SNAKE IS STUCK IN THE BEGINING (NEED TO BE BUILD IN THE FIRST TIME) 
				snakes[i].checkIfStuck(snakes[(i + 1) % Snake::NUM_OF_SNAKES], keyPressed);
			snakes[i].move(keyPressed, snakes[i].isStuck);
			headPoint = snakes[i].getHead();
			if (!snakes[i].isStuck && newBoard.getPoint(headPoint).checkChar())//Return true newBoard[y][x].ch y != ' ' OR the numEater hit a snake
			{
				missionsType mission_type = missionsType(missions[missionNumber]->GetMissionNum());
				missions[missionNumber]->handleEat(newBoard.getPoint(headPoint), snakes, i, newBoard, mission_type, gameOver);
				missionEnd = true;
				break;
			}
		}
	}
	if (missionEnd)
		return HandleMission(missionNumber, gameOver, newBoard, replaySuit, numOfSteps, replay);
	return gameOver;
}
Point Game::findFreeLocation(const Board& board)
{
	unsigned x = (rand() % SCREEN_WIDTH), y = (rand() % BOARD_HEIGHT) + BOARD_START_LINE;
	bool found = false;
	while (found == false)
	{
		if (board.board[y][x].ch == ' ')
			return board.board[y][x];
		x = (rand() % SCREEN_WIDTH);
		y = (rand() % BOARD_HEIGHT) + BOARD_START_LINE;
	}
	return Point{ 1,1 }; // Default
}
void Game::repositionSnake(int snakeNumber, Board& board, replaySuitCase& replaySuit, bool replay)
{
	Point freeLocation;
	int curY = snakes[snakeNumber].getHead().getY();//local

	if (BOARD_START_LINE <= curY && curY < SCREEN_HEIGHT) //IF THE SNAKE IN THE BOARD HE HAVE TO CLEAR IT FROM THE CREEN
		snakes[snakeNumber].deleteSnakeFromScreen();
	if (!replay)
	{
		freeLocation = findFreeLocation(board);
		replaySuit.freeLocations.push_front(freeLocation);
	}
	else
	{
		freeLocation = replaySuit.freeLocations.back();
		replaySuit.freeLocations.pop_back();
		replaySuit.tempFreeLocations.push_front(freeLocation);
	}
	snakes[snakeNumber].setBodyPartLocation(0, freeLocation.getX(), freeLocation.getY());
	snakes[snakeNumber].isStuck = false;
}
void Game::unstuckSnakes()
{
	snakes[0].isStuck = snakes[1].isStuck = false;
}
void Game::prepareNextMission(Board& board, Mission* cur_mission, replaySuitCase &replaySuit, unsigned int &numOfSteps, bool replay)
{
	for (size_t i = 0; i < Snake::NUM_OF_SNAKES; i++)
	{
		if (snakes[i].isStuck)
			repositionSnake(i, board, replaySuit, replay);
		snakes[i].active = true;
		snakes[i].resetShots();
	}
	for (size_t i = 0; i < NUM_OF_MONSTERS; i++)
		monsters[i]->Activate();
	board.clearHalfBoard();
	cur_mission->printInstructions();
	board.printBoard();
	ClearLists(replaySuit);
	numOfSteps = 0;
}
void Game::handleMovement(unsigned int& numOfSteps, Board& board, unsigned int& missionNumber, bool gameOver, bool replay, replaySuitCase &replaySuit)
{
#define NUM_STEPS_TO_RESET 5
	if (gameOver)
		return;
	int counter;
	numOfSteps++;
	counter = numOfSteps % NUM_STEPS_TO_RESET;

	if (counter == 0) //Every NUM_STEPS_TO_RESET steps
	{
		for (size_t i = 0; i < Snake::NUM_OF_SNAKES; i++)
			if (snakes[i].HandleActivation())
				repositionSnake(i, board, replaySuit, replay);
		Point *monstersPoints[5];
		for (size_t k = 0; k < 5; k++)
			monstersPoints[k] = monsters[k];

		board.addNumToBoard(snakes, replay, replaySuit.numList, replaySuit.tempNumList, monstersPoints);
		if (board.getNumOfNumbers() == 60)
		{
			missions[missionNumber]->checkEndOfStage(board);
			incMissionNum(missionNumber);
			prepareNextMission(board, missions[missionNumber], replaySuit, numOfSteps, replay);
		}
	}
}
void Game::handleSideMenu(unsigned int missionNumber, bool &ExitGame, bool &replay, bool &suitCaseLoaded, Board &board, char &keyAfterEsc)
{
	TheSnakesGame game;
	char userSelection;

	printSideMenu();
	keyAfterEsc = userSelection = _getch();

	Sleep(200);

	Utilities().ClearScreen();
	switch (userSelection)
	{
	case ESCAPE:
		ExitGame = true;
		/*game.run();*/
		break;

	case MAIN_MENU:
		game.run(TheSnakesGame::EMPTY_CHOICE);
		ExitGame = true;
		break;

	case CONTINUE:
		missions[missionNumber]->printInstructions();
		board.printBoard();
		break;
		//return to the same same point

	case RESTART:
		unstuckSnakes();
		this->run(missionNumber);
		break;

	case NEW_MISSION:
		unstuckSnakes();
		this->run((missionNumber + 1) % 6);
		break;

	case NEW_GAME:
		unstuckSnakes();
		game.run(TheSnakesGame::RUN_GAME);
		break;
	case REPLAY_MISSION:
		replay = true;
		suitCaseLoaded = false;
		break;
	default:
		cout << "Incorrect input, Please enter again.";
		for (size_t i = 0; i < 3; i++)
		{
			cout << '.';
			Sleep(500);
		}
		handleSideMenu(missionNumber, ExitGame, replay, suitCaseLoaded, board, keyAfterEsc);
		break;
	}
}
void Game::incMissionNum(unsigned int& missionNumber)
{
	missionNumber = (missionNumber + 1) % missions.size();
}
void Game::ShowMessageEndOfGame()
{
	char dummy;
	Utilities::ClearScreen();
	gotoxy(BOARD_WIDTH / 2 - strlen("EndOfGame.wav") / 2, BOARD_HEIGHT / 2);//Go to The middle of the Screen
	cout << "GAME IS OVER\n";
	PlaySound(TEXT("EndOfGame.wav"), nullptr, SND_NODEFAULT);
	Utilities::ClearScreen();

	if (_kbhit())
		dummy = _getch();
}
void Game::handleEnd(bool ExitGame)
{
	TheSnakesGame temp;

	if (!ExitGame) //THE USER DOSN'T WANT TO EXIT
		temp.run(TheSnakesGame::EMPTY_CHOICE); // GO BACK TO MAIN MENU
	else
		temp.run(TheSnakesGame::EXIT);
};

//Replay SuitCase Handling

void Game::resetLists(replaySuitCase& replaySuit, bool &replay)
{
	replayPoint curPoint;
	replay = false;

	ResetList(replaySuit.keyboardHits, replaySuit.tempKeyboardHits);
	ResetList(replaySuit.numList, replaySuit.tempNumList);
	ResetList(replaySuit.freeLocations, replaySuit.tempFreeLocations);
}
void Game::ClearLists(replaySuitCase& replaySuit)
{
	replaySuit.keyboardHits.clear();
	replaySuit.tempKeyboardHits.clear();
	replaySuit.numList.clear();
	replaySuit.tempNumList.clear();
	replaySuit.freeLocations.clear();
	replaySuit.tempFreeLocations.clear();
}
void Game::LoadSuitCase(replaySuitCase& replaySuit, unsigned &missionNumber, Board &newBoard, bool &suitCaseLoaded, unsigned int &numOfSteps)
{
	if (!suitCaseLoaded)
	{
		missions[missionNumber]->printInstructions();
		newBoard = replaySuit.board;
		for (auto i = 0; i < Snake::NUM_OF_SNAKES; ++i)
			snakes[i] = replaySuit.snakes[i];
		snakes[0].isStuck = snakes[1].isStuck = false;
		for (auto i = 0; i < NUM_OF_MONSTERS; i++)
			monsters[i]->Activate();
		suitCaseLoaded = true;
		numOfSteps = 0;
	}
}
void Game::saveSuitCase(replaySuitCase& replaySuit, Board &board, unsigned int numOfSteps)
{
	if (numOfSteps == 0)	//This is a start of a mission
	{
		replaySuit.board = board;
		for (auto i = 0; i < Snake::NUM_OF_SNAKES; ++i)
			replaySuit.snakes[i] = snakes[i];
	}
}
void Game::GetKeyPressedFromList(replaySuitCase &replaySuit, char& keyPressed)
{
	keyPressed = replaySuit.keyboardHits.back();
	replaySuit.keyboardHits.pop_back();
	replaySuit.tempKeyboardHits.push_front(keyPressed);
}

// Monsters
void Game::setMonsters()
{
	monsters[NUMEATER] = new NumEater(1, 0, 10, 19, HOLD);
	monsters[ROW_CROSSING] = new RowMonster(1, 0, 30, 23, true, RIGHT);
	monsters[ROW_NOT_CROSSING] = new RowMonster(-1, 0, 50, 15, false, LEFT);
	monsters[COL_NOT_CROSSING] = new ColMonster(0, -1, 45, 23, false, UP);
	monsters[COL_CROSSING] = new ColMonster(0, 1, 55, 15, true, DOWN);
}
bool Game::ShouldMoveRight(const Point& NumEaterPos, const Point& closestSolution, unsigned &x_min_distance) const
{
	unsigned overBorder_x_length, inBorder_x_length;

	inBorder_x_length = abs(NumEaterPos.getX() - closestSolution.getX());
	overBorder_x_length = BOARD_WIDTH - inBorder_x_length;
	x_min_distance = min(overBorder_x_length, inBorder_x_length);

	if (NumEaterPos.getX() - closestSolution.getX() >= 0) //YOU ARE THE AT THE RIGHT FROM THE NUMBER
	{

		if (x_min_distance == inBorder_x_length)		  //The closest route is in borders
			return  false; //MOVE LEFT
		else
			return  true;  //MOVE RIGHT
	}
	else												  //YOU ARE THE AT THE LEFT FROM THE NUMBER
	{
		if (x_min_distance == inBorder_x_length)		  //The closest route is in borders
			return true;   //MOVE RIGHT
		else
			return false;  //MOVE LEFT
	}
}
bool Game::ShouldMoveDown(const Point& NumEaterPos, const Point& closestSolution, unsigned &y_min_distance) const
{
	unsigned  overBorder_y_length, inBorder_y_length;

	inBorder_y_length = abs(NumEaterPos.getY() - closestSolution.getY());
	overBorder_y_length = BOARD_HEIGHT - inBorder_y_length;
	y_min_distance = min(overBorder_y_length, inBorder_y_length);

	//Y
	if ((NumEaterPos.getY() - closestSolution.getY()) >= 0) //YOU ARE DOWN FROM THE NUMBER
	{

		if (y_min_distance == inBorder_y_length)		  //The closest route is in borders
			return  false;  //MOVE UP
		else
			return true;    //MOVE DOWN
	}
	else												  //YOU ARE UP FROM THE NUMBER
	{
		if (y_min_distance == inBorder_y_length)		  //The closest route is in borders
			return true;
		else
			return  false;
	}
}
unsigned Game::GetRouteLength(const Point& NumEaterPos, const Point& closestSolution, Directions &nextDir) const
{
	unsigned x_min_distance, y_min_distance;
	bool move_right, move_down;

	move_right = ShouldMoveRight(NumEaterPos, closestSolution, x_min_distance);
	move_down = ShouldMoveDown(NumEaterPos, closestSolution, y_min_distance);


	if (x_min_distance > 0)
	{
		if (move_right) nextDir = RIGHT;
		else nextDir = LEFT;
	}
	else //At X position - Need to Get closer at the Y Direction
	{
		if (move_down) nextDir = DOWN;
		else nextDir = UP;
	}
	return x_min_distance + y_min_distance;
}
Directions Game::GetDirToClosestSolution(Point* solutions[60], unsigned numOfSolutions) const
{
	Point NumEaterPos, closestSolution;
	Directions nextDir = HOLD;
	unsigned minRoute;

	if (numOfSolutions == 0)
		return nextDir;
	NumEaterPos = monsters[NUMEATER]->GetPoint();
	closestSolution = *solutions[0];
	minRoute = GetRouteLength(NumEaterPos, closestSolution, nextDir);
	for (size_t curSolution = 1; curSolution < numOfSolutions; curSolution++)
	{
		unsigned int curRoute;
		Directions TempDir;
		curRoute = GetRouteLength(NumEaterPos, *solutions[curSolution], TempDir);
		if (curRoute < minRoute)
		{
			minRoute = curRoute;
			nextDir = TempDir;
		}
	}
	return nextDir;
}
void Game::SetNumEaterDirection(unsigned missionNumber, Board &board)
{
	Directions nextDir;
	unsigned numOfSolutions;
	Point *solutions[60];

	numOfSolutions = missions[missionNumber]->GetSolutions(solutions, board);
	nextDir = GetDirToClosestSolution(solutions, numOfSolutions);
	monsters[NUMEATER]->SetDirection(nextDir);
}
bool Game::HandleMonsters(Board& newBoard, bool& gameOver, unsigned missionNumber)
{
	for (auto monsterIndex = 0; monsterIndex < NUM_OF_MONSTERS; monsterIndex++)
		if (monsters[monsterIndex]->GetActive())
		{
			bool isHit = false;
			for (size_t i = 0; i < monsters[monsterIndex]->GetSpeed(); i++)
			{
				Point tempP;
				if (monsterIndex == NUMEATER)
					SetNumEaterDirection(missionNumber, newBoard);
				tempP = monsters[monsterIndex]->GetPoint();
				if (handleMonsterHitSnake(gameOver, monsterIndex, tempP, isHit))
					return true; //return true if the mission ends.
				monsters[monsterIndex]->move();
				if (CheckMonsterHit(newBoard, gameOver, monsterIndex))
					return true; // return true if the mission ends.
			}
		}
	return false;
}
bool Game::handleMonsterHitSnake(bool& gameOver, unsigned monsterIndex, Point monsterPoint, bool& isHit)
{
	string numEaterStr = "Eaten by the NumEater - Lose!!!\n";

	for (size_t snakeNum = 0; !isHit && snakeNum < Snake::NUM_OF_SNAKES; snakeNum++)
		for (size_t i = 0; !isHit && i < snakes[snakeNum].GetSnakeSize(); i++)
			if (monsterPoint == Point{ snakes[snakeNum].getBodyPart(i).getX(),snakes[snakeNum].getBodyPart(i).getY() })
			{
				size_t otherSnakeNum = (snakeNum + 1) % 2;
				snakes[snakeNum].hideSnake();
				isHit = true;
				if (typeid(*monsters[monsterIndex]) == typeid(NumEater))
				{
					if ((snakes[otherSnakeNum].incrementSnake()) == false)
						gameOver = true;
					Mission::printStat(numEaterStr);
					return true; //OTHER SNAKE WON THE MISSION
				}
			}
	return false;
}
void Game::handleMonsterWithNumber(Board& board, Point monsterPoint, bool& isHit)
{
	if (!isHit && board.board[monsterPoint.getY()][monsterPoint.getX()].checkChar())
	{
		board.removeNumber(monsterPoint, board);
		isHit = true;
	}
}
void Game::handleMonsterWithShots(unsigned monsterIndex, Point monsterPoint, bool& isHit)
{
	if (!isHit)
	{
		for (size_t snakeNum = 0; !isHit && snakeNum < Snake::NUM_OF_SNAKES; snakeNum++)
			if (snakes[snakeNum].CmpShotsToMonster(snakes, monsterPoint, isHit))
				if (typeid(*monsters[monsterIndex]) != typeid(RowMonster))
				{
					monsters[monsterIndex]->InActive(); // active false && draw space 
					snakes[snakeNum].incNumOfShots();
				}
	}
}
void Game::handleMonstersInteraction(unsigned monsterIndex, bool isHit)
{
	for (auto i = 0; !isHit && i < NUM_OF_MONSTERS; i++)
	{
		if (monsterIndex != i && monsters[monsterIndex]->GetPoint() == monsters[i]->GetPoint() && typeid(*monsters[monsterIndex]) != typeid(*monsters[i]))
		{
			if (typeid(*monsters[monsterIndex]) == typeid(RowMonster))
				monsters[i]->InActive(); //  rowMonster eat all
			else if (typeid(*monsters[monsterIndex]) != typeid(RowMonster) && typeid(*monsters[i]) != typeid(RowMonster))
				monsters[i]->InActive(); // col and numEater eat eachOther
			isHit = true;
		}
	}
}
bool Game::CheckMonsterHit(Board& board, bool& gameOver, unsigned monsterIndex)
{
	Point monsterPoint = { monsters[monsterIndex]->getX(), monsters[monsterIndex]->getY() };
	bool isHit = false;

	// Snake
	if (handleMonsterHitSnake(gameOver, monsterIndex, monsterPoint, isHit))
		return true; //mission end
	// Board
	handleMonsterWithNumber(board, monsterPoint, isHit);
	// Shots 
	handleMonsterWithShots(monsterIndex, monsterPoint, isHit);
	// Monsters Interaction
	handleMonstersInteraction(monsterIndex, isHit);

	return false;
}

// Print
void Game::printSideMenu()
{
	Utilities().ClearScreen();

	int x_printing_location = (SCREEN_WIDTH / 2 - 1) - (strlen("< 1 > Exit\n") / 2);
	int y_printing_location = SCREEN_HEIGHT / 2 - 4;

	Utilities::setColor(LIGHT_BLUE);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_BLUE); //FUNCTION OF COLOR
	gotoxy(x_printing_location, y_printing_location);
	cout << "< 1 > Exit Game\n";
	gotoxy(x_printing_location, y_printing_location + 1);
	cout << "< 2 > Main Menu\n";
	gotoxy(x_printing_location, y_printing_location + 2);
	cout << "< 3 > Back To Game\n";
	gotoxy(x_printing_location, y_printing_location + 3);
	cout << "< 4 > Restart Mission\n";
	gotoxy(x_printing_location, y_printing_location + 4);
	cout << "< 5 > Start New Mission\n";
	gotoxy(x_printing_location, y_printing_location + 5);
	cout << "< 6 > Start new Game\n";
	gotoxy(x_printing_location, y_printing_location + 6);
	cout << "< 7 > Replay Mission\n";
	Utilities::resetColor();
}

//Help Functions
void Game::getInput(char& keyPressed, unsigned int missionNumber, bool &ExitGame, bool &replay, bool &suitCaseLoaded, Board &board, char &keyAfterEsc)
{
	if (_kbhit())
	{
		keyPressed = _getch();
		if (keyPressed == ESC)
			handleSideMenu(missionNumber, ExitGame, replay, suitCaseLoaded, board, keyAfterEsc);
		else
			reguliseInput(keyPressed, missionNumber);
	}
	else
		keyPressed = 0;
}