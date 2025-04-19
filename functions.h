#pragma once
#include <windows.h>
#include <iostream>
using std::cout;
using std::cin;

extern CRITICAL_SECTION array_sect;

extern HANDLE* releaseEvent;

extern HANDLE* stopEvent;

struct ArrayStruct
{
	int* array = 0;
	int array_size = 0;
};

struct Thread_Info
{
	unsigned int index = 0;
	bool shouldStop = false;
};

struct Parameters
{
	ArrayStruct* array;
	Thread_Info* thread;

	Parameters() : array(0), thread(0)
	{

	}

	Parameters(ArrayStruct* as, Thread_Info* ti) : array(as), thread(ti)
	{

	}
};

DWORD WINAPI marker(LPVOID parameters);

#include <limits>

void cin_value(int& value, int min);

void cin_value(int& value, int min, int m);