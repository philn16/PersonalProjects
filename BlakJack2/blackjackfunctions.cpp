#include <iostream>
#include <iomanip>
#include "blackjackfunctions.h"

using namespace std;

void fixdeck(int deck1[], int deck[])
{
	for (int x = 0; x <= 9; x++)
	{
		deck[x] = deck1[x];
	}
	deck[10] = 0;
	for (int x = 10; x <= 13; x++)
	{
		deck[10] += deck1[x];
	}
	deck[11] = deck1[14];
}
void leveldown(int& card, int played[], int deck[], int& level)
{

	card = played[level] + 1;
	deck[played[level]]++;
	level--;
	deck[11] += deck[1];
	deck[1] = 0;
	if (card == 2)
		leveldown(card, played, deck, level);

}
void levelup(int played[], int& level, int card, double probs[], int deck[])
{
	level++;
	played[level] = card;
	probs[level] = prob(card, deck);

	deck[card]--;

}
void printfinalprobs(double finalprobs[6])
{
	for (int x = 0; x <= 4; x++)
	{
		cout << finalprobs[x] << "   [" << x + 17 << "]" << endl;
	}
	cout << finalprobs[5] << "   [bust]" << endl;
}
void printplayed(int played[], int level, int card)
{
	for (int x = 0; x <= level; x++)
		//cout << left << setw(3) << played[x]<<' ';
		cout << played[x] << ' ';
//cout << "      " << card;
	cout << endl;
}

void replacewith1(int played[], int level)
{
	for (; level >= 0; level--)
	{
		if (played[level] == 11)
			played[level] = 1;
	}
}

void replacewith11(int played[], int level)
{
	for (; level >= 0; level--)
	{
		if (played[level] == 1)
			played[level] = 11;
	}
}
int aceposition(int played[], int level)
{
	int position;
	for (position = level; position >= 0; position--)
	{
		if (played[position] == 11)
		{
			break;
		}
	}
	return position;
}
int decksum(int deck[])
{
	int sum = 0;
	for (int x = 2; x <= 11; x++)
	{
		sum += deck[x];
	}
	return sum;
}
int sum(int played[], int level)
{
	int sum = 0;
	for (int x = 0; x <= level; x++)
		sum += played[x];

	return sum;
}

double addedfinalprobs(double finalprobs[6])
{
	double sum = 0;
	for (int x = 0; x < 6; x++)
		sum += finalprobs[x];

	return sum;
}

double newprobcalculate(int played[], int level, int deckr[], int numcards)
{
	double prob = 1;
	deckr[1] = deckr[11];

	for (int x = 1; x <= level; x++)
	{
		prob *= (double) deckr[played[x]] / numcards;

		numcards--;
		deckr[played[x]]--;
	}

	for (int x = level; x > 0; x--)
		deckr[played[x]]++;
	;

	deckr[1] = 0;
	return prob;
}
double prob(int card, int deck[])
{
	double prob = (double) deck[card] / decksum(deck);

	return prob;
}
