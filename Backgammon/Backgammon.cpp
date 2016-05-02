#include "stdafx.h"
#include <iostream>
#include<ctime>
#include<string>
#include<vector>
#define vecint vector<int>
#define vecstr vector<string>

using namespace std;

vecstr chip(25);
vecint amount(25);
int player = 1, dice1, dice2;
string p[2] = { "#", "*" };

void init()
{
	amount[12] = 15;
	chip[12] = "*";
	amount[0] = 15;
	chip[0] = "#";
}

void show_field()
{
	int pos = 11; // ��������� � 11 ������
	cout << "    11   10   9    8    7    6     5    4    3    2    1    0" << endl;
	cout << "---------------------------------------------------------------" << endl;
	for (int i = 0; i < 15; i++) // ������ 15 �����
	{
		pos = 11;
		cout << "| ";
		for (int j = 0; j < 12; j++)
		{
			if (amount[pos] - i > 0)
			{
				cout << "  " + chip[pos] + "  ";
			}
			else cout << "     ";
			if (j == 5)
				cout << "|";
			pos--;
		}
		cout << "|" << endl;
	}
	for (int i = 15; i > 0; i--) // ��������� 15 �����
	{
		pos = 12;
		cout << "| ";
		for (int j = 0; j < 12; j++)
		{
			if (amount[pos] - i >= 0)
			{
				cout << "  " + chip[pos] + "  ";
			}
			else cout << "     ";
			if (j == 5)
				cout << "|";
			pos++;
		}
		cout << "|" << endl;
	}


	cout << "---------------------------------------------------------------" << endl;
	cout << "    12   13   14   15   16   17    18   19   20   21   22   23" << endl;
	for (int i = 0; i < 3; i++)
		cout << endl;
}

void throw_dice()
{
	srand(time(0));
	dice1 = rand() % 6 + 1;
	dice2 = rand() % 6 + 1;
	cout << "           �� ������ ������ ������:      " << dice1 << endl;
	cout << "           �� ������ ������ ������:      " << dice2 << endl;
}

void turn(int player)
{
	vecint dices;
	int pos, points;
	bool flag = 0; // ������� ����, ��� ��� �����
	throw_dice();
	dices.push_back(dice1);
	dices.push_back(dice2);
	if (dice1 == dice2) // �����
	{
		cout << "��� ����� �����!!!" << endl << endl;;
		dices.push_back(dice1);
		dices.push_back(dice2);
	}
	while (!dices.empty())
	{
		cout << "������� ����� ������, ������ ������������:     ";
		cin >> pos;
		while (chip[pos] != p[player - 1])
		{
			cout << "������ ������ ������ �������!" << endl;
			cin >> pos;
		}
		cout << "���������� �����:    ";
		cin >> points;
		for (int i = 0; i < dices.size(); i++)
			if (points == dices[i])
			{
				dices.erase(dices.begin() + i); // ������� ����� �� ������� �������
				flag = true;
				break;
			}
		if (flag)
		{
			chip[pos + points] = chip[pos];
			amount[pos]--;
			amount[pos + points]++;

			system("cls");
			show_field();

			if (!dices.empty())
				cout << "� ��� �������� ��������� ����:   " << endl;
			for (int i = 0; i < dices.size(); i++)
				cout << dices[i] << " ";
			cout << endl;
			flag = false;
		}
	}
	player = (player % 2) + 1;
}


int main()
{
	setlocale(LC_ALL, "Rus");
	init();
	show_field();
	turn(player);
	system("pause");
	return 0;
}