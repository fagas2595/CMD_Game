#include <stdio.h>
#include <stdlib.h>                                                                                      
#include <time.h>                                                                                                                                                                                                                                                              
#include <windows.h> 
#include <malloc.h>
#include <conio.h>

#define n 5

typedef struct Player
{
	int X = 0;
	int Y = 4;
	int HP = 10;
	int DMG = 5;
} Player;

typedef struct Enemy
{
	int Index;
	int X;
	int Y;
	int HP = 15;
	int DMG = 2;
} Enemy;

void Dungeons();
void FirstFloor(char map[][n]);
void UpdateMap(char map[][n], Player link, Enemy *ganon);
int MovePlayer(char map[][n], int row, int column, char movement);
void MoveEnemy(char map[][n], Enemy* ganon, Player link);
void BattleSystem(Player link, Enemy* ganon, int index);

int main()
{
	srand(time(0));
	Dungeons();
	system("pause");
	return 0;
}

void Dungeons()
{
	/* Creation of the Map */
	char map[n][n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			map[i][j] = ' ';

	FirstFloor(map);
}
void FirstFloor(char map[][n])
{
	Enemy* ganon = (Enemy*)malloc(sizeof(Enemy) * 2);
	Player link;// = { 0, 4, 10, 5 };
	char movement = ' ';
	int x;
	int y;

	/* Spawn Walls */
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (((i == 1 || i == 3) && j == 3) || i == 3 && j == 1)
				map[i][j] = '#';
		}

	/* Define Enemys location */
	ganon[0] = { 0, 3, 0 };
	ganon[1] = { 1, 4, 2 };
	
	for (int i = 0; i < 2; i++)
		map[ganon[i].Y][ganon[i].X] = 'S';

	/* Gameplay */
	while (link.Y < n && link.X < n)
	{
		system("cls");
		UpdateMap(map, link, ganon);

		/* Movements */
		do
		{
			x = link.X;
			y = link.Y;

			movement = (char)tolower(_getch());
			(movement == 'w' || movement == 's' ? link.Y : link.X) = MovePlayer(map, link.Y, link.X, movement);
			if (link.X == ganon->X || link.Y == ganon->Y)
			{
				link.X = x;
				link.Y = y;
				BattleSystem(link, ganon, ganon->Index);
			}
			MoveEnemy(map, ganon, link);
		} while (link.Y < 0 || link.X < 0);
	}
	free(ganon);
}
void UpdateMap(char map[][n], Player link, Enemy *ganon)
{
	map[link.Y][link.X] = 'P';
	
	printf("\n| Player Hp -> %d |\n\n", link.HP);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			printf("[%c]", map[i][j]);
		printf("\n");
	}
	printf("\n[P -> Player]\t[# -> Wall][0-9 -> enemys]\n");
	printf("| Hp %d -> %d | Hp %d -> %d |\n", ganon[0].Index, ganon[0].HP, ganon[1].Index, ganon[1].HP);
	printf("[W -> Up]\t[S -> Down]\t[A -> Left]\t[D -> Right]\n\n");
}
int MovePlayer(char map[][n], int row, int column, char movement)
{
	map[row][column] = ' ';
	switch (movement){
	case 'w':
		if (row > 0 && map[row - 1][column] != '#')
			row--;
		break;
	case 's':
		if (row < 4 && map[row + 1][column] != '#')
			row++;
		break;
	case 'a':
		if (column > 0 && map[row][column - 1] != '#')
			column--;
		break;
	case 'd':
		if (column < 4 && map[row][column + 1] != '#')
			column++;
	}
	return movement == 'w' || movement == 's' ? row : column;
}
void MoveEnemy(char map[][n], Enemy * ganon, Player link)
{
	int symbol = 0;
	int index = 0;
	int x;
	int y;

	do {
		map[ganon[index].Y][ganon[index].X] = ' ';
		x = ganon[index].X;
		y = ganon[index].Y;
		
		if (ganon[index].Y == link.Y) {
			ganon[index].X > link.X ? ganon[index].X-- : ganon[index].X++;
		}
		else if (ganon[index].X == link.X){
			ganon[index].Y > link.Y ? ganon[index].Y-- : ganon[index].Y++;
		}
		else{
			symbol = rand() % 4;
			switch (symbol) {
			case 0:
				if (ganon[index].Y < 4)
					++ganon[index].Y;
				break;
			case 1:
				if (ganon[index].Y > 0)
					--ganon[index].Y;
				break;
			case 2:
				if (ganon[index].X < 4)
					++ganon[index].X;
				break;
			case 3:
				if (ganon[index].X > 0)
					--ganon[index].X;
			}
		}
		if (map[ganon[index].Y][ganon[index].X] == '#' || map[ganon[index].Y][ganon[index].X] == ganon->Index){
			ganon[index].X = x;
			ganon[index].Y = y;
		}
		map[ganon[index].Y][ganon[index].X] = 'S';
		index++;
	} while (index < 2);
}
void BattleSystem(Player link, Enemy *ganon, int index)
{
	int chance;
	chance = rand() % 2;
	if (chance == 0)
		ganon[index].HP - link.DMG;
	
	chance = rand() % 2;
	if (chance == 1)
		link.HP - ganon[index].DMG;
}