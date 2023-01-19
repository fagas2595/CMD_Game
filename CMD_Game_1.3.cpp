#include <stdio.h>
#include <stdlib.h>                                                                                      
#include <time.h>                                                                                                                                                                                                                                                              
#include <windows.h> 
#include <malloc.h>
#include <conio.h>

#define MAP_SIZE 5

typedef struct
{
	int HP;
	int DMG;
}Stats;

typedef struct
{
	int X;
	int Y;
}Position;

typedef struct
{
	Position post;
	Stats stats;
} Player;

typedef struct Enemy
{
	Position post;
	Stats stats;
	Enemy *next;
} Enemy;

void Dungeons();
void FirstFloor(char map[][MAP_SIZE]);
void UpdateMap(char map[][MAP_SIZE], Player player, Enemy fEnemy, Enemy sEnemy);
int MovePlayer(char map[][MAP_SIZE], int row, int column, char movement);
void BattleSystem(Player player, Enemy* enemy);
//void MoveEnemy(char map[][MAP_SIZE], Enemy* enemy, Player player);

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
	char map[MAP_SIZE][MAP_SIZE];
	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++)
			map[i][j] = ' ';

	FirstFloor(map);
}
void FirstFloor(char map[][MAP_SIZE])
{
	Player link = {0, 4, 10, 5};
	char movement = ' ';
	int x;
	int y;

	/* Spawn Walls */
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (((i == 1 || i == 3) && j == 3) || i == 3 && j == 1){
					map[i][j] = '#';
			}
		}
	}

	/* Define Enemys location and stats */
	Enemy fEnemy = { 3, 0, 15, 2 };
	Enemy sEnemy = { 4, 2, 15, 2 };
	
	fEnemy.next = &sEnemy;
	Enemy* point = &fEnemy;

	while (point != NULL) {
		map[point->post.Y][point->post.X] = 'S';
		point = point->next;
	}
	
	/*map[fEnemy.post.Y][fEnemy.post.X] = 'S';
	map[sEnemy.post.Y][sEnemy.post.X] = 'S';*/

	/* Gameplay */
	while (link.post.Y < MAP_SIZE && link.post.X < MAP_SIZE)
	{
		system("cls");
		UpdateMap(map, link, fEnemy, sEnemy);

		/* Movements */
		do
		{
			point = &fEnemy;
			x = link.post.X;
			y = link.post.Y;

			movement = (char)tolower(_getch());
			(movement == 'w' || movement == 's' ? link.post.Y : link.post.X) = MovePlayer(map, link.post.Y, link.post.X, movement);
			while (point != NULL) {
				if (link.post.X == point->post.X && link.post.Y == point->post.Y)
				{
					link.post.X = x;
					link.post.Y = y;
					BattleSystem(link, point);
				}
				point = point->next;
			}
			//MoveEnemy(map, , link);
		} while (link.post.Y < 0 || link.post.X < 0);
	}
}
void UpdateMap(char map[][MAP_SIZE], Player player, Enemy fEnemy, Enemy sEnemy)
{
	map[player.post.Y][player.post.X] = 'P';
	
	printf("\n| Player Hp -> %d |\n\n", player.stats.HP);

	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++) {
			printf("[%c]", map[i][j]);
		}
		printf("\n");
	}
	printf("\n[P -> Player]\t[# -> Wall][S -> Spider]\n");
	printf("| Hp 1 -> %d | HP 2 -> %d |\n", fEnemy.stats.HP, sEnemy.stats.HP);
	printf("[W -> Up]\t[S -> Down]\t[A -> Left]\t[D -> Right]\n\n");
}
int MovePlayer(char map[][MAP_SIZE], int row, int column, char movement)
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
/*void MoveEnemy(char map[][MAP_SIZE], Enemy* enemy, Player player)
{
	int symbol = 0;
	int index = 0;
	int x;
	int y;

	do {
		map[enemy[index].Y][enemy[index].X] = ' ';
		x = enemy[index].X;
		y = enemy[index].Y;
		
		if (enemy[index].Y == player.Y) {
			enemy[index].X > player.X ? enemy[index].X-- : enemy[index].X++;
		}
		else if (enemy[index].X == player.X){
			enemy[index].Y > player.Y ? enemy[index].Y-- : enemy[index].Y++;
		}
		else{
			symbol = rand() % 4;
			switch (symbol) {
			case 0:
				if (enemy[index].Y < 4)
					++enemy[index].Y;
				break;
			case 1:
				if (enemy[index].Y > 0)
					--enemy[index].Y;
				break;
			case 2:
				if (enemy[index].X < 4)
					++enemy[index].X;
				break;
			case 3:
				if (enemy[index].X > 0)
					--enemy[index].X;
			}
		}
		if (map[enemy[index].Y][enemy[index].X] == '#' || map[enemy[index].Y][enemy[index].X] == enemy->Id){
			enemy[index].X = x;
			enemy[index].Y = y;
		}
		map[enemy[index].Y][enemy[index].X] = enemy->Id;
		index++;
	} while (index < 2);
}
void BattleSystem(Player player, Enemy *enemy, int index)
{
	int chance;
	chance = rand() % 2;
	if (chance == 0)
		enemy[index].HP - player.DMG;
	
	chance = rand() % 2;
	if (chance == 1)
		player.HP - enemy[index].DMG;
}*/
void BattleSystem(Player player, Enemy *enemy)
{
	int chance;
	chance = rand() % 2;
	if (chance == 0) { enemy->stats.HP -= player.stats.DMG; }
	chance = rand() % 2;
	if (chance == 1) { player.stats.HP -= enemy->stats.DMG; }
}