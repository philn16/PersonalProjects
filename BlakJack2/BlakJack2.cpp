#include <iostream>
using namespace std;

#include "blackjackfunctions.h"

void modifyDeck(int deck[]);
void printDeck(int deck[]);

void modifyDeck(int deck[])
{
	while (1)
	{
		int a;
		int b;
		printDeck(deck);
		std::cout << "which element would you like to change? If not 2-14, end: ";
		std::cin >> a;
		if (!(a >= 2 && a <= 14))
		{
			cout << "Ending deck modify" << endl;
			return;
		}
		cout << "What would you like to replace it with? ";
		cin >> b;
		deck[a] = b;
	}
}

void printDeck(int deck[])
{
	for (short i = 2; i <= 14; i++)
	{
		std::cout << "[" << i << "]: " << deck[i] << endl;
	}
}

int main()
{
	while (1)
	{
		long sentinalmax = 920000;
		int deck[12];
		int deckr[12];
		double tprob;
		int acepos;
		int deckorigional[] = { 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

		int faceup = 6;
		while (1)
		{
			cout << "0-21 faceup value expected. Anything else for modifying deck" << endl;
			cout << "Faceup is: ";
			cin >> faceup;
			if (!(faceup >= 0 && faceup <= 21))
			{
				modifyDeck(deckorigional);
				continue;
			}
			break;
		}
		int numcards = sum(deckorigional, 14);

		fixdeck(deckorigional, deck);
		fixdeck(deckorigional, deckr);
		//deck is now {0,0,4,4,4,4,4,4,4,4,16,4
		double probs[11];
//		probs[0] = 1;
		int level = 0;
		// 17,18,19,20,21,bust
		double finalprobs[6] = { 0, 0, 0, 0, 0, 0 };
		int played[27];
		played[0] = faceup;
		int sums;

		int card = 2;
		long sentinal = 0;

		while (level >= 0 && sentinal < sentinalmax)
		{
			sentinal++;

			if (card > 11)
			{
				leveldown(card, played, deck, level);
				continue;
			}

			if (deck[card] > 0)
			{
				levelup(played, level, card, probs, deck);
				sums = sum(played, level);
				acepos = aceposition(played, level);
				replacewith11(played, level);
				sums = sum(played, level);

				while (acepos >= 0 && sums > 21)
				{
					acepos = aceposition(played, level);
					played[aceposition(played, level)] = 1;
					sums = sum(played, level);
				}

				if (sums <= 21 && sums >= 17) // Dealer holds
				{
					tprob = newprobcalculate(played, level, deckr, numcards);
					finalprobs[sums - 17] += tprob;
					leveldown(card, played, deck, level);
					deck[11] += deck[1]; //
					deck[1] = 0; //
					continue;
				}
				if (sums > 21) // No aces now. Dealer Busts
				{
					tprob = newprobcalculate(played, level, deckr, numcards);
					finalprobs[5] += tprob;
					leveldown(card, played, deck, level);
					continue;
				}

				card = 2;
				continue;
			}
			else
				//deck[card] <= 0
				card++;

		} //end of loop
		if (faceup >= 2 && faceup <= 11)
		{
			deck[faceup]--;
		}

//cout << decksum(deck) << " is deck sum" << endl;
		cout << endl << endl;
		printfinalprobs(finalprobs);

		if (addedfinalprobs(finalprobs) > 1.0001 || addedfinalprobs(finalprobs) < 0.9999)
			cout << '\n' << "error!   " << addedfinalprobs(finalprobs) << "  == added total" << endl;

//cout << endl << "Faceup is " << faceup << endl;
		cout << '\n' << sentinal << " calculations" << endl;

		if (sentinal == sentinalmax)
		{
			cout << "****************" << endl;
			cout << " CRITICAL SENTINAL ERROR!" << endl;
			cout << "****************" << endl;

		}
		int d;
		cout << "End program?. 0 for yes, 1 for no: ";
		cin >> d;
		if (d == 0)
			return 0;
	}
	return 0;
}

