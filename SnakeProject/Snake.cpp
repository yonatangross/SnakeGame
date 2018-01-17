#include "Snake.h"
#include <synchapi.h> //TODO: WHO THE FUCK ARE YOU?
#include "Utilities.h"
#include "Board.h"
#include <string>

// CTOR/DTOR
Snake::Snake(const Point& head, char c1, int _color, Directions dir)
{
	color = _color;
	c = c1;
	direction = dir;
	blockedDirection = blockedDir(dir);
	body[0] = head;
}
Snake& Snake::operator=(const Snake& other)
{
	{
		if (this == &other)
			return *this;
		log_size = other.log_size;
		direction = other.direction;
		blockedDirection = other.blockedDirection;
		c = other.c;
		shoot_key = other.shoot_key;
		color = other.color;
		active_shots = other.active_shots;
		numOfShots = other.numOfShots;
		inactiveCounter = other.inactiveCounter;
		isStuck = other.isStuck;
		active = other.active;
		for (int i = 0; i < PH_SIZE; i++)
			body[i] = other.body[i];
		for (int i = 0; i < NUM_OF_DIRS; i++)
			dirKeys[i] = other.dirKeys[i];
		return *this;
	}
}

// Getters/Setters
void Snake::setKeys(char keyLeft, char keyRight, char keyUp, char keyDown, char shootKey)
{
	dirKeys[LEFT] = keyLeft;
	dirKeys[RIGHT] = keyRight;
	dirKeys[UP] = keyUp;
	dirKeys[DOWN] = keyDown;
	shoot_key = shootKey;
}

void Snake::setBodyPartLocation(int i, int x, int y)
{
	body[i].setPoint(x, y);
}

// Functions
Directions Snake::blockedDir(Directions dir)
{
	switch (dir)
	{
	case(LEFT):
		return RIGHT;
	case(RIGHT):
		return LEFT;
	case(UP):
		return DOWN;
	case(DOWN):
		return UP;
	default: break;
	}
	return{};
}

void Snake::changeDir(char keyPressed)
{
	for (const auto& key : dirKeys)
	{
		if (key == keyPressed)
			if (blockedDirection != Directions(&key - dirKeys))
			{
				direction = Directions(&key - dirKeys);
				blockedDirection = blockedDir(direction);
				return;
			}
	}
}

void Snake::move(char keyPressed, bool isStuck)
{
	if (!isStuck)
	{
		body[log_size - 1].draw(' ');
		for (int i = log_size - 1; i > 0; --i)
		{
			body[i] = body[i - 1];
		}
		changeDir(keyPressed);
		body[0].movePoint(direction);
		Utilities::setColor(color); //SET COLOR TO SNAKE COLOR
		body[0].draw(c);
		Utilities::resetColor(); //UNSET COLOR
	}
}

bool Snake::incrementSnake()
{
	log_size++;
	if (log_size == 12)
	{
		cout << "PLAYER: " << this->c << " YOU WON THE GAME\n";
		Sleep(1000);
		return false; //game is over
	}
	return true; //game is NOT over
}

bool Snake::isDirection(char key_pressed)
{
	for (const auto& key : dirKeys)
	{
		if (key == key_pressed)
			return true;
	}
	return false;
}

Directions Snake::calcDir(char key_pressed)
{
	Directions res = {};
	for (const auto& key : dirKeys)
		if (key == key_pressed)
			res = Directions(&key - dirKeys);
	return res;
}

Point Snake::getNextHeadPos(const Point& point, Directions nextDir) const
{
	Point res = point;

	if (nextDir == blockedDirection)
		res.changeDir(direction);
	else
		res.changeDir(nextDir);

	res.moveImpl();
	return res;
}

void Snake::checkIfStuck(Snake otherSnake, char keyPressed)
{
	Directions nextDir;
	Point nextHeadPos;

	if (isDirection(keyPressed))
		nextDir = calcDir(keyPressed);
	else
		nextDir = direction;

	nextHeadPos = getNextHeadPos(body[0], nextDir);

	for (size_t i = 1; i < log_size; i++)
		if ((body[0].getX() == body[i].getX()) && (body[0].getY() == body[i].getY()))
		{ //if the head is touching any other part of his
			isStuck = true;
			return;
		}

	for (size_t i = 0; i < otherSnake.GetSnakeSize(); i++) //Here we have to watch the otherSnake Head as well
		if (nextHeadPos.getX() == otherSnake.body[i].getX() && nextHeadPos.getY() == otherSnake.body[i].getY())
		{ //if the head is touching athe otherSnake Body\Head
			isStuck = true;
			return;
		}

	isStuck = false;
}

void Snake::Shoot()
{
	if (numOfShots > 0)
	{
		Shot s(body[0], direction);
		active_shots.push_back(s);
		numOfShots--;
	}
}

void Snake::PrintStats(Snake snakes[NUM_OF_SNAKES])
{
	string secondStr = " Player 2:  Score " + to_string(snakes[1].GetSnakeSize()) + " Ammo " + to_string(snakes[1].numOfShots);
	gotoxy(SCREEN_WIDTH / 8, 1);
	Utilities::setColor(snakes[0].color);
	cout << " Player 1: ";
	cout << " Score " << snakes[0].GetSnakeSize() << " Ammo " << snakes[0].numOfShots;
	Utilities::resetColor();

	gotoxy((SCREEN_WIDTH * 7 / 8) - secondStr.length(), 1);
	Utilities::setColor(snakes[1].color);
	cout << " Player 2: ";
	cout << " Score " << snakes[1].GetSnakeSize() << " Ammo " << snakes[1].numOfShots;
	Utilities::resetColor();
}

void Snake::handleShots(Board& board, Snake snakes[NUM_OF_SNAKES], char keyPressed,Monster *monsters[5])
{
#define	SHOTSPEED 2 //This Const defines the number of movements per snake's movement

	if (active && keyPressed == this->shoot_key)
		this->Shoot();
	for (size_t i = 0; i < active_shots.size(); ++i)
		for (size_t j = 0; j < SHOTSPEED; ++j) //BUG: maybe need to do the loop outside?
		{
			active_shots[i].move(getHead());//Move to the same direction as the snake
			if (active_shots[i].HandleHit(active_shots, board, snakes, numOfShots, color,monsters))
			{//return true iff this shot has to been deleted
				active_shots.erase(active_shots.begin() + i);
				if (numOfShots > 0)
					numOfShots--;
				break;
			}
		}
	PrintStats(snakes);
}

bool Snake::HandleActivation()
{//return true IFF the snake had to be reposition
	if (!active)
	{
		inactiveCounter++;
		if (inactiveCounter == 5)
		{
			inactiveCounter = 0;
			active = true;
			isStuck = false;
			return true;
		}
	}
	return false;
}

void Snake::deleteSnakeFromScreen()
{
	int y_value = getHead().getY();//local

	for (unsigned int i = 0; i < log_size; i++)
	{
		if (y_value >= BOARD_START_LINE && y_value < SCREEN_HEIGHT) { //IF THE SNAKE IN THE BOARD HE HAVE TO CLEAR IT FROM THE CREEN
			body[i].draw(' ');
			body[i].setPoint(100 + i, 100 + i);
		}
	}
}

void Snake::hideSnake()
{
	active = false;
	isStuck = true;
	deleteSnakeFromScreen();
}

void Snake::resetShots()
{
	active_shots.clear();
	numOfShots = STARTING_NUM_OF_SHOTS;
}

bool Snake::CmpShotsToMonster(Snake snakes[NUM_OF_SNAKES], Point monsterPoint, bool& isHit)
{
		for (size_t i = 0; !isHit &&i < active_shots.size(); ++i)
			if (active_shots[i].cmpToPoint(monsterPoint))
			{
				active_shots.erase(active_shots.begin() + i); 
				numOfShots--; 
				isHit = true;
				return true;
			}
	return false;
}