#include "Board.h"
#include "string"
#include "Mission.h"
#include "replayPoint.h"

// CTOR/DTOR
Board::Board()
{
	for (unsigned i = BOARD_START_LINE; i < SCREEN_HEIGHT; i++)
		for (unsigned j = 0; j < SCREEN_WIDTH; j++)
		{
			board[i][j].setPoint(j, i);
			board[i][j].draw(board[i][j].ch);
		}
}

// Functions
void Board::clearHalfBoard()
{
	unsigned int halfNumOfNumbers;
	size_t ind;
	Point *showing[MAX_NUMBERS_ONBOARD + 1];
	unsigned int counter = 0;
	for (size_t i = 3; i < SCREEN_HEIGHT; i++)
		for (size_t j = 0; j < SCREEN_WIDTH; j++)
			if (board[i][j].ch != ' ')
			{
				showing[counter++] = &(board[i][j]);			  //counter = num of numbers on board
				while (board[i][j].ch != ' ' && j < SCREEN_WIDTH) //PASS ALL THE NUMBER
					j++;
			}
	numOfNumbers = counter;
	halfNumOfNumbers = numOfNumbers / 2;

	while (halfNumOfNumbers < numOfNumbers)
	{
		ind = rand() % counter;
		unsigned int x = showing[ind]->getX(), y = showing[ind]->getY(); //locals
		if (board[y][x].ch != ' ')
		{
			while (board[y][x].ch != ' ' && x < SCREEN_WIDTH)
			{
				board[y][x].ch = ' ';
				board[y][x].draw(' ');
				x++;
			}
			numOfNumbers--; //WE DELETED A NUMBER
		}
	}
}
void Board::removeNumber(const Point& point, Board& board)
{
	int x = point.getX(), y = point.getY(); //locals

	//Need to go to the left edge - Not Crossing the borders of the screen
	while (board.board[y][x].ch != ' ' && x > 0)
		x--;
	//read left to right = num untill whiteSpace
	x++;
	while (board.board[y][x].ch != ' ' && x < SCREEN_WIDTH)
	{
		board.board[y][x].ch = ' ';
		board.board[y][x].draw(' ');
		x++;
	}
	board.setNumOfNumbers(numOfNumbers - 1);
}
unsigned int Board::generateNum()
{
	unsigned int num = rand();
	unsigned int probality = rand() % 3; //same probality for all the ranges

	switch (probality)
	{
	case 0:	num %= 26;	break;			 // 0-25 
	case 1: num %= 56; num += 26; break; //26-81
	case 2:	num %= 88; num += 82; break; //82-169
	default: break;
	}
	return  num;
}
bool Board::checkInBorders(int x, unsigned int  numOfDigits)
{
	if (x + numOfDigits >= BOARD_WIDTH)
		return false;
	return true;
}
bool Board::checkSnakesClearance(const Snake* snake, const Point& point, unsigned numOfDigits)
{
	bool isValid = true;
	int pX = point.getX();
	int pY = point.getY(); // locals

	for (unsigned i = 0; i < Snake::NUM_OF_SNAKES; i++) // num Snakes
	{
		int headpX = snake[i].getHead().getX();
		int headpY = snake[i].getHead().getY(); // locals 
		for (unsigned j = 0; j < numOfDigits; j++) // num Digits
		{
			if (pX + j == headpX && pY + j == headpY)	   // if at the head position
				isValid = false;
			if (pX + j == headpX - 1 && pY + j == headpY) // if at the head position left
				isValid = false;
			if (pX + j == headpX + 1 && pY + j == headpY) // if at the head position right
				isValid = false;
			if (pX + j == headpX && pY + j == headpY - 1) // if at the head position up
				isValid = false;
			if (pX + j == headpX && pY + j == headpY + 1) // if at the head position down
				isValid = false;
			for (unsigned int k = 0; k < snake[i].GetSnakeSize(); k++)
				if (pX + j == snake[i].getBodyPart(k).getX() && pY + j == snake[i].getBodyPart(k).getY())// if at the Body Position position
					isValid = false;
		}
	}
	return isValid;
}
bool Board::checkNumbersClearance(Point board[SCREEN_HEIGHT][SCREEN_WIDTH], const Point& point, unsigned numOfDigits)
{
	int pX = point.getX(), pY = point.getY();
	if ((pX == pY) && pX == 0 && board[0][0].ch != ' ') //checking the exact point
		return  false;
	for (unsigned j = 0; j < numOfDigits; j++) //num Digits
	{
		if (board[pY][pX + j].ch != ' ')	// exact
			return  false;
		if (board[pY][(pX + j - 1) % SCREEN_WIDTH].ch != ' ') // left
			return  false;
		if (board[pY][pX + j + 1].ch != ' ') // right
			return  false;

		if (pY - 1 < BOARD_START_LINE || pY + 1 > SCREEN_HEIGHT - 1);
		else
		{
			if (board[pY + 1][pX + j].ch != ' ') // down
				return false;
			if (board[(pY - 1)][pX + j].ch != ' ') // up 
				return false;
		}
	}
	return true;
}
bool Board::checkMonstersClearance(const Point& numLocation, unsigned numOfDigits, Point *monstersPoints[5])
{
	int X = numLocation.getX(), Y = numLocation.getY();
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < int(numOfDigits); j++) //num Digits
		{
			if (Point{ X + j ,Y} == monstersPoints[i]->GetPoint())	// exact
				return  false;
		}
		
	return true;
}
bool Board::checkNumLocation(unsigned int num, const Snake* snakes, Point board[SCREEN_HEIGHT][SCREEN_WIDTH], Point curPoint,Point *monstersPoints[5])
{
	unsigned int numOfDigits;
	numOfDigits = Utilities::CalculateNumDigits(num);

	//check if the number is not flowing to the edges
	if (!checkInBorders(curPoint.getX(), numOfDigits))
		return false;
	//check if the number is not near the snake 
	if (!checkSnakesClearance(snakes, curPoint, numOfDigits))
		return false;
	//check if the number is not adjacent to a another number
	if (!checkNumbersClearance(board, curPoint, numOfDigits))
		return false;
	//check if the number is not adjacent to a monster
	if (!checkMonstersClearance(curPoint, numOfDigits,monstersPoints))
		return false;
	return true; //LOCATION IS FINE
}
Point Board::generateLocation() const
{
	int x, y;
	Point curPoint;

	x = rand() % (BOARD_WIDTH);
	y = (rand() % (BOARD_HEIGHT)) + BOARD_START_LINE;

	while (board[y][x].ch != ' ') // If location is not free try again
	{  // Generate random x and y values within the map
		x = rand() % (BOARD_WIDTH);
		y = (rand() % (BOARD_HEIGHT)) + BOARD_START_LINE;
	}

	curPoint.setPoint(x, y);
	return curPoint;
}
void Board::addNumToBoard(const Snake snakes[Snake::NUM_OF_SNAKES], bool replay, list<replayPoint> &numList, list<replayPoint> &numListCopy,Point *monstersPoints[5])
{
	unsigned int num;
	Point curPoint;
	replayPoint loadPoint;
	string str;
	int pX, pY;

	if (!replay || (replay && numList.empty()))
	{
		curPoint = generateLocation();
		num = generateNum();
		while (!checkNumLocation(num, snakes, board, curPoint,monstersPoints)) { // checks validity of number location
			curPoint = generateLocation();
			num = generateNum();
		}
		numList.push_front(replayPoint(curPoint.getX(), curPoint.getY(), num));
	}
	else
	{
		loadPoint = numList.back();
		curPoint.setX(loadPoint.getX());
		curPoint.setY(loadPoint.getY());
		num = loadPoint.getNum();
		numList.pop_back();
		numListCopy.push_front(loadPoint);
	}

	//local
	pX = curPoint.getX();
	pY = curPoint.getY();

	//if number location is ok - print
	for (size_t i = 0; i < Utilities().CalculateNumDigits(num); i++)
	{
		str = to_string(num);
		board[pY][pX + i].ch = str[i];
		board[pY][pX + i].draw(board[pY][pX + i].ch);
	}
	setNumOfNumbers(numOfNumbers + 1);//every number we increment
}

// Print
void Board::printBoard() const
{
	Utilities::resetColor();
	for (size_t i = BOARD_START_LINE; i < SCREEN_HEIGHT; i++)
		for (size_t j = 0; j < SCREEN_WIDTH; j++)
			if (board[i][j].ch != ' ')
				board[i][j].draw(board[i][j].ch);
}
void Board::drawNumberFromArr(Point* solutions[60], unsigned int numOfSolutions) const
{
	for (size_t i = 0; i < numOfSolutions; i++)
	{
		unsigned int x = solutions[i]->getX();
		unsigned int y = solutions[i]->getY();
		while (board[y][x].ch != ' ' && x < SCREEN_WIDTH) //draw all the number
		{
			board[y][x].draw(board[y][x].ch);
			x++;
		}
	}
}