#ifndef _Board_h
#define _Board_h
#include "Point.h" 
#include "Snake.h"
#include "Utilities.h"
#include <list>
#include "replayPoint.h"

enum screenSize { SCREEN_HEIGHT = 24, SCREEN_WIDTH = 80 };
enum boardSize { BOARD_HEIGHT = 21, BOARD_WIDTH = 80 ,BOARD_START_LINE = 3,MAX_NUMBERS_ONBOARD = 60};
class Snake;
class Board
{
	unsigned int numOfNumbers = 0;
public:
	Point board[SCREEN_HEIGHT][SCREEN_WIDTH];

	// CTOR/DTOR
	Board(); // CTOR
	void removeNumber(const Point& point,Board& board);
	// Functions
	static unsigned int generateNum();
	static bool checkSnakesClearance(const Snake* snake, const Point& point, unsigned numOfDigits);
	static bool checkNumbersClearance(Point board[SCREEN_HEIGHT][SCREEN_WIDTH], const Point& point, unsigned numOfDigits);
	static bool checkInBorders(int x, unsigned int  numOfDigits);
	static bool checkMonstersClearance(const Point& numLocation, unsigned numOfDigits, Point* monstersPoints[5]);
	static bool checkNumLocation(unsigned int num, const Snake* snakes, Point board[SCREEN_HEIGHT][SCREEN_WIDTH], Point curPoint, Point *monstersPoints[5]);
	Point generateLocation() const;
	void addNumToBoard(const Snake snakes[], bool replay, list<replayPoint>& numList, list<replayPoint>& numListCopy, Point* monstersPoints[5]);
	void clearHalfBoard();

	// Print
	void printBoard() const;
	void drawNumberFromArr(Point* solutions[60], unsigned int numOfSolutions) const;

	// Getters/Setters
	Point getPoint(Point p) { return board[p.getY()][p.getX()]; }
	unsigned int getNumOfNumbers() const { return numOfNumbers; }
	void setNumOfNumbers(unsigned int num) { numOfNumbers = num; }
};

#endif
