#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "t6h.h"
#include "t6h.cpp"


using namespace std;

const long interval = pow(10, 5);
const int numThreads = 15;
short EncryptOption = 1;

struct encode
{
	bool done;
	bool ready;
	long end;
	char Doc[interval + 1];
	char* passLoc;
	long pwsize;
	int* executef;
	int executeNum;
};
struct writer
{
	int end[numThreads];
	char *filename;
	bool Die;
	bool *DoneLocations;
	bool done;
	void *p1;
	int totalReps;
};
void *writee(void *cat);
void *encrypt(void *encode);

void *writee(void *cat)
{
	const int byteSize = sizeof(encode);

	int curwriter = 0;
	writer& joe = *(writer*) cat;
	encode& encoders = *(encode*) joe.p1;
	joe.Die = 0;

	std::ofstream write;
	write.open((char*) &joe.filename[0]);
	//int sent = 99999;

	//May prematurely end
	long reps = 0;
	while (1/*sent > 0*/)
	{
		void *p1 = ((char*) &encoders.done + curwriter * byteSize);
		void *p2 = ((char*) &encoders.end + curwriter * byteSize);
		void *p3 = ((char*) &encoders.ready + curwriter * byteSize);

		bool& Edone = *(bool*) p1;
		bool& Eready = *(bool*) p3;

		while (Edone == 0)
		{
			if (joe.totalReps == reps - 1)
			{
				cout << "Ending write" << endl;
				write.close();
				joe.done = 1;
				return NULL;
			}
			usleep(99);
		}
		long& Eend = *(long*) p2;

		for (int i = 0; i <= Eend; i++)
		{
			char a = *(char*) (&encoders.Doc[i] + curwriter * byteSize);
			write << a;
		}
		Edone = 0;
		Eready = 1;
		curwriter++;
		if (curwriter == numThreads)
			curwriter = 0;
		reps++;
	}

	cout << "Error!" << endl;
	return NULL;
}
void printLong(long a)
{
	cout << a << endl;
}
void *encrypt(void *p1)
{

	encode& cat = *(encode*) p1;
	//ready will be 1 when writee is finished with it.
	cat.ready = 0;
	if (EncryptOption != 0)
	{
		Scramble(cat.passLoc, cat.pwsize, &cat.Doc[0], cat.end, cat.executef, cat.executeNum);
	}
	else
	{
		DScramble(cat.passLoc, cat.pwsize, &cat.Doc[0], cat.end, cat.executef, cat.executeNum);
	}

	cat.done = 1;
	pthread_exit(NULL);
	return NULL;
}

int main()
{
	while (1)
	{
		int executeNum = 25;
		int executef[] = { -3, 1, 1, -2, 1, 2, 2, 3, 1, 3, 3, 2, 2, 1, 2, -1, -2, 1, 1, 2, 1, -2, -2, 3, 1 };
		pthread_t threads[numThreads];
		pthread_t WriteThread;
		encode encoders[numThreads];
		writer joe;
		joe.done = 0;
		ifstream read;
		string input;
		cout << "Enter password: ";
		cin >> input;
		//input = "password";
		string opassword = input;
		long pwSize = getLength((char*) &input[0]);
		char password[pwSize + 1];
		password[pwSize] = 0;
		stringToChar((char*) &input[0], &password[0], pwSize);
		short FileDoesNotExist = 1;
		while (FileDoesNotExist)
		{
			cout << "enter the document to be encrypted/decrypted: ";
			cin >> input;
//	input = "ReadDoc.txt";
			read.open((char*) &input[0]);
			if (!read)
			{
				cout << "error: no file found!" << endl;
				read.close();
			}
			else
				FileDoesNotExist = 0;
		}
		while (1)
		{
			ifstream temp;
			cout << "encrypted document to be created: ";
			cin >> input;
			temp.open((char*) &input[0]);
			short option;
			if (temp)
			{
				temp.close();
				cout << "File exists. Overwrite? 1 for yes, 0 for no:";
				cin >> option;
				if (option != 0) //if yes
				{
					ofstream temp2;
					temp2.open((char*) &input[0]);
					temp2.close();
					break;
				}
				temp.close();
			}
			else
			{
				temp.close();
				break;
			}
		}
//	input = "haha.txt";
		joe.filename = &input[0];
		cout << "1 for encrypt, 0 for decrypt: ";
		cin >> EncryptOption;

		read >> std::noskipws;

		for (int i = 0; i < numThreads; i++)
		{
			encoders[i].Doc[interval] = 0;
			encoders[i].done = 0;
			encoders[i].ready = 1;
			encoders[i].pwsize = pwSize;
			encoders[i].executeNum = executeNum;
			encoders[i].executef = &executef[0];
			encoders[i].passLoc = &password[0];
		}
		joe.p1 = &encoders[0];
		joe.totalReps = -5;
		pthread_create(&WriteThread, NULL, *writee, &joe);

		int curThread = 0;

		long reps = 0;
		while (read /* && sent > 0*/)
		{
			//Delays until == 1. writee sets ready
			while (encoders[curThread].ready == 0)
			{
				usleep(99);
			}

			long i;
			for (i = 0; i < interval && read; i++)
			{
				read >> encoders[curThread].Doc[i];/* = read.get()*/
			}
			if (!read)
				i--;
			i--;
			encoders[curThread].end = i;

			//	cout << "thread " << curThread << " created" << endl;
			pthread_create(&threads[curThread], NULL, *encrypt, &encoders[curThread]);

			curThread++;
			if (curThread == numThreads)
				curThread = 0;

			//	sent--;
			reps++;
		}
		joe.Die = 1;
		joe.totalReps = reps - 1;
		;
		//end of while

		bool AllDone = 0;
		while (!AllDone)
		{
			AllDone = 1;
			for (int i = 0; i < numThreads; i++)
			{
				if (encoders[i].done == 1)
				{
					AllDone = 0;
					cout << "waiting for thread " << i << endl;
				}

			}
			if (joe.done == 0)
			{
				cout << "waiting for writer" << endl;
				AllDone = 0;
			}
			usleep(66666);
		}
		cout << "All done!" << endl;
		cout << "Run again? 1 for yes, 0 for no: ";
		cin >> EncryptOption;
		if (EncryptOption == 0)
			return 0;

	}
	return 0;
}

