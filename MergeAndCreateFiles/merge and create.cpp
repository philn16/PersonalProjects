#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

long sentinalValue = pow(10, 9.5);
int numfiles;

int find(char* string, char character);
void intToString(char* name, int i, int places);
bool create(string filename, string fileextension);
void merge(string filename, string fileextension, string combo);

int main()
{

	string mergedname;
	bool d;
	std::cout << "\n 1 for merge, 0 for split: ";
	cin >> d;
	string input;

	while (1)
	{
		ifstream temp;
		if (d)
		{
			cout << "\n What is the base name of the file that is to be merged? Example:cat.jpg.: ";
			cin >> input;
			temp.open((char*) &input[0]);
			if (!temp)
			{
				std::cout << "Error! File does not exist" << std::endl;
				continue;
			}
			cout << "\n How many files are to be merged?: ";
			cin >> numfiles;
			break;
		}
		if (!d)
		{
			std::cout << "\n What is the name of the file that is to be split?: ";
			cin >> input;
			temp.open((char*) &input[0]);
			if (!temp)
			{
				std::cout << "Error! File does not exist" << std::endl;
				continue;
			}
			std::cout << "\n How many files are to be created?: ";
			cin >> numfiles;
			break;
		}
	}
	int point = find(&input[0], '.');
	int end = find(&input[0], 0);
	char fileextension[end - point + 1];
	fileextension[end - point] = 0;
	for (int i = point; i < end; i++)
	{
		fileextension[i - point] = input[i];
	}
	char filename[point + 1];
	filename[point] = 0;
	for (int i = 0; i < point; i++)
	{
		filename[i] = input[i];
	}

	if (!d)
	{
		bool a = create(filename, fileextension);
		if (!a)
		{
			std::cout << "There was a critical error!" << std::endl;
			return 0;
		}
	}
	if (d)
	{
		std::cout << "\n What is the name of the desired file to be created?: ";
		cin >> mergedname;
		merge(filename, fileextension, mergedname);
	}
	std::cin.ignore();
	return 0;
}
bool create(string filename, string fileextension)
{
	string combo = filename + fileextension;
	ifstream read;
	read.open((char*) &combo[0]);
	if (!read)
	{
		cout << "error!" << endl;
		return 0;
	}
	read >> std::noskipws;

	string names[numfiles];
	ofstream writers[numfiles];

	int places = log10(numfiles) + 1;
	char nums[places + 1];
	nums[places] = 0;

	for (int i = 0; i < numfiles; i++)
	{
		nums[places] = 0;
		intToString(&nums[0], i, places);
		names[i] = filename + nums + fileextension;
		writers[i].open((char*) &names[i][0]);
		//		cout << names[i] << endl;
	}
	int curWriter;
	char a;
	a = read.get();
	int s;
	for (s = 0; s < sentinalValue && read; s++)
	{
		curWriter = s % numfiles;
		writers[curWriter] << a;
		a = read.get();
	}
	if (s >= sentinalValue)
	{
		cout << "sentinalValue Error!" << endl;
	}
	read.close();
	return 1;
}

void intToString(char* name, int number, int places)
{
	int temp = number;
	for (int j = 0; j < places; j++)
	{
		*(char*) (name + j) = 48 + temp / pow(10, places - 1 - j);
		temp = temp - (int) (temp / pow(10, places - 1 - j)) * pow(10, places - 1 - j);
	}
}

int find(char* string, char character)
{
	int location;

	for (int s = 0; s < 999; s++)
	{
		if (string[s] == character)
		{
			location = s;
			return location;
		}
	}

	return -1;
}
void merge(string filename, string fileextension, string make)
{

	ifstream reader[numfiles];
	ofstream write;
	write.open((char*) &make[0]);
	string names[numfiles];

	int places = log10(numfiles) + 1;
	char nums[places + 1];
	nums[places] = 0;
	for (int i = 0; i < numfiles; i++)
	{
		nums[places] = 0;
		intToString(&nums[0], i, places);
		names[i] = filename + nums + fileextension;
		reader[i].open((char*) &names[i][0]);
		reader[i] >> std::noskipws;
		if (!reader[i])
		{
			cout << "Error!: file " << names[i] << " not found." << endl;
		}
	}
	char letters[numfiles];

	for (int i = 0; i < numfiles; i++)
	{
		letters[i] = reader[i].get();
	}

	bool noError = 1;
	while (noError)
	{
		for (long i = 0; i < numfiles; i++)
		{

			if (!reader[i])
			{
				noError = 0;
				break;
			}
			else
			{
				write << letters[i];
				letters[i] = reader[i].get();
			}

		}
	}

	write.close();
	for (int i = 0; i < numfiles; i++)
	{
		reader[i].close();
	}
}
