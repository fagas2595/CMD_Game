#include <stdio.h>
#include <stdlib.h>                                                                                      
#include <time.h>                                                                                                                                                                                                                                                              
#include <windows.h> 
#include <malloc.h>
#include <conio.h>

#define MAP_SIZE 5

typedef struct
{
	int X;
	int Y;
}Position;

typedef struct Stats
{
public:
	int DMG;
	int HP;
	int MaxHP;
	int LVL;
	int XP;
	int MaxXP;
}Stats;

typedef struct
{
	int foodCount;
	Position post;
	Stats stats;
} Player;

typedef struct Enemy
{
	Position post;
	Stats stats;
	Enemy* next;
} Enemy;

void Dungeons();
void FirstFloor(char map[][MAP_SIZE]);
void UpdateMap(char map[][MAP_SIZE], Player player, Enemy fEnemy, Enemy sEnemy);
int MovePlayer(char map[][MAP_SIZE], Player *player, char movement);
void BattleSystem(Player* player, Enemy* enemy);
int DeathCheck(char map[][MAP_SIZE], Player* player, Enemy* enemy);
void MoveEnemy(char map[][MAP_SIZE], Enemy* enemy, Player *player);
void DeadPlayer();
void Eating(Player* player);
void LvlUp(Player* player);

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
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			map[i][j] = '.';
		}
	}
	FirstFloor(map);
}
void FirstFloor(char map[][MAP_SIZE])
{
	Player link = { 1, 0, 4, 5, 10, 10, 1, 0, 2 };
	Player* pointOnPlayer = &link;
	Enemy* pointOnEnemy;
	char movement = '.';
	int x;
	int y;
	int playerDeath = 0;

	/* Spawn Walls */
	map[1][3] = '#';
	map[3][3] = '#';
	map[3][1] = '#';

	/* Define Enemys location and stats */
	Enemy fEnemy = { 3, 0, 2, 15 };
	Enemy sEnemy = { 4, 2, 2, 15 };

	fEnemy.next = &sEnemy;

	map[fEnemy.post.Y][fEnemy.post.X] = 'S';
	map[sEnemy.post.Y][sEnemy.post.X] = 'S';

	/* Gameplay */
	while (link.post.Y < MAP_SIZE && link.post.X < MAP_SIZE && playerDeath == 0)
	{
		system("cls");
		UpdateMap(map, link, fEnemy, sEnemy);

		/* Movements */
		do
		{
			pointOnEnemy = &fEnemy;
			x = link.post.X;
			y = link.post.Y;

			movement = (char)tolower(_getch());
			(movement == 'w' || movement == 's' ? link.post.Y : link.post.X) = MovePlayer(map, pointOnPlayer, movement);
			if (movement == 'e') { Eating(pointOnPlayer); }
			while (pointOnEnemy != NULL) {
				if (link.post.X == pointOnEnemy->post.X && link.post.Y == pointOnEnemy->post.Y) {
					link.post.X = x;
					link.post.Y = y;
					BattleSystem(pointOnPlayer, pointOnEnemy);
					playerDeath = DeathCheck(map, pointOnPlayer, pointOnEnemy);
				}
				MoveEnemy(map, pointOnEnemy, pointOnPlayer);
				pointOnEnemy = pointOnEnemy->next;
			}
			if (playerDeath == 1) { break; }
		} while (link.post.Y < 0 || link.post.X < 0);
	}
}
void UpdateMap(char map[][MAP_SIZE], Player player, Enemy fEnemy, Enemy sEnemy)
{
	map[player.post.Y][player.post.X] = 'P';
	
	
	
	printf("\n| Player Hp -> %d |\n\n", player.stats.HP);

	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (map[i][j] == 'P') { printf("\033[0;32m"); }
			else if ( map[i][j] == 'S') { printf("\033[1;31m"); }
			else { printf("\033[0m"); }
			printf("%2c", map[i][j]);
		}
		printf("\n");
	}
	printf("\n[P -> Player]\t[# -> Wall][S -> Spider]\n");
	printf("| Hp 1 -> %d | HP 2 -> %d |\n", fEnemy.stats.HP, sEnemy.stats.HP);
	printf("[W -> Up]\t[S -> Down]\t[A -> Left]\t[D -> Right]\n\n");
}
int MovePlayer(char map[][MAP_SIZE], Player *player, char movement)
{
	map[player->post.Y][player->post.X] = '.';
	switch (movement) {
	case 'w':
		if (player->post.Y > 0 && map[player->post.Y - 1][player->post.X] != '#')
			player->post.Y--;
		break;
	case 's':
		if (player->post.Y < 4 && map[player->post.Y + 1][player->post.X] != '#')
			player->post.Y++;
		break;
	case 'a':
		if (player->post.X > 0 && map[player->post.Y][player->post.X - 1] != '#')
			player->post.X--;
		break;
	case 'd':
		if (player->post.X < 4 && map[player->post.Y][player->post.X + 1] != '#')
			player->post.X++;
	}
	return movement == 'w' || movement == 's' ? player->post.Y : player->post.X;
}
void MoveEnemy(char map[][MAP_SIZE], Enemy* enemy, Player* player)
{
	int symbol;
	map[enemy->post.Y][enemy->post.X] = '.';

	if (enemy->post.Y == player->post.Y) {
		if (enemy->post.X > 0 && enemy->post.X > player->post.X && map[enemy->post.Y][enemy->post.X-1] == '.') {
			enemy->post.X--;
		}
		else if (enemy->post.X < 4 && enemy->post.X < player->post.X && map[enemy->post.Y][enemy->post.X + 1] == '.') {
			enemy->post.X++;
		}
	}
	else if (enemy->post.X == player->post.X) {
		if (enemy->post.Y > 0 && enemy->post.Y > player->post.Y && map[enemy->post.Y - 1][enemy->post.X] == '.') {
			enemy->post.Y--;
		}
		else if (enemy->post.Y < 4 && enemy->post.Y < player->post.Y && map[enemy->post.Y + 1][enemy->post.X] == '.') {
			enemy->post.Y++;
		}
	}
	else {
		symbol = rand() % 4;
		switch (symbol) {
		case 0:
			if (enemy->post.Y < 4 && map[enemy->post.Y + 1][enemy->post.X] == '.')
				++enemy->post.Y;
			break;
		case 1:
			if (enemy->post.Y > 0 && map[enemy->post.Y - 1][enemy->post.X] == '.')
				--enemy->post.Y;
			break;
		case 2:
			if (enemy->post.X < 4 && map[enemy->post.Y][enemy->post.X + 1] == '.')
				++enemy->post.X;
			break;
		case 3:
			if (enemy->post.X > 0 && map[enemy->post.Y][enemy->post.X - 1] != '.')
				--enemy->post.X;
		}
	}
	map[enemy->post.Y][enemy->post.X] = 'S';
}
void BattleSystem(Player* player, Enemy* enemy)
{
	int chance = rand() % 101;
	if (chance <= 50) { enemy->stats.HP -= player->stats.DMG; }
	chance = rand() % 101;
	if (chance >= 50) { player->stats.HP -= enemy->stats.DMG; }
}
int DeathCheck(char map[][MAP_SIZE], Player* player, Enemy* enemy)
{
	int playerDeath = 0;
	if (enemy->stats.HP <= 0) {
		map[enemy->post.Y][enemy->post.X] = '.';
		enemy->post.Y = 100;
		enemy->post.X = 100;
		player->stats.XP++;
		if (player->stats.XP == player->stats.MaxXP) { LvlUp(player); }
	}
	if (player->stats.HP <= 0) {
		DeadPlayer();
		playerDeath = 1;
	}
	return playerDeath;
}
void DeadPlayer()
{
	system("cls");
	printf("\033[1;31m");
	printf("You Died\n");
	printf("\033[0m");
}
void Eating(Player* player)
{
	if (player->foodCount > 0) {
		player->foodCount--;
		if (player->stats.HP + 5 <= player->stats.MaxHP){ player->stats.HP += 5; }
		else { player->stats.HP += player->stats.MaxHP - player->stats.HP; }
	}
}
void LvlUp(Player* player)
{
	player->stats.LVL++;
	player->stats.XP = 0;
	player->stats.MaxXP += 2;
	player->stats.HP += 4;
	player->stats.MaxHP += 4;
}