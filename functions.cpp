#pragma once
#include "functions.h"


DWORD WINAPI marker(LPVOID parameters)
{
	Parameters* paramets = (Parameters*)parameters;
	ArrayStruct* arrayStruct = paramets->array;
	Thread_Info* thread = paramets->thread;
	int array_size = arrayStruct->array_size;
	int* array = arrayStruct->array;
	srand(thread->index);
	int rand_num;
	int count = 0;
	bool* isMr = new bool[array_size];
	for (int i = 0; i < array_size; i++)
	{
		isMr[i] = false;
	}
	while (true)
	{
		rand_num = rand() % array_size;
		EnterCriticalSection(&array_sect);
		if (array[rand_num] == 0)
		{
			Sleep(5);
			array[rand_num] = thread->index;
			count++;
			isMr[rand_num] = true;
			Sleep(5);
			LeaveCriticalSection(&array_sect);
		}
		else
		{
			LeaveCriticalSection(&array_sect);
			cout << "\nНомер потока marker: " << thread->index << "\n";
			cout << "Количество помеченных элементов: " << count << "\n";
			cout << "Индекс непомечаемого элемента массива: " << rand_num;
			ResetEvent(releaseEvent[thread->index - 1]);
			SetEvent(stopEvent[thread->index - 1]);
			WaitForSingleObject(releaseEvent[thread->index - 1], INFINITE);
			if (thread->shouldStop)
			{
				break;
			}
		}
	}
	for (int i = 0; i < array_size; i++)
	{
		if (isMr[i])
		{
			EnterCriticalSection(&array_sect);
			array[i] = 0;
			LeaveCriticalSection(&array_sect);
		}
	}
	SetEvent(stopEvent[thread->index - 1]);
	return 0;
}

void cin_value(int& value, int min)
{
	while (true)
	{
		cin >> value;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(100000, '\n');
			cout << "Ошибка: пожалуйста, введите числовое значение.\n";
		}
		else if (value < min)
		{
			cout << "Введите значение в диапазоне от " << min << "\n";
		}
		else
		{
			return;
		}
	}
}

void cin_value(int& value, int min, int m)
{
	while (true)
	{
		cin >> value;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(100000, '\n');
			cout << "Ошибка: пожалуйста, введите числовое значение.\n";
		}
		else if (value < min || value > m)
		{
			cout << "Введите значение в диапазоне : [" << min << ", " << m << "]\n";
		}
		else
		{
			return;
		}
	}
}