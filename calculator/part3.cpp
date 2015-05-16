#include <iostream>
#include <math.h>
#include "p3.h"
using namespace std;

const int maxequationsize = 99;

int main() {
	std::cout << "Enter the equation:" << std::endl;
	while (1) {
		char equation[maxequationsize];
		std::cin >> equation;
		double answer = anss(&equation[0]);
		cout << answer << endl;
		clear(&equation[0], maxequationsize);
		continue;
	}
	return 0;
}
