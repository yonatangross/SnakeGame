#ifndef _SNAKE_H_
#define _SNAKE_H_
#include <Windows.h>
#include "vector"
#include "Shot.h"
using namespace std;

//Forward decleration
class Monster;
class Board;
class Shot;

class Snake {
public:
	enum SNAKE_SIZES{ NUM_OF_SNAKES = 2, PH_SIZE = 12 };
	bool isStuck = false;
	bool active = true;
	// CTOR/DTOR
	Snake() = default;
	Snake(const Point& head, char c1, int _color, Directions dir = UP);
	Snake& operator=(const Snake& other);

	// Functions
	void changeDir(char keyPressed);
	void move(char keyPressed, bool isStuck);
	bool incrementSnake();
	bool isDirection(char key_pressed);
	Directions calcDir(char key_pressed);
	void checkIfStuck(Snake otherSnake, char keyPressed);
	void eraseShot(size_t shotIndex) { active_shots.erase(active_shots.begin() + shotIndex); }
	static Directions blockedDir(Directions dir);

	void Shoot();
	// Getters/Setters
	unsigned int GetSnakeSize() const { return log_size; }
	Directions getDir() const { return direction; }
	int getColor() const { return color; }
	int getSign() const { return c; }
	Point getHead() const { return body[0]; }
	Point getBodyPart(int i) const { return body[i]; }
	Point getNextHeadPos(const Point& point, Directions nextDir) const;
	unsigned int getNumOfShots() const { return numOfShots; }
	void setKeys(char keyLeft, char keyRight, char keyUp, char keyDown, char shootKey);
	void setBodyPartLocation(int i, int x, int y);
	void setDir(Directions newDir) { direction = newDir; }
	void setNumOfShots(unsigned int _NumOfShots) { numOfShots = _NumOfShots; }
	void incNumOfShots() { numOfShots++; }
	void PrintStats(Snake snakes[NUM_OF_SNAKES]);
	void handleShots(Board& board, Snake snakes[], char keyPressed, Monster *monsters[5]);
	bool HandleActivation();
	void deleteSnakeFromScreen();
	void hideSnake();
	void resetShots();
	bool CmpShotsToMonster(Snake snakes[NUM_OF_SNAKES], Point monsterPoint, bool& isHit);
private:
	enum { STARTING_NUM_OF_SHOTS = 5, NUM_OF_DIRS = 4 };
	unsigned int log_size = 3; // Snake Length
	Point body[PH_SIZE];
	Directions direction;
	Directions blockedDirection;
	char c;
	char dirKeys[NUM_OF_DIRS];	// the keys for the four possible directions
	char shoot_key = 0;
	int color;
	vector<Shot> active_shots;
	unsigned int numOfShots = STARTING_NUM_OF_SHOTS;
	unsigned int inactiveCounter = 0;
};

#endif
