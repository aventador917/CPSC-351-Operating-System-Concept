/*
Paul Tang

CPSC 351
October 25, 2019

Assignment#3
*/

#include<iostream>
#include<thread>
#include<mutex>
#include<fstream>
#include<string>

using namespace std;

mutex mtx;

void printLetters(string fileName)
{
	mtx.lock();

	fstream file;
	file.open(fileName,fstream::app);
	for (int c = 0; c < 20; c++)
	{
		for (char i = 'A'; i <= 'Z'; i++)
		{
			file<< static_cast<char>(i) << " ";
		}
		file<< endl;
	}
	file.close();
	mtx.unlock();
}
void printNumbers(string fileName)
{
	mtx.lock();

	fstream fs;
	fs.open(fileName,fstream::app);
	
	for (int c = 0; c < 20; c++)
	{
		for (int i=1 ; i <= 26; i++)
		{
			fs << i << " ";	
		}
		fs<< endl;
	}
	fs.close();
	mtx.unlock();
}


int main()
{
	ofstream file("synch.txt");
	
	thread thrA(printLetters,"synch.txt");
	thread thrB(printNumbers,"synch.txt");
	
	thrA.join();
	thrB.join();
	
	system("pause");
	return 0;
}