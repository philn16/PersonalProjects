#include <iostream>
#include "p3.h"
#include <math.h>
using namespace std;
#define PI 3.141592653589793238

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
		ans = sinus(ans) / cos(ans + PI / 2);
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

	return sinn*neg;
}

void clear(char* somestring, int ammount)
{
	for (int i = 0; i < ammount; i++)
	{
		somestring[i] = 0;
	}
}
//The numerical ans to be eval, the string location, how much is to be displayed
void toString(double ans, char* answer, int display)
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
	for (short i = 1; i <= display + 5; i++)
	{
		answer[i] = 0;
	}


	answer[1] = (short) pureans + '0';
	answer[2] = '.';
	pureans = 10 * (pureans - (short) pureans);

	for (int i = 3; i < display + 2; i++)
	{
		answer[i] = (short) pureans + '0';
		pureans = 10 * (pureans - (short) pureans);

	}
	answer[display + 2] = 'E';
	answer[display + 3] = ' ';
	answer[display + 4] = (short) (power / 10) + '0';
	answer[display + 5] = power % 10 + '0';
cout << answer << endl;
	short place = 2;
	char temp;
	if (power < display)
	{
		for (short i = 0; i < power; i++)
		{
			temp = answer[place];
			answer[place] = answer[place + 1];
			answer[place + 1] = temp;
			place++;
		}
		answer[display + 2] = 0;
		answer[display + 3] = 0;
		answer[display + 4] = 0;
		answer[display + 5] = 0;
	}

}

