#pragma once
#include "Point.h"
class replayPoint :	public Point
{
	unsigned int num;
public:
	replayPoint() { Point(0, 0); num = 0; }
	replayPoint(int _x,int _y,unsigned int _num):Point(_x,_y),num(_num){}
	replayPoint(const replayPoint& other): Point(other), num(other.num){}

	unsigned int getNum() const { return num; }
};

