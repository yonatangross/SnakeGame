#pragma once
#include "Monster.h"
class NumEater :public Monster
{
	unsigned color;
public:
		NumEater(int _dir_x, int _dir_y, int _x, int _y, Directions _direction,
		char _ch = '%', unsigned _speed = 2, bool _active = true,
		bool _can_cross = true,unsigned _color = YELLOW) :
		Monster(_dir_x, _dir_y, _ch, _x, _y, _speed, _active, _can_cross, _direction),color(_color) {}
	virtual void move() override;
	virtual void Activate() override;
	virtual void InActive() override;
	virtual void InActive(unsigned &numOfShots) override;
	virtual ~NumEater() override {}

};
