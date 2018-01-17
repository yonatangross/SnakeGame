#include "Point.h"
#include "Board.h"


// Functions
void Point::moveImpl()
{
	x += dir_x;
	y += dir_y;
	if (x == SCREEN_WIDTH && dir_x == 1) // direction is right + position at the End of the screen
		x = 0;							 //return x to start position
	else if (x == -1 && dir_x == -1)	 // direction is left + position at the left End of the screen				
		x = SCREEN_WIDTH - 1;			 //move x to the right End of the screen 

	if (y == SCREEN_HEIGHT && dir_y == 1) // direction is down + position at the End of the screen
		y = 3;							  // move y to line number 4 (from top)
	else if (y == 2 && dir_y == -1)		  // direction is up + position at line number 4 (top edge)
		y = SCREEN_HEIGHT - 1;			  // move y to the down End of the screen 
}
void Point::changeDir(Directions dir) {
	switch (dir) {
	case LEFT:
		dir_x = -1;
		dir_y = 0;
		break;
	case RIGHT:
		dir_x = 1;
		dir_y = 0;
		break;
	case UP:
		dir_x = 0;
		dir_y = -1;
		break;
	case DOWN:
		dir_x = 0;
		dir_y = 1;
		break;
	case HOLD:
		dir_x = 0;
		dir_y = 0;
		break;
	default: break;
	}
}
void Point::movePoint(Directions dir) {

	changeDir(dir);
	moveImpl();
}
bool Point::checkChar()
{
	if (this->ch != ' ' && this->ch != '_')
		return true;
	return false;
}

// Print
void Point::draw(char c) const
{
	gotoxy(x, y);
	if(y<SCREEN_HEIGHT)
		cout << c << endl;
}
