#pragma once
#include "Monster.h"

class ColMonster :public Monster
{
	unsigned color;
public:
	ColMonster(int _dir_x, int _dir_y, int _x, int _y, bool _can_cross, Directions _direction,
		char _ch = '$', unsigned _speed = 1, bool _active = true, unsigned _color = RED) : Monster(_dir_x, _dir_y, _ch, _x, _y, _speed, _active, _can_cross, _direction),color(_color) {}

	virtual ~ColMonster() override { }
	virtual void move() override;
	virtual void Activate() override;
	virtual void InActive() override;
	virtual void InActive(unsigned &numOfShots) override;

};

