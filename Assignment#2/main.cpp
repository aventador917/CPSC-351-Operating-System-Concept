/*
	Paul Tang(Zhiyuan Tang)

	CPSC 351
	September 26, 2019

	assignment#2
*/

#include <iostream>
#include <windows.h>
#include <string.h>

using namespace std;
const int length = 100;
char command[length];

DWORD WINAPI exec(LPVOID lpParamter)
{
	system(command);
	return 0;
}

int main()
{
	cout << "Welcome to myShell." << endl;
	cout << "Please enter your command." << endl;
	while (1)
	{
		cout << "cmd<< ";
		cin.getline(command, length);
		size_t pos = string(command).find(" ");

		if (string(command) == "dir" || string(command) == "help" || string(command) == "vol" || string(command) == "path" || string(command) == "tasklist" || string(command) == "notepad")
		{
			HANDLE hThread = CreateThread(NULL, 0, exec, NULL, 0, NULL);
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
		}
		else if (string(command).substr(0, pos) == "echo" || string(command).substr(0, pos) == "ping" || string(command).substr(0, pos) == "color")
		{
			HANDLE hThread = CreateThread(NULL, 0, exec, NULL, 0, NULL);
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
		}
		else if (string(command) == "exit" || string(command) == "quit")
		{
			cout << "Thank you for using myShell" << endl;
			return 0;
		}
		else
		{
			cout << "The command can't be executed, please input another command." << endl;
		}
	}
}