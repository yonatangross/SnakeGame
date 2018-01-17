#include "Shot.h"
#include "RowMonster.h"

void Shot::move(Point headP)
{
	if (position != headP)
		position.draw(' ');

	position.movePoint(dirShot);
	position.draw(shotChar);
}

bool Shot::HandleHit(vector<Shot>& shots, Board& board, Snake snakes[Snake::NUM_OF_SNAKES], unsigned int& numOfShots, int snakeColor,Monster *monsters[NUM_OF_MONSTERS])
{//return true iff this shot has to been deleted

	bool isHit = false;

	// Snake
	for (size_t snakeNum = 0; !isHit && snakeNum < Snake::NUM_OF_SNAKES; snakeNum++)
		for (size_t i = 0; !isHit && i < snakes[snakeNum].GetSnakeSize(); i++)
			if (position.getX() == snakes[snakeNum].getBodyPart(i).getX() && position.getY() == snakes[snakeNum].getBodyPart(i).getY())
			{
				snakes[snakeNum].hideSnake();
				isHit = true;
				if (snakes[snakeNum].getColor() != snakeColor)
					numOfShots++;
			}

	// BOARD
	if (!isHit && board.board[position.getY()][position.getX()].checkChar())
	{
		board.removeNumber(position, board);
		isHit = true;
	}
	//Monsters
	for (size_t monsterIndex = 0; !isHit && monsterIndex < NUM_OF_MONSTERS; monsterIndex++)
		if (*(monsters[monsterIndex]) == position)
		{
			monsters[monsterIndex]->InActive(numOfShots); //ROW MONSTER DONT INACTIVATE HIMSELF!!
			isHit = true;
		}
	
	//TODO: Shots interaction - if we have time.
	return isHit;
}

bool Shot::cmpToPoint(Point otherPoint) const
{
	if (position.getX() == otherPoint.getX() && position.getY() == otherPoint.getY())
		return true;
	return false;
}
