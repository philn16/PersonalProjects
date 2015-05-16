#include <stdlib.h> // For random(), RAND_MAX
#include <string>
#include <iostream>

using namespace std;
long getLength(char* doc)
{
	for (long i = 0; 1; i++)
	{
		if (doc[i] == 0)
		{
			return i;
		}
	}
	return -1;
}

long random(long max)
{

	unsigned long
	// max <= RAND_MAX < ULONG_MAX, so this is okay.
	num_bins = (unsigned long) max + 1, num_rand = (unsigned long) RAND_MAX + 1,
			bin_size = num_rand / num_bins, defect = num_rand % bin_size;

	long x;
	// This is carefully written not to overflow
	while (num_rand - defect <= (unsigned long) (x = random()))
		;
	// Truncated division is intentional
	//no 0
	if (x / bin_size == 0)
	{
		return 1;
	}
	return x / bin_size;
}

void randomString(char* stuff, long length)
{
	for (int i = 0; i < length; i++)
	{
		stuff[i] = random(255);
	}
}

void scramble1(char* code, long length, char* doc, long pwSize)
{
	int codeLength = getLength(&code[0]);

	int curletter = 0;
	for (int i = 0; i < length; i++)
	{
		doc[i] = doc[i] + code[curletter];

		curletter++;
		if (curletter == codeLength + 1)
		{
			curletter = 0;
		}
	}
}

void dscramble1(char* code, long length, char* doc, long pwSize)
{
	int codeLength = getLength(&code[0]);
	int curletter = 0;
	for (int i = 0; i < length; i++)
	{
		doc[i] = doc[i] - code[curletter];
		curletter++;
		if (curletter == codeLength + 1)
		{
			curletter = 0;
		}
	}
}

void scramble2(char* code, long length, char* doc,  long pwSize)
{
	int codeLength = getLength(code);
	int curletter = codeLength - 1;
	long interval = length - 0;
	unsigned char temp;

	int a;
	int add;
	for (long i = 0; i <= length; i++)
	{
		add = ((unsigned char) code[curletter]) % interval;

		if (add + i > length)
		{
			a = (add - (length - i) - 1);
		}
		else
		{
			a = i + add;
		}
		if (a < 0 || a > length)
		{
			cout << "Crit error at i= " << i << " a= " << a << endl;
			return;
		}
		temp = doc[a];
		doc[a] = doc[i];
		doc[i] = temp;

		//	cout <<"i: " << i << " a: "<<a<< "  add: " << add <<lengthl;

		curletter--;
		if (curletter == -1)
		{
			curletter = codeLength - 1;
		}
	}
}

void dscramble2(char* code,long length,  char* doc,  long pwSize)
{
	int codeLength = getLength(code);
	int curletter = codeLength - (length - 0) % codeLength - 1;
	long interval = length - 0;
	unsigned char temp;

	int a;
	int add;

	for (long i = length; i >= 0; i--)
	{
		add = ((unsigned char) code[curletter]) % interval;

		if (add + i > length)
		{
			a = 0 + (add - (length - i) - 1);
		}
		else
		{
			a = i + add;
		}
		if (a < 0 || a > length)
		{
			cout << "Crit error at i= " << i << " a= " << a << endl;
			return;
		}
		temp = doc[a];
		doc[a] = doc[i];
		doc[i] = temp;

//cout <<"i: " << i << " a: "<<a<< "  add: " << add <<lengthl;

		curletter++;
		if (curletter > codeLength - 1)
		{
			curletter = 0;
		}
	}
}

void stringToChar(char* theString, char* theCharArray, long arrayLength)
{
	for (int i = 0; i < arrayLength; i++)
	{
		theCharArray[i] = theString[i];
	}
	return;
}

void pwm1(char* password, long pwsize)
{
	int curletter = pwsize - 1;
	for (int i = 0; i < pwsize; i++)
	{
		password[i] = password[i] + password[curletter];
		curletter--;
	}
}

void Scramble(char* passwordR, long &pwSize, char* doc, long length, int executefR[], int executeNum)
{
	//copys executef2
	int executef[executeNum];
	for (int x = 0; x < executeNum; x++)
	{
		executef[x] = executefR[x];
	}
//coppys password
	char password[pwSize];
	for (int i = 0; i < pwSize; i++)
	{
		password[i] = passwordR[i];
	}

	int pwchange = 0;
	int pwMax = 0;
	//gets pwMax
	for (int i = 0; i < executeNum; i++)
	{
		if (executef[i] == -3)
		{
			if (pwchange == 0)
			{
				executef[i] *= -1;
			}
			else
			{
				pwchange--;
			}
		}

		if (executef[i] == 3)
		{
			pwchange++;
			if (pwchange > pwMax)
			{
				pwMax = pwchange;
			}
		}
	}

	char pwArray[pwMax + 1][pwSize + 1];
	for (int i = 0; i <= pwMax; i++)
	{
		pwArray[i][pwSize] = 0;
		for (int x = 0; x < pwSize; x++)
		{
			pwArray[i][x] = password[x];
		}
		//	cout << "[" << i << "] " << pwArray[i] << endl;

		pwm1(&password[0], pwSize);
	}
	//the one difference. do not copy!
	pwchange = 0;

	for (int i = 0; i < executeNum; i++)
	{
		switch (executef[i])
		{
		case 0:
			std::cout << "Error Scramble? At i: " << i << std::endl;
			break;
		case 1:
			scramble1(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case -1:
			dscramble1(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case 2:
			scramble2(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case -2:
			dscramble2(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case 3:
			pwchange++;
			break;
		case -3:
			pwchange--;
			break;
		default:
			std::cout << "No function was used as executef[i]== " << executef[i] << std::endl;
			break;
		}
	}

}

void DScramble(char* passwordR, long &pwSize, char* doc, long length, int executefR[], int executeNum)
{
	//copys executef2
	int executef[executeNum];
	for (int x = 0; x < executeNum; x++)
	{
		executef[x] = executefR[x];
	}
//coppys password
	char password[pwSize];
	for (int i = 0; i < pwSize; i++)
	{
		password[i] = passwordR[i];
	}

	int pwchange = 0;
	int pwMax = 0;
	//gets pwMax
	for (int i = 0; i < executeNum; i++)
	{
		if (executef[i] == -3)
		{
			if (pwchange == 0)
			{
				executef[i] *= -1;
			}
			else
			{
				pwchange--;
			}
		}

		if (executef[i] == 3)
		{
			pwchange++;
			if (pwchange > pwMax)
			{
				pwMax = pwchange;
			}
		}
	}

	char pwArray[pwMax + 1][pwSize + 1];
	for (int i = 0; i <= pwMax; i++)
	{
		pwArray[i][pwSize] = 0;
		for (int x = 0; x < pwSize; x++)
		{
			pwArray[i][x] = password[x];
		}
//		cout << "[" << i << "] " << pwArray[i] << endl;

		pwm1(&password[0], pwSize);
	}

	for (int i = 0; i < executeNum; i++)
	{
		executef[i] *= -1;
	}

	for (int i = executeNum - 1; i >= 0; i--)
	{
		switch (executef[i])
		{
		case 0:
			std::cout << "Error DScramble? At i: " << i << std::endl;
			break;
		case 1:
			scramble1(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case -1:
			dscramble1(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case 2:
			scramble2(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case -2:
			dscramble2(&pwArray[pwchange][0], length, &doc[0], pwSize);
			break;
		case 3:
			pwchange++;
			break;
		case -3:
			pwchange--;
			break;
		default:
			std::cout << "No function was used as executef[i]== " << executef[i] << std::endl;
			break;
		}
	}

}
