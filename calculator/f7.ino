//#include <p3.h>

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//**********************************
//The number of pins that will be initialized
const short numpins = 25;
const short maxequationsize = 99;
//**********************************
//Coresponding pins
short copin[numpins];
//cochar = coresponding characters
char cochar[numpins];
//Only numpins of copin are initialized
//When 1, the fist part of program ends
boolean HaveString = 0;
//The equation string 
char equation[maxequationsize];
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//The LCD code
//****************************
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
char top[17];
char bottem[17];
int pos = 0; // Position of eqation to be displayed
//****************************

//*************************
double answer;
boolean printans = 0;
//*************************

void setup()
{

	Serial.begin(9600);
	short cat = 0;

	cat = 0;
	for (short i = 24; i <= 48; i++)
	{
		copin[cat] = i;
		cat++;
	}

	cat = 0;
	cochar[cat++] = 0;

	cochar[cat++] = 'B';
	cochar[cat++] = 'T';
	cochar[cat++] = 'C';
	cochar[cat++] = 'S';
	cochar[cat++] = 'L';

	cochar[cat++] = 'R';
	cochar[cat++] = '(';
	cochar[cat++] = ')';
	cochar[cat++] = '.';
	cochar[cat++] = '+';

	cochar[cat++] = '-';
	cochar[cat++] = '*';
	cochar[cat++] = '/';
	cochar[cat++] = '^';
	cochar[cat++] = '0';

	cochar[cat++] = '1';
	cochar[cat++] = '2';
	cochar[cat++] = '3';
	cochar[cat++] = '4';
	cochar[cat++] = '5';

	cochar[cat++] = '6';
	cochar[cat++] = '7';
	cochar[cat++] = '8';
	cochar[cat++] = '9';
//	cochar[cat++] = '0';

//Sets all of copin to read
	for (int i = 0; i < numpins; i++)
	{
		digitalRead(copin[i]);
	}
}
//end of setup

void loop()
{
//Responds to push buttons. Also occationally prints to LCD 
	part1();
//Evalueates the equation
	evaluate();
//Clears the equation for repeat
	clearequation();
}
//end of main loop

void part1()
{
	//Push button wait delay
	static int Cdelay = 20;
	static const char DisplaySize = 4+8;
//1 for neg,positive, 1 for decimal, 1 for space, 1 for E, 2 for following digits
	//static char answerChar[DisplaySize];

	while (!HaveString)
	{
		//tdelay is how long untill text is re-displayed
		static int tdelay = 0;
		getstring();

//how often LCD is too be refreshed
		if (tdelay > 1000)
		{
			//This is where the LCD screne is refreshed with new equation
			if (!printans)
			{
				Serial.println(equation);
				fullprint(&equation[pos]);
			}
			if (printans)
			{

//The numerical ans to be eval, the string location, how much is to be displayed
			//	toString(answer, &answerChar[0], DisplaySize-6);

				lcd.begin(16, 2);
				lcd.setCursor(0, 0);
//Checks for overflow
//				if (answer + 1 != answer)
//				{
//					Serial.println(answerChar);
//					lcd.print(answerChar);
//
//				}
//				else
//				{
					Serial.println(answer);
					lcd.print(answer);
//				}
			}
			tdelay = 0;
		}
		tdelay += Cdelay;
		delay(Cdelay);

	}
	HaveString = 0;

	Serial.println("Equation is ready for eval!");
}
//end of part 1 function^^

//Here goes the code for evaluating
void evaluate()
{
	answer = anss(&equation[0]);
	Serial.println(answer);
}
void getstring()
{
	//The place that a char will be added to the equation
	static short place = 0;
	static boolean on[numpins];
	//Enterpushed prevents getstring from repeatidly executing a pushed enter button
	static boolean ennterpushed = 1;

	//used to prevent repetiton
	static boolean BeenOn[numpins];

//Goes through every avaliable pin
	for (short i = 0; i < numpins; i++)
	{
//is 1 when at 5v
		on[i] = digitalRead(copin[i]);

		//If it has not already been on, do stuff
		if (on[i] == 1 && !BeenOn[i])
		{
			BeenOn[i] = 1;
			//adds a char to equation
			equation[place] = cochar[i];
			//Ends the program if enter has been pushed
			if (cochar[i] == 0 && !ennterpushed)
			{
				pos = 0;
				HaveString = 1;
				place = 0;
				ennterpushed = 1;
				printans = 1;
				return;
			}

			//If it was not enter that was pushed...
			ennterpushed = 0;
			place++;
			printans = 0;
			switch (cochar[i])
			{
			case 'B':
				place--;
				Serial.println("Moving Back!");
				equation[place] = 0;
				place--;
				if (place != 0)
				{
					equation[place] = 0;

				}

				break;
			case 'L':
				place--;
				equation[place] = 0;
				shift(0);
				Serial.println("Not moving left");

				break;
			case 'R':
				place--;
				equation[place] = 0;
				shift(1);
				Serial.println("Not moving right");
				break;
			}
			//end of special switch case statement
		}

		//end of if 1 statement. continues to end of loop
		if (on[i] == 0)
		{
			BeenOn[i] = 0;
		}

	}
	//end of for loop
}
//end of getstring^^

void clearequation()
{
	for (int i = 0; i < maxequationsize; i++)
		equation[i] = 0;
}

void fullprint(char* PositionPointer)
{
	setBT(PositionPointer);
	Print();
}

void Print()
{
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print(top);
	lcd.setCursor(0, 1);
	lcd.print(bottem);
}

//Set bottem and top array to be displayed
void setBT(char *location)
{
	static const short cols = 16;

	for (short i = 0; i < cols; i++)
	{
		top[i] = *(location + i);
	}

	for (short i = 0; i < cols; i++)
	{
		bottem[i] = *(location + cols + i);
	}
	int equSize = findSize(&equation[0]);
	//Takes Care of bottem having non zero values when it is at the end of position
	for (int i = pos + cols - equSize; i > 0; i--)
	{
		top[cols - i] = 0;
	}
	//Takes Care of bottem having non zero values
	for (int i = pos + 2 * cols - equSize; i > 0; i--)
	{
		bottem[cols - i] = 0;
	}
}

void shift(boolean d)
//for d = 0, go left. For d = 1, go right
{
// do nothing 
	if (d == 0 && pos == 0)
	{
		return;
	}

// do nothing  
	if (d == 1 && pos >= findSize(&equation[0]))
	{
		return;
	}

	//If d==0, go left
	if (!d)
	{
		pos--;

	}
	if (d)
	{
		pos++;
	}
}
//end of shift function^^

//Returns the factorial of given number
long factorial(long num)
{
	if (num == 0 || num == 1)
		return 1;
	else
	{
		num = num * factorial(num - 1);
		return num;
	}
}

//finds the amount of a specific char
int CharCount(char* equation, char chartype, int begin, int end)
{
	int count = 0;
//	int totalsize = findSize(equation);
	for (int i = begin; i < end; i++)
	{
		if (*(i + equation) == chartype)
		{
			count++;
		}
	}
	return count;
}

//Finds the size of a string. called with findSize(&equation[0]);
int findSize(char *p)
{
	int i = 0;
	while (*(i + p) != 0)
	{
		i++;
	}
	return i;
}

//parses a string to double
double parse(char* equation, int begin, int end)
{
	char cursur;
	double value = 0;
	int dloc = find(equation, '.', begin, end);

	int loops = 0;
	//before decimal point values
	for (int i = dloc - 1; i >= begin; i--)
	{
		cursur = *(char*) (equation + i) - '0';
		value += cursur * pow(10, loops);
		loops++;
	}
	loops = -1;
	for (int i = dloc + 1; i <= end; i++)
	{
		cursur = *(char*) (equation + i) - '0';
		value += cursur * pow(10, loops);
		loops--;
	}

	return value;
}

//finds the first location of a char between start and end
int find(char* stringer, char cat, int start, int end)
{
	int location;
	for (location = start; location <= end; location++)
	{
		if (*(char*) (stringer + location) == cat)
		{
			break;
		}
	}
	if (location == end)
		return -1;

	return location;
}
//Rfind goes from right to left
int Rfind(char* stringer, char cat, int start, int end)
{
	int location;
	for (location = end; location >= start; location--)
	{
		if (*(char*) (stringer + location) == cat)
		{
			break;
		}
	}
	if (location == start - 1)
		return -1;

	return location;
}

//Finds the location of the first parenthesis pair that comes across
void locs(char* equation, int* cat)
{
	int p1 = -1;
	int p2 = -1;
	int size = findSize(&equation[0]);
	for (int i = 0; i < size; i++)
	{
		if (*(char*) (equation + i) == '(' || equation[i] == 'S' || equation[i] == 'C'
				|| equation[i] == 'T')
		{
			p1 = i;
		}
		if (*(char*) (equation + i) == ')')
		{
			p2 = i;
			break;
		}
	}

	*cat = p1;
	*(cat + 1) = p2;
}

//Makes the array hold values. Also assigns 1,0 to sigs array
void toDubbs(char* equation, double* thedubs, bool* sigs)
{
	int size = findSize(&equation[0]);
//Makes all values of sigs 0
	for (int i = 0; i < size; i++)
	{
		*(bool*) (sigs + i) = 0;
	}

	int l1 = 0;
	int l2 = 0;
	int i = 0;
	bool letter = 1;
	;
	while (i < size)
	{

		while (letter)
		{
			letter = (equation[i] == '+' || equation[i] == '-' || equation[i] == '*'
					|| equation[i] == '/' || equation[i] == '^' || equation[i] == '('
					|| equation[i] == ')' || equation[i] == 'S' || equation[i] == 'C'
					|| equation[i] == 'T');
			i++;
		}
		l1 = i - 1;
		l2 = i;
		while (!letter)
		{
			letter = (equation[i] == '+' || equation[i] == '-' || equation[i] == '*'
					|| equation[i] == '/' || equation[i] == '^' || equation[i] == '('
					|| equation[i] == ')') || equation[i] == 0 || equation[i] == 'S'
					|| equation[i] == 'C' || equation[i] == 'T';
			i++;
		}
		l2 = i - 2;
		thedubs[l2] = parse(&equation[0], l1, l2);
		sigs[l2] = 1;

	}

}

void RightParentasisCorrect(char* equation)
{

	int size = findSize(&equation[0]);
	int pL = CharCount(equation, '(', 0, size);
	pL += CharCount(equation, 'S', 0, size);
	pL += CharCount(equation, 'C', 0, size);
	pL += CharCount(equation, 'T', 0, size);

	int pR = CharCount(equation, ')', 0, size);
	for (int i = 0; i < pL - pR; i++)
	{
		equation[size + i] = ')';
	}
}

void ExpEval(double* thedubs, char* equation, bool* sigs, int begin, int end)
{

	while (CharCount(&equation[0], '^', begin, end))
	{
		int opp = Rfind(&equation[0], '^', begin, end);
		int b4 = Rfind((char*) &sigs[0], 1, begin, opp);
		int after = find((char*) &sigs[0], 1, opp, end);

		thedubs[b4] = pow(thedubs[b4], thedubs[after]);
		sigs[after] = 0;
		equation[opp] = '#';

	}
}

void MulDivEval(double* thedubs, char* equation, bool* sigs, int begin, int end)
{
	int m;
	int d;
	int opp;
	int b4;
	int after;

	int numDiv;
	int numMul;
	numDiv = CharCount(&equation[0], '/', begin, end);
	numMul = CharCount(&equation[0], '*', begin, end);
	if (numDiv + numMul == 0)
	{
		return;
	}
	do
	{
		numDiv = CharCount(&equation[0], '/', begin, end);
		numMul = CharCount(&equation[0], '*', begin, end);

		m = find(&equation[0], '*', begin, end);
		d = find(&equation[0], '/', begin, end);

		if (((m < d) && (numMul > 0)) || (numDiv == 0))
		{
			opp = find(&equation[0], '*', begin, end);
			b4 = Rfind((char*) &sigs[0], 1, begin, opp);
			after = find((char*) &sigs[0], 1, opp, end);
			thedubs[b4] = thedubs[b4] * thedubs[after];
			sigs[after] = 0;
			equation[opp] = '#';
			numMul--;
		}

		else

		{
			opp = Rfind(&equation[0], '/', begin, end);
			b4 = Rfind((char*) &sigs[0], 1, begin, opp);
			after = find((char*) &sigs[0], 1, opp, end);
			thedubs[b4] = thedubs[b4] / thedubs[after];
			sigs[after] = 0;
			equation[opp] = '#';
			numDiv--;
		}
	} while (numDiv + numMul > 0);
}

//Exactly the same as MulDivEval except '*' becomes '+', and '/' become '-'
void PlusMinusEval(double* thedubs, char* equation, bool* sigs, int begin, int end)
{
	int m;
	int d;
	int opp;
	int b4;
	int after;

	int numDiv;
	int numMul;

	numDiv = CharCount(&equation[0], '-', begin, end);
	numMul = CharCount(&equation[0], '+', begin, end);
	if (numDiv + numMul == 0)
	{
		return;
	}
	do
	{
		numDiv = CharCount(&equation[0], '-', begin, end);
		numMul = CharCount(&equation[0], '+', begin, end);
		m = find(&equation[0], '+', begin, end);
		d = find(&equation[0], '-', begin, end);

		if (((m < d) && (numMul > 0)) || (numDiv == 0))
		{
			opp = find(&equation[0], '+', begin, end);
			b4 = Rfind((char*) &sigs[0], 1, begin, opp);
			after = find((char*) &sigs[0], 1, opp, end);
			thedubs[b4] = thedubs[b4] + thedubs[after];
			sigs[after] = 0;
			equation[opp] = '#';
			numMul--;
		}

		else

		{
			opp = Rfind(&equation[0], '-', begin, end);
			b4 = Rfind((char*) &sigs[0], 1, begin, opp);
			after = find((char*) &sigs[0], 1, opp, end);
			thedubs[b4] = thedubs[b4] - thedubs[after];
			sigs[after] = 0;
			equation[opp] = '#';
			numDiv--;
		}
	} while (numDiv + numMul > 0);

}

double solve(double* thedubs, char* equation, bool* sigs, int begin, int end)
{
	ExpEval(&thedubs[0], &equation[0], &sigs[0], begin, end);
	MulDivEval(&thedubs[0], &equation[0], &sigs[0], begin, end);
	PlusMinusEval(&thedubs[0], &equation[0], &sigs[0], begin, end);
	return thedubs[0];
}

int PlCount(char* equation, int size)
{
	int pL = CharCount(equation, '(', 0, size);
	pL += CharCount(equation, 'S', 0, size);
	pL += CharCount(equation, 'C', 0, size);
	pL += CharCount(equation, 'T', 0, size);
	return pL;
}

double anss(char* equation)
{
	RightParentasisCorrect(&equation[0]);

	int size = findSize(&equation[0]);

	//the array of doubles that will be used. sigs tells if the doubles are significant
	double thedubs[size];
	bool sigs[size];

	toDubbs(&equation[0], &thedubs[0], &sigs[0]);

	int begin = 0;
	int end = size;
	int plocs[2];
	int RpCount = PlCount(&equation[0], size);
	for (int i = 0; i < RpCount; i++)
	{
		locs(&equation[0], plocs);
		solve(&thedubs[0], &equation[0], &sigs[0], plocs[0], plocs[1]);
		funcEval(plocs[0], plocs[1], &equation[0], sigs, &thedubs[0]);
		equation[plocs[0]] = '#';
		equation[plocs[1]] = '#';
	}

	solve(&thedubs[0], &equation[0], &sigs[0], begin, end);

	for (int i = 0; i < size; i++)
	{
		if (sigs[i] == 1)
		{
			return thedubs[i];
		}
	}
	//never reaches return statement
	return 0;
}

//Changes number between l1 and l2 for function purposes
void funcEval(int l1, int l2, char* equation, bool* sigs, double* thedubs)
{
	int activespot = find((char*) &sigs[0], (char) 1, l1, l2);
	char funcType = equation[l1];

	double ans = thedubs[activespot];
	switch (funcType)
	{
	case 'C':
		ans = sinus(ans + PI / 2);
		break;
	case 'S':
		ans = sinus(ans);
		break;
	case 'T':
		ans = log(ans);
		break;
	}
	thedubs[activespot] = ans;
}

double sinus(double num)
{
	num = num - 2 * PI * (int) (num / PI / 2);
	short neg = 1;
	if (num - PI > 0)
	{
		num -= PI;
		neg = -1;
	}
	double sinn = 0;
	for (short i = 1; i < 15;)
	{
		sinn += pow(num, i) / factorial(i);
		i += 2;
		sinn -= pow(num, i) / factorial(i);
		i += 2;
	}
	return sinn * neg;
}

//The numerical ans to be eval, the string location, how much is to be displayed
void toString(double ans, char* answer, int displayy)
{

	if( ans >= 0)
	{
		answer[0]= '+';
	}
	else
	{
		ans = -ans;
		answer[0]= '-';
	}
int power = log10(ans);
	double pureans = ans / pow(10, power);
//1 for decimal, 1 for space, 1 for E, 2 for following digits
	for (short i = 1; i <= displayy; i++)
	{
		answer[i] = 0;
	}


	answer[1] = (short) pureans + '0';
	answer[2] = '.';
	pureans = 10 * (pureans - (short) pureans);

	for (int i = 3; i < displayy + 2; i++)
	{
		answer[i] = (short) pureans + '0';
		pureans = 10 * (pureans - (short) pureans);

	}
	answer[displayy + 2] = 'E';
	answer[displayy + 3] = ' ';
	answer[displayy + 4] = (short) (power / 10) + '0';
	answer[displayy + 5] = power % 10 + '0';
	short place = 2;
	char temp;
	if (power < displayy)
	{
		for (short i = 0; i < power; i++)
		{
			temp = answer[place];
			answer[place] = answer[place + 1];
			answer[place + 1] = temp;
			place++;
		}
		answer[displayy + 2] = 0;
		answer[displayy + 3] = 0;
		answer[displayy + 4] = 0;
		answer[displayy + 5] = 0;
	}

}
