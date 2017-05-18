// A program that runs a program (notepad++.exe instead of notepad.exe)
// Note to compile use 'g++ runNotepadPPExe.cpp -o notepad.exe -mwindows'

#include <iostream>
#include <stdlib.h>
#include <string>

using std::cout;
using std::endl;

int main(int args, char** argv){

for(int i=0; i < args; i++)
cout << argv[i] << endl;

std::string sysCalls;
sysCalls="/cygdrive/b/programs/windows/Notepad++/notepad++.exe ";

for(int i=1; i < args; i++)
	sysCalls=sysCalls+argv[i]+std::string(" ");

sysCalls=sysCalls+"&";

for(int i=0; i < sysCalls.length(); i++)
	if( sysCalls[i] == '\\')
		sysCalls[i] = '/';

cout << sysCalls << endl;
system(sysCalls.c_str());

return 0;
}
