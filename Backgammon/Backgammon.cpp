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
vector<value> val(25);// элемент массива - количество фишек и ее номинал
string p[2] = { "#", "*" }; //фишки
int player = 0, dice1, dice2, player1_chips, player2_chips;
string error_message = ""; //будет выводиться при неправильном вводе

void init()
{
	player1_chips = 15;
	player2_chips = 15;
	val[12].amount = 15;
	val[12].chip = "*";
	val[0].amount = 15;
	val[0].chip = "#";
}
bool end_of_game()
{
	return(player1_chips == 0 || player2_chips == 0);

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
}

void throw_dice()
{
	srand(time(0));
	dice1 = rand() % 6 + 1;
	dice2 = rand() % 6 + 1;
	cout << "           На первом кубике выпало:      " << dice1 << endl;
	cout << "           На втором кубике выпало:      " << dice2 << endl;
}

bool jump_of_turn(vecint dices, bool head_chip) // возвращает true если переход хода
{
	int count = 0; // считаем, что оба хода возможны
	for (int i = 0; i < dices.size(); i++) // проверяем возможность хода с очками на кубиках
	{
		int ok_positions = 24; // позиция "плохая", если на ней нет фишек или с неё нельзя сходить, изначально считаем, что все "хорошие"
		for (int j = 0; j < 24; j++)
			if ( (val[j].chip == p[player] && val[ (j + dices[i])%24 ].chip == p[((player+1) % 2)])  ||(j==0 && head_chip && player==0)
				|| (j==12 && head_chip && player==1) || val[j].chip!=p[player])
				ok_positions--; 
		/* если на текущей позиции - ваша фишка, а на итоговой - фишка соперника, или текущая позиция пустая,
		 или если это голова и с нее уже сняли фишку, или же на этой позиции чужая фишка, то метим позицию как плохую)))))*/
			// (player-1) - текущий игрок, ( (player+1) %2 ) - противник!
		if (ok_positions == 0) 
			count++; // ход с этим кубиком невозможен
	}
	if ( count==dices.size() )
		return true;
	return false;		
}

bool out_of_house(int pos, int points) //возвращает true, если фишка вышла из дома
{
	return ( (player == 1 && pos >= 6 && pos <= 11 && (pos + points)>11) || (player == 0 && pos >= 18 & pos <= 23 && (pos + points)>23) );
}

bool chips_in_house() //проверка на то что все фишки в доме
{
	int count = 0,start, end;
	switch (player)
	{
	case(0) :
		start = 18;
		end = 23;
		break;
	case(1) :
		start = 6;
		end = 11;
		break;
	}
	for (int i = start; i <= end; i++)
		if (val[i].chip == p[player])
			count += val[i].amount;
	return( (player==0 && count == player1_chips) || (player==1 && count==player2_chips) );
}

void throwing_chips(vecint dices) // выбрасывание фишек с поля
{
	int pos, x;
	int maxpos; // максимальная позиция( №23 для 1 игрока и №11 для 2 игрока)
	int minpos; // соответсвенно минимальная позинция в доме
	if (player == 0)
	{
		maxpos = 23;
		minpos = 18;
	}
	else
	{
		maxpos = 11;
		minpos = 6;
	}
	while (!dices.empty() && !end_of_game())
	{
		cout << "Выбрасывание фишек." << endl;
		cout<<"Введите позицию с которой хотите произвести действие(отсчет идет с края стола) и количество очков(0 если хотите выбросить фишку) : ";
		cin >> pos;
		cin >> x;
		while (out_of_house(pos,x))
		{
			cout << "Выход за пределы дома, введите позицию и очки(0, если хотите выбросить фишку) заново:   ";
			cin >> pos;
			cin >> x;
		}
		if (x == 0) 
		{
			val[pos].amount--;
			if (player == 0)
				player1_chips --;
			else player2_chips--;
		}
		else
		{ // перестановка
			val[pos + x].chip = val[pos].chip;
			val[pos].amount--;
			val[pos + x].amount++;
		}
		if (val[pos].amount == 0) // удаляем фишку, если ячейка стала пустой
			val[pos].chip.erase();
		system("cls");
		show_field();

		for (int i = 0; i < dices.size(); i++)
			if (x == dices[i] || (x==0 && pos - minpos==dices[i]))
				//если ходим на это количество очков или удаляем с ячейки, равной количеству очков
			{
				dices.erase(dices.begin() + i); // удаляем кубик из массива кубиков
				break;
			}
		if (!dices.empty())
		{
			cout << "У вас остались следующие ходы:   " << endl;
			for (int i = 0; i < dices.size(); i++)
				cout << dices[i] << " ";
			cout << endl;
		}
	}
}

bool turn_is_possible(int pos, int points, bool head_chip, vecint dices) //проверка на дебильность
{
	bool correct_points = false; // 
	if ((val[pos].chip != p[player]) || (head_chip && pos == 12 && player == 1) || (head_chip && pos == 0 && player == 0))
		// если ход чужой фишкой или попытка снять еще одну с головы
	{
		if ((pos == 0 && player == 0) || (pos == 12 && player == 1))
			error_message = "С головы фишку снимать больше нельзя.";
		else
			error_message = "Здесь чужая фишка или пустая ячейка.";
		return false;
	}
	if (val[(pos + points) % 24].chip == p[(player + 1) % 2])
	{
		error_message = "Ход невозможен, здесь чужая фишка";
		return false;
	}
	if (out_of_house(pos, points))
	{
		error_message = "Выходить из дома нельзя";
		return false;
	}

	for (int i = 0; i < dices.size(); i++)
	{
		if (points == dices[i])
		{
			correct_points = true;
			break;
		}
	}
	if (!correct_points)
	{
		error_message = "Вы неверно ввели количество очков";
		return false;
	}
	return true;
} 


void turn()
{
	vecint dices;
	int pos, points;
	bool head_chip = false; //снята ли фишка с головы
	throw_dice();
	dices.push_back(dice1);
	dices.push_back(dice2);
	cout << "Ходит игрок " << player+1 << endl;
		if (dice1 == dice2) // дубль
		{
			cout << "Вам выпал дубль!!!" << endl << endl;
			dices.push_back(dice1);
			dices.push_back(dice2);
		}

		if (chips_in_house()) // если все фишки текущего игрока в доме, переходим к процедуре выброса фишек
			throwing_chips(dices);
		else

		while (!dices.empty())
		{
			if (!jump_of_turn(dices, head_chip))
			{
				cout << "Введите номер ячейки, откуда переставлять и количество очков через пробел:     ";
				cin >> pos >> points;
				while (!turn_is_possible(pos,points,head_chip, dices))
				{
					cout << error_message << endl;
					cout << "Введите номер ячейки и количество очков заново:     ";
					cin >> pos >> points;
				}

				if ((pos == 0 && player == 0) || (pos == 12 && player == 1)) // сняли фишку с головы, больше снимать нельзя
					head_chip = true;

					for (int i = 0; i < dices.size(); i++)
						if (points == dices[i])
						{
							dices.erase(dices.begin() + i); // удаляем кубик из массива кубиков
							break;
						}
				{ // перемещение фишки
					val[(pos + points) % 24].chip = val[pos].chip;
					val[pos].amount--;
					if (val[pos].amount == 0)
						val[pos].chip.erase(); // удаляем фишку, если ячейка стала пустой
					val[(pos + points) % 24].amount++;

					system("cls");
					show_field();

					if (!dices.empty())
					{
						cout << "У вас остались следующие ходы:   " << endl;
						for (int i = 0; i < dices.size(); i++)
							cout << dices[i] << " ";
					}
					cout << endl;
				}
			}
			else
			{
				system("cls");
				show_field();
				cout << "Вы не можете ходить, ход переходит сопернику" << endl;
				system("PAUSE"); 
				break;
			}
		}
	head_chip = false;
	player = (player+1) %2; //переход хода другому игроку
}
int main()
{
	setlocale(LC_ALL, "Rus");
	init();
	show_field();
	while (!end_of_game())
		turn();
	cout << "Победил игрок " << player+1 << " !!!"<<endl;
	system("pause");
	return 0;
}