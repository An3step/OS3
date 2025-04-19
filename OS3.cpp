#pragma once
#include <iostream>
#include "functions.h"
CRITICAL_SECTION array_sect;

HANDLE* releaseEvent;

HANDLE* stopEvent;

HANDLE* handle_threads;

ArrayStruct arrayStruct;

int thread_count;

bool* isStop;

Thread_Info* threads;

Parameters* paramts;

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    InitializeCriticalSection(&array_sect);

    cout << "Введите размерность массива: ";

    cin_value(arrayStruct.array_size, 1);

    arrayStruct.array = new int[arrayStruct.array_size];

    for (int i = 0; i < arrayStruct.array_size; i++)
    {
        arrayStruct.array[i] = 0;
    }

    cout << "Введите количество потоков: ";
    cin_value(thread_count, 1);

    threads = new Thread_Info[thread_count];

    for (int i = 0; i < thread_count; i++)
    {
        threads[i].index = i + 1;
    }

    handle_threads = new HANDLE[thread_count];

    releaseEvent = new HANDLE[thread_count];
    for (int i = 0; i < thread_count; i++)
    {
        releaseEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    stopEvent = new HANDLE[thread_count];
    for (int i = 0; i < thread_count; i++)
    {
        stopEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    paramts = new Parameters[thread_count];

    for (int i = 0; i < thread_count; i++)
    {
        paramts[i].array = &arrayStruct;
        paramts[i].thread = &threads[i];
    }

    for (int i = 0; i < thread_count; i++)
    {
        handle_threads[i] = CreateThread(0, 0, marker, (LPVOID)(&paramts[i]), 0, 0);
    }

    isStop = new bool[thread_count];
    for (int i = 0; i < thread_count; i++)
    {
        isStop[i] = false;
    }

    while (true)
    {
        //WaitForMultipleObjects(thread_count, stopEvent, TRUE, INFINITE);
        for (int i = 0; i < thread_count; i++)
        {
            if (!isStop[i])
            {
                WaitForSingleObject(stopEvent[i], INFINITE);
            }
        }
        for (int i = 0; i < thread_count; i++)
        {
            ResetEvent(stopEvent[i]);
        }
        cout << '\n';
        for (int i = 0; i < arrayStruct.array_size; i++)
        {
            cout << arrayStruct.array[i] << ' ';
        }
        cout << '\n';
        int stopthread;
        while (true)
        {
            cout << "\nНомер останавливаемого потока: ";
            cin_value(stopthread, 1, thread_count);
            stopthread--;
            if (isStop[stopthread])
            {
                cout << "Поток уже остановлен\n";
            }
            else
            {
                break;
            }
        }
        threads[stopthread].shouldStop = TRUE;
        isStop[stopthread] = true;
        SetEvent(releaseEvent[stopthread]);
        WaitForSingleObject(stopEvent[stopthread], INFINITE);
        for (int i = 0; i < stopthread; i++)
        {
            SetEvent(releaseEvent[i]);
        }
        for (int i = stopthread + 1; i < thread_count; i++)
        {
            SetEvent(releaseEvent[i]);
        }
        //for (int i = 0; i < thread_count; i++)
        //{
        //}
        bool isbreak = true;
        for (int i = 0; i < thread_count; i++)
        {
            if (!isStop[i])
            {
                isbreak = false;
                break;
            }
        }
        if (isbreak)
        {
            break;
        }
    }
    cout << "Все потоки остановились\n";
    system("pause");
    return 0;
}