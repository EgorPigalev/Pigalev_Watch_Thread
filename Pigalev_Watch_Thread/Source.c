#include <Windows.h>
#include <stdio.h>

HANDLE hThread[4];

int hour = 0;
int minute = 0;
int second = 0;
int milesecond = 0;

int hour4 = 0;
int minute4 = 0;
int second4 = 0;
int milesecond4 = 0;

// таймерное время
int hour3;
int minute3;
int second3;

void WorkHours() // Поток имитирующий работу часов
{
	while (1)
	{
		second++;
		if (second == 60)
		{
			minute += 1;
			second = 0;
		}
		if (minute == 60)
		{
			minute = 0;
			hour += 1;
		}
		if (hour == 24)
		{
			hour = 0;
		}
		Sleep(1000);
	}
}

void WorkHoursSecondomer() // Поток имитирующий работу часов с милисекундами
{
	while (1)
	{
		milesecond4++;
		if (milesecond4 == 10)
		{
			second4++;
			milesecond4 = 0;
		}
		if (second4 == 60)
		{
			minute4 += 1;
			second4 = 0;
		}
		if (minute4 == 60)
		{
			minute4 = 0;
			hour4 += 1;
		}
		if (hour4 == 24)
		{
			hour4 = 0;
		}
		Sleep(94);
	}
}

void ShowHours() // Вывод часов
{
	while (1)
	{
		unsigned int start_time = clock();
		system("cls");
		printf("%d:%d:%d\n", hour, minute, second);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		Sleep(1000 - search_time);
	}
}

void ShowHoursMileSecond() // Вывод часов в милисекундах
{
	while (1)
	{
		unsigned int start_time = clock();
		system("cls");
		printf("%d:%d:%d:%d\n", hour4, minute4, second4, milesecond4);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		Sleep(100 - search_time);
	}
}

void GetTimer()
{
	while (1)
	{
		if (hour4 == hour3 && minute4 == minute3 && second4 == second3 && milesecond4 == 0)
		{
			break;
		}
	}
}

void main()
{
	system("chcp 1251>nul");

	hThread[0] = CreateThread(NULL, 0, WorkHours, NULL, 0, 0);
	hThread[1] = CreateThread(NULL, 0, ShowHours, NULL, 0, 0);

	int command;
	int commandSec = 0; // команда вкл/выкл секундомер

	// Для секундомера
	int hour1;
	int minute1;
	int second1;
	int milesecond1;

	// Для таймера
	// время до таймера
	int hour2;
	int minute2;
	int second2;
	int milesecond2;

	while (1)
	{
		scanf_s("%d", &command);
		switch (command)
		{
		case(1):
			if (ResumeThread(hThread[0]) != 0)
			{
				printf("Пауза выключена\n");
				ResumeThread(hThread[0]);
				ResumeThread(hThread[1]);
			}
			else
			{
				printf("Включена пауза\n");
				SuspendThread(hThread[0]);
				SuspendThread(hThread[1]);
			}
			break;
		case(2):
			SuspendThread(hThread[0]);
			SuspendThread(hThread[1]);
			system("cls");
			while (1)
			{
				printf("Введите время:");
				scanf_s("%d%d%d", &hour, &minute, &second);
				if (ProverkaVhodData(hour, minute, second) == 0)
				{
					break;
				}
			}
			milesecond = 0;
			system("cls");
			ResumeThread(hThread[0]);
			ResumeThread(hThread[1]);
			break;
		case(3):
			if (commandSec == 0)
			{
				SuspendThread(hThread[1]);
				system("cls");
				printf("Включен режим секундомера\n");
				hThread[2] = CreateThread(NULL, 0, ShowHoursMileSecond, NULL, 0, 0);
				hThread[3] = CreateThread(NULL, 0, WorkHoursSecondomer, NULL, 0, 0);
				commandSec = 1;
			}
			else
			{
				SuspendThread(hThread[2]);
				SuspendThread(hThread[3]);
				printf("Режим секундомера выключен\n");
				ResumeThread(hThread[1]);
				commandSec = 0;
			}
			break;
		case(4):
			SuspendThread(hThread[1]);
			system("cls");
			printf("Включен режим таймера\n");
			while (1)
			{
				printf("Введите таймерное время: ");
				scanf_s("%d%d%d", &hour3, &minute3, &second3);
				if (ProverkaVhodData(hour3, minute3, second3) == 0)
				{
					break;
				}
			}
			hThread[2] = CreateThread(NULL, 0, ShowHoursMileSecond, NULL, 0, 0);
			hThread[3] = CreateThread(NULL, 0, WorkHoursSecondomer, NULL, 0, 0);
			HANDLE b = CreateThread(NULL, 0, GetTimer, NULL, 0, 0);
			WaitForSingleObject(b, INFINITE);
			CloseHandle(b);
			SuspendThread(hThread[2]);
			SuspendThread(hThread[3]);
			printf("Таймерное время вышло!\n");
			system("pause");
			system("cls");
			ResumeThread(hThread[0]);
			ResumeThread(hThread[1]);
			break;
		default:
			CloseHandle(hThread[0]);
			CloseHandle(hThread[1]);
			CloseHandle(hThread[2]);
			CloseHandle(hThread[3]);
			return;
			break;
		}
	}
}

int ProverkaVhodData(int hour, int minute, int second) // Проверка при вводе часов
{
	if (hour >= 24 || minute >= 60 || second >= 60 || hour < 0 || minute < 0 || second < 0)
	{
		printf("Указано несуществующее время! Повторите ввод!\n");
		return 1;
	}
	return 0;
}
