#ifndef _Mission_h
#define _Mission_h
#include "Game.h"
#include "Board.h"
#include <windows.h>


enum missionsType { Prime, DivisionInX, Multiplication, SquareRoot, DivisionWithReminder, squareOfX, EquationMission , numUnderX, numBetweenXY, numFollowingX};
#define NUM_OF_NUMBERS 169

//Forward Decleration
class Game;
class Mission
{
	string Instructions; // every mission save her instruction
	unsigned int MissionNum, x1, x2;
	class EquationMission
	{
		enum OPERATORS { addition, subtraction, multiplication, division, NUM_OF_OPERATORS };
		enum NUM_LOCATIONS { num1_loc = 1, num2_loc, num3_loc, num4_loc, NUM_OF_NUM_LOCATIONS = 4 };
		char op1, op2;
		unsigned int num1, num2, num3, num4;
		unsigned int missing_num_location;
	public:
		friend class Mission;//So Mission class can access to this class
	};
	EquationMission *eqObj = nullptr;
public:
	// CTOR/DTOR
	Mission() :Instructions(nullptr) {} //EMPTY CTOR
	Mission(string str, unsigned _MissionNum = 0) :Instructions(str), MissionNum(_MissionNum) {} //CTOR
	Mission(const string& instructions, unsigned mission_num, unsigned x1, unsigned x2)			//CTOR2
		: Instructions(instructions), MissionNum(mission_num), x1(x1), x2(x2) {}

	~Mission() { if (eqObj != nullptr) delete eqObj; }
	// Functions
	unsigned int getNumber(const Point& p, const Board& boardObj);
	bool checkWin(unsigned num, missionsType missionType);
	void handleEat(const Point& p, Snake snakes[], int i, Board& boardObj, missionsType msnType, bool& gameOver);
	unsigned GetSolutions(Point* solutions[], Board& board);
	void checkEndOfStage(Board& board);
	int calc(char op, unsigned leftNum, unsigned rightNum, bool &reminder);
	bool isLegalNumber(unsigned& res, unsigned resNum, unsigned num, char op, bool mode);
	unsigned getValidNumber(unsigned& res, unsigned num, char op, bool mode);
	void SetEquation();
	void CreateEquationMission();
	string CreateEqMissionStr(unsigned missing_num);

	// Print
	void printInstructions() const;
	static void newMissionMessage();
	static void printStat(string stat);
	// Getters/Setters
	unsigned int GetMissionNum() const { return MissionNum; }
	void setMissionNum(unsigned int missionNumber) { MissionNum = missionNumber; }
	void SetInstructions(string str) { Instructions = str; };

	// Missions
	bool Prime(unsigned num);
	bool DivisionInX(unsigned num);
	bool Multiplication(unsigned num);
	bool SquareRoot(unsigned num);
	bool DivisionWithReminder(unsigned num);
	bool squareOfX(unsigned num);
	bool Equation(unsigned num);
	bool numUnderX(unsigned num);
	bool numBetweenXY(unsigned num);
	bool numFollowingX(unsigned num);
};

#endif
