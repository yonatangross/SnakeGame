#pragma once
#include "Monster.h"

class RowMonster :public Monster
{
	unsigned color;
public:
	virtual ~RowMonster() override {}
	RowMonster(int _dir_x, int _dir_y, int _x, int _y, bool _can_cross,Directions _direction ,
			   char _ch = '!', unsigned _speed = 2, bool _active = true,unsigned _color = RED) : Monster(_dir_x,_dir_y, _ch,_x,_y,_speed,_active,_can_cross,_direction),color(RED){}
	virtual void move() override;
	virtual void Activate() override;
	virtual void InActive(unsigned &numOfShots) override {};
	virtual void InActive() override{};
};
