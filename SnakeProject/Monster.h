#pragma once
#include "Point.h"
#include "Utilities.h"

class Monster : public Point {
	unsigned int speed;
	bool active;
	bool canCross;
	Directions direction;
public:
	enum boardSize { BOARD_HEIGHT = 21, BOARD_WIDTH = 80, BOARD_START_LINE = 3, MAX_NUMBERS_ONBOARD = 60 };
	enum screenSize { SCREEN_HEIGHT = 24, SCREEN_WIDTH = 80 };

	Monster(int _dir_x, int _dir_y, char _ch, int _x, int _y, unsigned speed, bool active,
	bool can_cross, Directions _direction) : Point(_dir_x, _dir_y, _ch, _x, _y),
		speed(speed), active(active), canCross(can_cross), direction(_direction) {}
	Monster(const Monster& other) : Point(other), 
	                                speed(other.speed),
	                                active(other.active),
	                                canCross(other.canCross),
	                                direction(other.direction)
	{}
	virtual ~Monster() {}
	virtual void move() = 0;
	virtual void InActive(unsigned &numOfShots) = 0;
	virtual void InActive() = 0;
	virtual void Activate() { active = true; }
	// GETTERS /SETTERS
	unsigned GetSpeed() const { return speed; }
	void SetSpeed(unsigned speed) { this->speed = speed; }
	bool GetActive() const { return active; }
	void SetActive(bool active) { this->active = active; }
	bool GetCanCross() const { return canCross; }
	void SetCanCross(bool can_cross) { canCross = can_cross; }
	Directions GetDirection() const { return direction; }
	void SetDirection(Directions newDir) { direction = newDir; }
};

