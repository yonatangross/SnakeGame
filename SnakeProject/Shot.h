#ifndef _SHOT_H_
#define _SHOT_H_
#include <vector>
#include "Board.h"
#include "Monster.h"
class Snake;
class Board;
class Shot
{
	enum { SIZE = 1 ,NUM_OF_MONSTERS = 5};
	Point position;
	Directions dirShot{};
	char shotChar = '*';
public:
	void move(Point headP);
	Shot(Point p, Directions d) :position(p), dirShot(d) {}//CTOR
	bool HandleHit(vector<Shot>& shots, Board& board, Snake snakes[], unsigned int& numOfShots, int snakeColor, Monster *monsters[NUM_OF_MONSTERS]);
	bool cmpToPoint(Point otherPoint) const;
	Point GetPosition()const { return position; }
};

#endif
