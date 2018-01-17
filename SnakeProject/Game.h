#ifndef _Game_h
#define _Game_h
#include "TheSnakesGame.h"
#include "Snake.h"
#include "Mission.h"
#include "NumEater.h"
#include "RowMonster.h"
#include "ColMonster.h"
#include <list>

using namespace std;

class Mission;//Forward Decleration
class Board;  //Forward Decleration
class Game
{
public:
	enum { NUM_OF_MISSIONS = 7};
	enum KEYS { ESC = 27 };
	enum SIDE_MENU { ESCAPE = '1', MAIN_MENU = '2', CONTINUE = '3', RESTART = '4', NEW_MISSION = '5', NEW_GAME = '6', REPLAY_MISSION = '7' };
	enum MONSTERS_INDEX { NUMEATER, ROW_CROSSING, ROW_NOT_CROSSING, COL_CROSSING, COL_NOT_CROSSING, NUM_OF_MONSTERS };
	Snake snakes[Snake::NUM_OF_SNAKES] = {
		{{10,9},'@',BLUE},
		{{70,9},'#',GREEN }
	};
	Monster *monsters[NUM_OF_MONSTERS];
	vector<Mission*> missions;
	struct replaySuitCase
	{
		Snake snakes[Snake::NUM_OF_SNAKES];
		Board board;
		list<char> keyboardHits;
		list<char> tempKeyboardHits;
		list<replayPoint> numList;
		list<replayPoint> tempNumList;
		list<Point> freeLocations;
		list<Point> tempFreeLocations;
		//TODO: include the equation mission.
	};
	string strings[NUM_OF_MISSIONS]{
		"Pick a Prime number",
		"Collect a number that is a multiple of 4",
		"Pick a number that 7 is a root of him",
		"Pick a number that has a square root.",
		"Pick a number that divides by 7 with 3 reminder",
		"Pick 13 square."
	};
	// CTOR/DTOR
	Game()
	{			//CTOR
		snakes[0].setKeys('a', 'd', 'w', 'x', 'z');
		snakes[1].setKeys('j', 'l', 'i', 'm', 'n');
	}
	~Game();	//DESTRUCTOR(RELESE ALL MISSIONS)


	template<class LIST>
	void ResetList (LIST &list, LIST &tempList)
	{
		while (!list.empty())
		{
			tempList.push_front(list.back());
			list.pop_back();
		}
		tempList.swap(tempList);
	}

	void resetLists(replaySuitCase& replaySuit, bool& replay);
	void LoadSuitCase(replaySuitCase& replaySuit, unsigned& missionNumber, Board& newBoard, bool& suitCaseLoaded, unsigned& numOfSteps);
	void saveSuitCase(replaySuitCase& replaySuit, Board& board, unsigned numOfSteps);
	// Functions
	void run(unsigned int missionNumber = 0, unsigned difficulty = 2);
	void reguliseInput(char& key_pressed, unsigned int missionNumber);
	bool HandleMission(unsigned &missionNumber, bool &gameOver, Board& newBoard, replaySuitCase& replaySuit, unsigned &numOfSteps,bool replay);
	bool ExecuteStep(unsigned& missionNumber, Board& newBoard, unsigned numOfSteps, bool& gameOver, char keyPressed, replaySuitCase& replaySuit, bool replay);
	void GetKeyPressedFromList(replaySuitCase &replaySuit, char& keyPressed);
	void getInput(char& keyPressed, unsigned missionNumber, bool& ExitGame, bool& replay, bool& suitCaseLoaded, Board& board, char &keyAfterEsc);
	string GetFileName(unsigned difficulty);
	void InterpretToken(const string& token, unsigned counter, string& mission_name, string& basic_string, string& num1_str, string& num2_str);
	Mission* MakeMissionFromParamaters(const string& missionName, const string& missionDescription, int num1, int num2);
	Mission *GetMission(string missionFormat);
	void setMissions(unsigned difficulty);
	static void ClearLists(replaySuitCase& replaySuit);
	void prepareNextMission(Board& board, Mission* cur_mission, replaySuitCase& replaySuit, unsigned& numOfSteps, bool replay);
	void unstuckSnakes();
	Point findFreeLocation(const Board& board);
	void repositionSnake(int snakeNumber, Board& board, replaySuitCase& replaySuit, bool replay);
	void Game::handleMovement(unsigned& numOfSteps, Board& board, unsigned& missionNumber, bool gameOver, bool replay, replaySuitCase& replaySuit);
	void handleSideMenu(unsigned missionNumber, bool& ExitGame, bool& replay, bool& suitCaseLoaded, Board& board,char &keyAfterEsc);
	void handleEnd(bool ExitGame);
	void incMissionNum(unsigned int &missionNumber);
	void ShowMessageEndOfGame();
	
	// Monsters
	void setMonsters();
	bool ShouldMoveRight(const Point& NumEaterPos, const Point& closestSolution, unsigned& x_min_distance) const;
	bool ShouldMoveDown(const Point& NumEaterPos, const Point& closestSolution, unsigned& y_min_distance) const;
	unsigned GetRouteLength(const Point& NumEaterPos, const Point& closestSolution, Directions& nextDir) const;
	Directions GetDirToClosestSolution(Point* solutions[], unsigned numOfSolutions) const;
	void SetNumEaterDirection(unsigned missionNumber, Board& board);
	bool HandleMonsters(Board& newBoard, bool& gameOver, unsigned missionNumber);
	bool handleMonsterHitSnake(bool& gameOver,unsigned monsterIndex, Point monsterPoint, bool& isHit);
	void handleMonsterWithNumber(Board& board, Point monsterPoint, bool& isHit);
	void handleMonsterWithShots(unsigned monsterIndex, Point monsterPoint, bool& isHit);
	void handleMonstersInteraction(unsigned monsterIndex, bool isHit);
	bool CheckMonsterHit(Board& board, bool& gameOver, unsigned monsterIndex);
	// Print
	static void printSideMenu();
};

#endif
