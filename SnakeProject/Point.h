#ifndef _POINT_H_
#define _POINT_H_
#include <Windows.h>
#include <iostream>
#include "Gotoxy.h"

using namespace std;
enum Directions { LEFT, RIGHT, UP, DOWN, HOLD };

class Point
{
	int x;
	int y;
	int dir_x = 1;
	int dir_y = 0;

public:
	char ch = ' ';

	// CTOR/DTOR
	Point(int _x = 0, int _y = 0)
	{
		x = _x;
		y = _y;
	}
	Point(int _dir_x, int _dir_y, char _ch, int _x = 0, int _y = 0)
	{
		x = _x;
		y = _y;
		dir_x = _dir_x;
		dir_y = _dir_y;
		ch = _ch;
	}
	Point(const Point& other) : x(other.x), y(other.y), dir_x(other.dir_x), dir_y(other.dir_y), ch(other.ch) {}
	// Functions
	void changeDir(Directions dir);
	void movePoint(Directions dir);
	void moveImpl();
	bool operator== (const Point& other)const { return x == other.x && y == other.y; }
	bool operator!= (const Point& other)const { return !(*this == other); }
	bool checkChar();
	// Print
	void draw(char c) const;

	// Getters/Setters
	int getX() const { return x; }
	int getY() const { return y; }
	int getDirX() const { return dir_x; }
	int getDirY() const { return dir_y; }
	void setPoint(int _x, int _y) { x = _x; y = _y; }
	Point GetPoint() const
	{
		Point p(dir_x,dir_y,ch,x,y);
		return p;
	}
	void setX(int _x) { x = _x; }
	void setY(int _y) { y = _y; }
};

#endif
