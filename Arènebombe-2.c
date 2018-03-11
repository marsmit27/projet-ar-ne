#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windows.h>

#define SIZE 10
#define RANDOM 100

typedef struct s_coord {
	int x;
	int y;
} coord;

typedef struct s_bombe {
	int tour;
	coord pos;
} bombe;

typedef struct s_joueur {
	coord pos;
	int score;
	int alive;
	bombe bombes[3];

} joueur;

int **create_map()
{
	int **map;

	map = malloc(sizeof(int *) * SIZE);
	for (int i = 0; i < SIZE; i++)
		map[i] = malloc(sizeof(int) * SIZE);
	return (map);
}

int **fill_map(int **map)
{
	int i = 0;
	int j;

	while (i < SIZE)
	{
		j = 0;
		while (j < SIZE)
		{
			map[i][j] = rand() % RANDOM;
			j++;
		}
		i++;
	}
	return (map);
}

void free_map(int **map)
{
	int i = 0;

	while (i < SIZE)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

void display_map(int **map)
{
	int i;
	int j;
	int k;

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
	for (i = 0; i < SIZE; i++) {
		for (int l = 0; l < SIZE * 5 + 1; l++)
			printf("-");
		printf("\n");
		for (k = 0; k < SIZE; k++) {
			printf("|    ");
		}
		printf("|\n");
		for (j = 0; j < SIZE; j++) {
			printf("| %02d ", map[i][j]);

		}
		printf("|\n");
		for (k = 0; k < SIZE; k++) {
			printf("|    ");
		}
		printf("|\n");
	}
	for (int l = 0; l < SIZE * 5 + 1; l++)
		printf("-");
	printf("\n\n");
}

joueur init_joueur()
{
	joueur ret;

	ret.pos.x = rand() % SIZE;
	ret.pos.y = rand() % SIZE;
	ret.score = 0;
	ret.alive = 1;
	for (int i = 0; i < 3; i++)
		ret.bombes[i].tour = -1;
	return (ret);
}

int check_end_game(joueur j1, joueur j2, int **map)
{
	int alive;
	int sum = 0;

	alive = j1.alive + j2.alive;
	if (alive < 2)
		return (0);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			sum += map[i][j];
		}
	}
	if (sum > 0)
		return (1);
	return (0);
}

void bomb(joueur *j1, joueur *j2)
{
	int i;

	for (i = 0; j1->bombes[i].tour != -1 && i < 3; i++) {
		j1->bombes[i].tour -= 1;
		if (j1->bombes[i].tour == 0) {
			for (int x = -1; x < 2; x++) {
				for (int y = -1; y < 2; y++) {
					if (x == j2->pos.x && y == j2->pos.y) {
						j2->alive = 0;
					}
				}
			}
		}
	}
}

int **ia_random(joueur *j, int **map, joueur *other)
{
	int i = 0;
	int x, y;

	j->score += map[j->pos.x][j->pos.y];
	map[j->pos.x][j->pos.y] = 0;
	while (j->bombes[i].tour != -1 && i < 3)
		i++;
	j->bombes[i].tour = 3;
	j->bombes[i].pos.x = j->pos.x;
	j->bombes[i].pos.y = j->pos.y;
	x = rand() % SIZE;
	y = rand() % SIZE;
	while (x == other->pos.x && y == other->pos.x) {
		x = rand() % SIZE;
		y = rand() % SIZE;
	}
	j->pos.x = x;
	j->pos.y = y;
	bomb(j, other);
	return (map);
}

int **ia(joueur *j, int **map, joueur *other)
{
	int i = 0;
	int k;
	int tmp = map[0][0];
	int x_tmp = 0, y_tmp = 0;

	j->score += map[j->pos.x][j->pos.y];
	map[j->pos.x][j->pos.y] = 0;
	while (j->bombes[i].tour != -1 && i < 3)
		i++;
	j->bombes[i].tour = 3;
	j->bombes[i].pos.x = j->pos.x;
	j->bombes[i].pos.y = j->pos.y;
	for (i = 0; i < SIZE; i++) {
		for (k = 0; k < SIZE; k++) {
			if (map[i][k] >= tmp) {
				x_tmp = i;
				y_tmp = k;
				tmp = map[i][k];
			}
		}
	}
	j->pos.x = x_tmp;
	j->pos.y = y_tmp;
	bomb(j, other);
	return (map);
}

void print_end(joueur joueur1, joueur joueur2)
{
	int win;

	if (joueur1.alive == 0 || joueur2.alive == 0) {
		if (joueur1.alive == 0)
			win = 2;
		else
			win = 1;

		printf("Player 2 wins with %d points, player 1 dead with %d points\n",
            win, joueur2.score, joueur1.score);
	}
	else {
		if (joueur1.score < joueur2.score)
			win = 2;
		else
			win = 1;
		printf("Player %d wins with %d points, player 1 loses with %d points\n",
			win, joueur2.score, joueur1.score);
	}
}

int main()
{
	int ret;
	int **map;
	joueur joueur1;
	joueur joueur2;

	srand(time(NULL));
	map = create_map();
	map = fill_map(map);
	joueur1 = init_joueur();
	joueur2 = init_joueur();
	while (check_end_game(joueur1, joueur2, map)) {
		map = ia_random(&joueur1, map, &joueur2);
		map = ia(&joueur2, map, &joueur1);
		display_map(map);
		printf("Scores: j1 = %d ; j2 = %d;\n", joueur1.score, joueur2.score);
		Sleep(1);
	}
	print_end(joueur1, joueur2);
	free_map(map);
	return (ret);
}
