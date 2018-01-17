#include "ColMonster.h"

void ColMonster::move()
{
	draw(' ');
	if (!GetCanCross())
	{
		if (GetDirection() == UP)
		{
			if (getY() == BOARD_START_LINE) SetDirection(DOWN);
		}
		else if (getY() == SCREEN_HEIGHT -1) SetDirection(UP); //DIR == DOWN
	}
	movePoint(GetDirection());
	Utilities::setColor(color);
	draw(ch);
	Utilities::resetColor();
}

void ColMonster::Activate()
{
	Monster::Activate();
	if (GetCanCross())
		setPoint(55, 15);
	else
		setPoint(45, 23);
}

void ColMonster::InActive()
{
	SetActive(false);
	draw(' ');
	setPoint(100, 100);
}

void ColMonster::InActive(unsigned &numOfShots)
{
	InActive();
	numOfShots++;
}
