#include "RowMonster.h"
#include "Utilities.h"

void RowMonster::move()
{
	draw(' ');
	if (!GetCanCross())
	{
		if (GetDirection() == RIGHT)
		{
			if (getX() == BOARD_WIDTH - 1)
				SetDirection(LEFT);
		}
		else if (getX() == 0)
			SetDirection(RIGHT); //DIR == LEFT
	}
	movePoint(GetDirection());
	Utilities::setColor(color);
	draw(ch);
	Utilities::resetColor();
}
void RowMonster::Activate()
{
	Monster::Activate();
	if (GetCanCross())
		setPoint(30, 23);
	else
		setPoint(50, 15);
}
