#include "Mission.h"

// Functions
unsigned int Mission::getNumber(const Point& p, const Board &boardObj)
{
	unsigned int num = 0;
	int x = p.getX();
	//Need to go to the left edge - Not Crossing the borders of the screen
	while (boardObj.board[p.getY()][x].ch != ' ' && x > 0)
		x--;
	//read left to right = num untill whiteSpace
	x++;
	while (boardObj.board[p.getY()][x].ch != ' ' && x < SCREEN_WIDTH)
	{
		num *= 10;
		num += (boardObj.board[p.getY()][x].ch - '0');
		x++;
	}
	return num;
}
bool Mission::checkWin(unsigned num, missionsType missionType)
{
	switch (missionType)
	{
	case missionsType::Prime:
		return Prime(num);
	case missionsType::DivisionInX:
		return DivisionInX(num);
	case missionsType::Multiplication:
		return Multiplication(num);
	case missionsType::SquareRoot:
		return SquareRoot(num);
	case missionsType::DivisionWithReminder:
		return DivisionWithReminder(num);
	case missionsType::squareOfX:
		return squareOfX(num);
	case missionsType::EquationMission:
		return Equation(num);
	case missionsType::numUnderX:
		return numUnderX(num);
	case missionsType::numBetweenXY:
		return numBetweenXY(num);
	case missionsType::numFollowingX:
		return numFollowingX(num);
	default: break;
	}
	return false;
}
void Mission::handleEat(const Point& p, Snake snakes[2], int i, Board& boardObj, missionsType msnType, bool& gameOver)
{
	unsigned int currNum;
	string win = "Pass The Level !!!\n";
	string lose = "Wrong Number !!!\n";

	PlaySound(TEXT("eat.WAV"), nullptr, SND_NODEFAULT);
	currNum = getNumber(p, boardObj);
	boardObj.removeNumber(p, boardObj);
	if (checkWin(currNum, msnType)) // correct number -> pass level
	{
		if (snakes[i].incrementSnake())
			printStat(win);
		else
			gameOver = true;
	}
	else // wrong number -> pass level (the other player gets the points).
	{
		if (snakes[(i + 1) % 2].incrementSnake())
			printStat(lose);
		else
			gameOver = true; //return false if the snake length = 12
	}
	Utilities().ClearScreen();
}

unsigned Mission::GetSolutions(Point* solutions[60], Board& board)
{//Return NumOfSolutions
	unsigned int num, numOfSolutions = 0;
	for (size_t i = 3; i < SCREEN_HEIGHT; i++)
		for (size_t j = 0; j < SCREEN_WIDTH; j++)
		{
			if (board.board[i][j].ch != ' ')
			{
				num = getNumber(board.board[i][j], board);
				if (checkWin(num, missionsType(this->GetMissionNum())))	//IF THERE IS A SOLUTION
					solutions[numOfSolutions++] = &(board.board[i][j]); //pointer to the solution point + increment the num of solutions
			}
		}
	return numOfSolutions;
}
void Mission::checkEndOfStage(Board &board)
{ //Checks whether the players could have won the stage or a correct number just didn't appear.
	unsigned int numOfSolutions = 0;
	Point *solutions[60];
	Utilities().ClearScreen();
	cout << "NO TIME IS LEFT\n";
	Sleep(300);

	numOfSolutions = GetSolutions(solutions, board);

	if (numOfSolutions > 0)
	{
		cout << "YOU MISSED THESE SOLUTIONS: ";
		Utilities::setColor(0012);
		board.drawNumberFromArr(solutions, numOfSolutions);
		Sleep(1300);
		Utilities::setColor(0011);
		board.drawNumberFromArr(solutions, numOfSolutions);
		Sleep(1300);
		Utilities::resetColor();
	}
	else
		cout << "NO SOLUTIONS EXIST,GOOD JOB!";

	newMissionMessage();
}
void Mission::SetEquation()
{//SET ALL PARAMATERS IN MISSION7
#define GET_LEFT_OPRAND false
#define GET_RIGHT_OPRAND true

	unsigned int res = 0;
	unsigned int res2 = 0;

	eqObj->op1 = rand() % eqObj->NUM_OF_OPERATORS;
	eqObj->op2 = rand() % eqObj->NUM_OF_OPERATORS;

	if (eqObj->op2 == eqObj->multiplication || eqObj->op2 == eqObj->division) //OP2 is FIRST
	{
		eqObj->num2 = rand() % NUM_OF_NUMBERS;
		eqObj->num3 = getValidNumber(res, eqObj->num2, eqObj->op2, GET_RIGHT_OPRAND);//FIRST WHILE
		eqObj->num1 = getValidNumber(res2, res, eqObj->op1, GET_LEFT_OPRAND);//SECOND WHILE - RES = <NUM1> OP <NUM2>
		eqObj->num4 = res2;
	}
	else														  //OP1 is First
	{
		eqObj->num1 = rand() % NUM_OF_NUMBERS;
		eqObj->num2 = getValidNumber(res, eqObj->num1, eqObj->op1, GET_RIGHT_OPRAND);//FIRST WHILE
		eqObj->num3 = getValidNumber(res2, res, eqObj->op2, GET_RIGHT_OPRAND);//SECOND WHILE - RES = <NUM1> OP <NUM2>
		eqObj->num4 = res2;
	}
}
string Mission::CreateEqMissionStr(unsigned missing_num)
{
	string str;
	//addition, subtraction, multiplication, division
	string Signs[4] = { "+","-", "*",  "/" };

	switch (missing_num)
	{
	case EquationMission::NUM_LOCATIONS::num1_loc:
		str = " ___ " + Signs[eqObj->op1] + " " + to_string(eqObj->num2) + " " + Signs[eqObj->op2] + " " + to_string(eqObj->num3) + " = " + to_string(eqObj->num4); break;
	case EquationMission::NUM_LOCATIONS::num2_loc:
		str = to_string(eqObj->num1) + " " + Signs[eqObj->op1] + " ___ " + Signs[eqObj->op2] + " " + to_string(eqObj->num3) + " = " + to_string(eqObj->num4); break;
	case EquationMission::NUM_LOCATIONS::num3_loc:
		str = to_string(eqObj->num1) + " " + Signs[eqObj->op1] + " " + to_string(eqObj->num2) + " " + Signs[eqObj->op2] + " ___ " + "= " + to_string(eqObj->num4); break;
	case EquationMission::NUM_LOCATIONS::num4_loc:
		str = to_string(eqObj->num1) + " " + Signs[eqObj->op1] + " " + to_string(eqObj->num2) + " " + Signs[eqObj->op2] + " " + to_string(eqObj->num3) + " = " + " ___"; break;
	default: break;
	}
	return "Eat the missing part that Solves the equation: " + str;
}
int Mission::calc(char op, unsigned leftNum, unsigned rightNum, bool &reminder)
{
	switch (op)
	{
	case EquationMission::addition:
		return leftNum + rightNum;
	case EquationMission::subtraction:
		return  leftNum - rightNum;
	case EquationMission::multiplication:
		return leftNum * rightNum;
	case EquationMission::division:
		if (rightNum == 0)
			return  -1;
		if (leftNum % rightNum != 0)
			reminder = true;
		return leftNum / rightNum;
	default:
		return leftNum + rightNum;
	}
}
bool Mission::isLegalNumber(unsigned &res, unsigned resNum, unsigned num, char op, bool mode)
{	//MODE 1 -> GET RIGHT OPERAND (NUM IS THE NUMBER LEFT TO OP OPERATOR)
	//MODE 0 -> GET LEFT OPERAND  (NUM IS THE NUMBER RIGHT TO OP OPERATOR)
	bool legalNum = true;
	bool reminder = false;

	if (mode)
	{
		if (resNum == 0 && op == EquationMission::OPERATORS::division)
			legalNum = false;
		else
			res = calc(op, num, resNum, reminder); // calc dosnt return reminder
	}
	else
	{
		if (num == 0 && op == EquationMission::OPERATORS::division)
			legalNum = false;
		else
			res = calc(op, resNum, num, reminder); // calc dosnt return reminder
	}
	if (res < 0 || res > NUM_OF_NUMBERS || reminder)
		legalNum = false;

	return legalNum;
}
unsigned Mission::getValidNumber(unsigned &res, unsigned num, char op, bool mode)
{
	//MODE 1 -> GET RIGHT OPERAND (NUM IS THE NUMBER LEFT TO OP OPERATOR)
	//MODE 0 -> GET LEFT OPERAND  (NUM IS THE NUMBER RIGHT TO OP OPERATOR)

	bool legalNum = false;
	unsigned int resNum = 0;

	while (!legalNum)
	{
		resNum = rand() % NUM_OF_NUMBERS;
		legalNum = isLegalNumber(res, resNum, num, op, mode);
	}

	return resNum;
}
void Mission::CreateEquationMission()
{
	eqObj = new EquationMission;
	SetEquation();
	eqObj->missing_num_location = (rand() % EquationMission::NUM_OF_NUM_LOCATIONS) + 1; //WHICH NUMBER WILL BE MISSING 
	SetInstructions(CreateEqMissionStr(eqObj->missing_num_location));
}

// Print
void Mission::printInstructions() const
{
	Utilities::setColor(YELLOW);
	gotoxy(SCREEN_WIDTH / 2 - Instructions.length() / 2, 0);
	cout << " " << Instructions << endl << endl;
	Utilities::resetColor();
	cout << "________________________________________________________________________________" << endl;

}
void Mission::newMissionMessage()
{
	Utilities().ClearScreen();
	gotoxy(SCREEN_WIDTH / 2 - strlen("STARTING A NEW MISSION IN :\n") / 2, (SCREEN_HEIGHT - 1) / 2);
	cout << "STARTING A NEW MISSION IN :  ";

	for (size_t i = 3; i > 0; i--)
	{
		Sleep(300);
		cout << i << ' ';
		PlaySound(TEXT("bleep.wav"), nullptr, SND_NODEFAULT);
	}
	Sleep(300);
	Utilities().ClearScreen();
}
void Mission::printStat(string stat)
{
	Utilities::ClearScreen();
	gotoxy(SCREEN_WIDTH / 2 - (stat.length() / 2), SCREEN_HEIGHT / 2);
	cout << stat;
	Sleep(1000);
	Utilities::ClearScreen();
}
// Missions
bool Mission::Prime(unsigned num)
{
	if (num == 2) //2 IS THE FIRST PRIME
		return true;
	if (num == 0) //ZERO IS NOT A PRIME 
		return false;

	for (unsigned int i = 2; i < num; i++)
		if (num % i == 0)
			return false;

	return true;
}
bool Mission::DivisionInX(unsigned num)
{
	if (num % x1 == 0)
		return true;
	return false;
}
bool Mission::Multiplication(unsigned num)
{
	if (num % x1 == 0)
		return true;
	return false;
}
bool Mission::SquareRoot(unsigned num)
{
	for (auto i = 1; i <= 13; i++)
	{
		if (num == i*i)
			return true;
	}
	return false;
}
bool Mission::DivisionWithReminder(unsigned num)
{
	if (num % x1 == x2)
		return true;
	return false;
}
bool Mission::squareOfX(unsigned num)
{
	if (num == x1 * x1)
		return true;
	return false;
}
bool Mission::Equation(unsigned num)
{
	bool reminder = false;
	int resLeft, resRight;
	if (eqObj == nullptr) return false; //Avoid OverFlow
	switch (eqObj->missing_num_location)
	{
	case EquationMission::NUM_LOCATIONS::num1_loc:
		if (eqObj->op2 == eqObj->multiplication || eqObj->op2 == eqObj->division) //OP2 is FIRST
		{
			resRight = calc(eqObj->op2, eqObj->num2, eqObj->num3, reminder);
			if (eqObj->op1 == eqObj->division && resRight == 0) return false;
			resLeft = calc(eqObj->op1, num, resRight, reminder);
			return resLeft == eqObj->num4;
		}
		else														  //OP1 is First
		{
			resLeft = calc(eqObj->op1, num, eqObj->num2, reminder);
			resRight = calc(eqObj->op2, resLeft, eqObj->num3, reminder);
			return resRight == eqObj->num4;
		}
	case EquationMission::NUM_LOCATIONS::num2_loc:
		if (eqObj->op2 == eqObj->multiplication || eqObj->op2 == eqObj->division) //OP2 is FIRST
		{
			resRight = calc(eqObj->op2, num, eqObj->num3, reminder);
			if (eqObj->op1 == eqObj->division && resRight == 0) return false;
			resLeft = calc(eqObj->op1, eqObj->num1, resRight, reminder);
			return resLeft == eqObj->num4;
		}
		else														  //OP1 is First
		{
			if (eqObj->op1 == eqObj->division && num == 0) return false;

			resLeft = calc(eqObj->op1, eqObj->num1, num, reminder);
			resRight = calc(eqObj->op2, resLeft, eqObj->num3, reminder);
			return resRight == eqObj->num4;
		}
	case EquationMission::NUM_LOCATIONS::num3_loc:
		if (eqObj->op2 == eqObj->multiplication || eqObj->op2 == eqObj->division) //OP2 is FIRST
		{
			if (eqObj->op2 == eqObj->division && num == 0) return false;
			resRight = calc(eqObj->op2, eqObj->num2, num, reminder);
			if (eqObj->op1 == eqObj->division && resRight == 0) return false;
			resLeft = calc(eqObj->op1, eqObj->num1, resRight, reminder);
			return resLeft == eqObj->num4;
		}
		else														  //OP1 is First
		{
			resLeft = calc(eqObj->op1, eqObj->num1, eqObj->num2, reminder);
			resRight = calc(eqObj->op2, resLeft, num, reminder);
			return resRight == eqObj->num4;
		}
	case EquationMission::NUM_LOCATIONS::num4_loc:
		if (eqObj->num4 == num) return true;
	default: break;
	}
	return false;
}
bool Mission::numUnderX(unsigned num)
{ //BUG: new missions numEater doesn't work. 
  //Bug: snakes are written in instructions lines.
	if (num < x1)
		return true;
	return false;
}
bool Mission::numBetweenXY(unsigned num)
{
	if (x1 <= num && num <= x2)
		return true;
	return false;
}
bool Mission::numFollowingX(unsigned num)
{
	if (x1 == num - 1)
		return true;
	return false;
}