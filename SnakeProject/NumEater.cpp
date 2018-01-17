#include "NumEater.h"

void NumEater::move()
{
	draw(' ');
	//TODO: NumEater Algorithem.
	movePoint(GetDirection());
	Utilities::setColor(color);
	draw(ch);
	Utilities::resetColor();
}
void NumEater::Activate()
{
	Monster::Activate();
	setPoint(10, 19);
}
void NumEater::InActive()
{
	SetActive(false);
	draw(' ');
	setPoint(100, 100);
}
void NumEater::InActive(unsigned &numOfShots)
{
	InActive();
	numOfShots++;
}