#include "stdafx.h"
#include <iostream>
#include<ctime>
#include<windows.h>
#include<string>
#include<vector>
#define vecint vector<int>
#define vecstr vector<string>

using namespace std;
struct value 
{
	string chip;
	int amount;
};
vector<value> val(25);

vecstr chip(25); // массив фишек
vecint amount(25);
int player=1, dice1, dice2;
string p[2] = { "#", "*" }; //фишки

void init()
{
	val[12].amount = 15;
	val[12].chip = "*";
	val[0].amount = 15;
	val[0].chip = "#";
}

void show_field()
{
	int pos = 11; // генерация с 11 ячейки
	cout << "    11   10   9    8    7    6     5    4    3    2    1    0" << endl;
	cout << "---------------------------------------------------------------" << endl;
	for (int i = 0; i < 15; i++) // первые 15 строк
	{
		pos = 11;
		cout << "| ";
		for (int j = 0; j < 12; j++)
		{
			if (val[pos].amount - i > 0)
			{
				cout << "  " + val[pos].chip + "  ";
			}
			else cout << "     ";
			if (j == 5)
				cout << "|";
			pos--;
		}
		cout << "|" << endl;
	}
	for (int i = 15; i > 0; i--) // последние 15 строк
	{
		pos = 12;
		cout << "| ";
		for (int j = 0; j < 12; j++)
		{
			if (val[pos].amount - i >= 0)
			{
				cout << "  " + val[pos].chip + "  ";
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
	cout << "           На первом кубике выпало:      " << dice1 << endl;
	cout << "           На втором кубике выпало:      " << dice2 << endl;
}

bool turn_is_possible(vecint dices, bool head_chip)
{
	int count = 0; // считаем, что оба хода возможны
	for (int i = 0; i < dices.size(); i++) // проверяем возможность хода с очками на кубиках
	{
		int ok_positions = 24; // позиция "плохая", если на ней нет фишек или с неё нельзя сходить, изначально считаем, что все "хорошие"
		for (int j = 0; j < 24 - dices[i]; j++)
			if ( (val[j].chip == p[player - 1] && val[j + dices[i]].chip == p[(player % 2)])  ||  val[j].chip.empty()  ||  (j==0 && head_chip && player==1)
				|| (j==12 && head_chip && player==2) || val[j].chip!=p[player-1])
				ok_positions--; /* если на текущей позиции - ваша фишка, а на итоговой - фишка соперника, или текущая позиция пустая,
								 или если это голова и с нее уже сняли фишку, или же на этой позиции чужая фишка, то метим позицию как плохую)))))*/
								// (player-1) - текущий игрок, ( (player %2)+1 ) - противник!
		if (ok_positions == 0) 
			count++; // ход с этими очками невозможен
	}
	if ( count==dices.size() )
		return false;
	return true;		
}

void turn()
{
	vecint dices;
	int pos, points;
	bool flag = false;// признак того, что ход верен
	bool head_chip = false;
	throw_dice();
	dices.push_back(dice1);
	dices.push_back(dice2);
	cout << "Ходит игрок " << player << endl;
		if (dice1 == dice2) // дубль
		{
			cout << "Вам выпал дубль!!!" << endl << endl;
			dices.push_back(dice1);
			dices.push_back(dice2);
		}
		while (!dices.empty())
		{
			if (turn_is_possible(dices, head_chip))
			{
				cout << "Введите номер ячейки, откуда переставлять:     ";
				cin >> pos;
				while ((val[pos].chip != p[player - 1]) || (head_chip && pos == 12 && player == 2) || (head_chip && pos == 0 && player == 1))
					// если ход чужой фишкой или попытка снять еще одну с головы
				{
					if ((pos == 0 && player == 1) || (pos == 12 && player == 2))
						cout << "С головы фишку снимать больше нельзя. Введите другую позицию" << endl;
					else
						cout << "Здесь чужая фишка или пустая ячейка. Введите позицию заново." << endl;
					cin >> pos;
				}
				if ((pos == 0 && player == 1) || (pos == 12 && player == 2)) // сняли фишку с головы, больше снимать нельзя
					head_chip = true;
				cout << "Количество очков:    ";
				cin >> points;
				while (val[(pos + points) % 24].chip == p[player % 2])
				{
					cout << "Ход невозможен, здесь чужая фишка. Введите количество очков заново или любую другую цифру, чтобы изменить ход." << endl;
					cin >> points;
				}
				for (int i = 0; i < dices.size(); i++)
					if (points == dices[i])
					{
						dices.erase(dices.begin() + i); // удаляем кубик из массива кубиков
						flag = true;
						break;
					}

				if (flag) // flag - признак верности хода, меняем расположение фишки
				{
					if ((player == 1 && (pos + points) <= 23) || (player == 2 && (pos + points) >= 6)) //условие того, что фишка не выйдет из дома
					{
						val[(pos + points) % 24].chip = val[pos].chip;
						val[pos].amount--;
						if (val[pos].amount == 0)
							val[pos].chip.erase(); // удаляем фишку, если ячейка стала пустой
						val[(pos + points) % 24].amount++;

						system("cls");
						show_field();

						if (!dices.empty())
							cout << "У вас остались следующие ходы:   " << endl;
						for (int i = 0; i < dices.size(); i++)
							cout << dices[i] << " ";
						cout << endl;
						flag = false;
					}
				}
				else
					cout << "Ходить из дома нельзя." << endl;
			}else
			{
				cout << "Вы не можете ходить, ход переходит сопернику";
				Sleep(1000);
				break;
			}
		}
	head_chip = false;
	player = (player % 2) + 1; //переход хода другому игроку
}

int main()
{
	setlocale(LC_ALL, "Rus");
	init();
	show_field();
	while (true)
		turn();
	system("pause");
	return 0;
}