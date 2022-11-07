#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning (disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define COL 50
#define ROW 30 

#define EMPTY 0 
#define WALL 1 
#define FOOD 2 
#define HEAD 3 
#define BODY 4 

#define UP 72 
#define DOWN 80 
#define LEFT 75 
#define RIGHT 77 

void HideCursor();
void GotoXY(int x, int y);
void Set_Color(int color);
void Init_UI();
void Init_Snake();
void Draw_Snake();
void Draw_Food();
void Move_Snake();
void Snake_Status();
void Snake_Action();
void Snake_Direction(int Direction);

struct Snake
{
	int length;
	int x;
	int y;
}snake,snake_next;

struct Body
{
	int x;
	int y;
}body[ROW * COL];

struct Tail
{
	int x;
	int y;
}tail[1];

int status[COL][ROW];
int Direction = RIGHT;
int MAX, GRADE;
int Flag = 0;
int Food = 0;
int i, j;

void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	curInfo.dwSize = 1;
	curInfo.bVisible = FALSE;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &curInfo); 
}

void GotoXY(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
}

void Set_Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Init_UI()
{
	system("mode con cols=100 lines=30");
	Set_Color(11);
	for(int i = 0; i < COL; i++)
	{
		for (int j = 0; j < ROW; j++)
		{
			if (i == 0 || i == COL - 1 || j == 0 || j == ROW - 1)
			{
				GotoXY(2 * i, j);
				printf("¡ö");
				status[i][j] = WALL;
			}
		}
	}
}

void Init_Snake()
{
	snake.x = COL / 2, snake.y = ROW / 2, snake.length = 1;
	body[0].x = COL / 2 - 1, body[0].y = ROW / 2;
	tail[0].x = 0, tail[0].y = 0;
	status[snake.x][snake.y] = HEAD;
	status[body[0].x][body[0].y] = BODY;
}

void Draw_Snake()
{
	Set_Color(10);
	GotoXY(2 * snake.x, snake.y);
	printf("¡ö");
	for (int i = 0; i < snake.length; i++)
	{
		if (status[body[i].x][body[i].y] == BODY)
		{
			GotoXY(2 * body[i].x, body[i].y);
			printf("¡õ");
		}
	}
	if (status[tail[0].x][tail[0].y] == EMPTY && tail[0].x != 0 && tail[0].y != 0)
	{
		GotoXY(2 * tail[0].x, tail[0].y);
		printf("  ");
	}
		Flag = 0;
}

void Draw_Food()
{	
	if (Food == 0)
	{
		srand((unsigned)time(NULL));
		do
		{
			i = rand() % COL;
			j = rand() % ROW;

		}while(status[i][j] != EMPTY);
		Food = 1;
		status[i][j] = FOOD;
		Set_Color(12);
		GotoXY(2 * i, j);
		printf("¡ñ");
	}
}

void Snake_Action()
{
	int keypad;
	if (kbhit())
	{
		keypad = getch();
		switch (keypad)
		{
			case UP:
				if (Direction == LEFT || Direction == RIGHT)
				{
					snake_next.x = snake.x;
					snake_next.y = snake.y - 1;
					Direction = UP;
				}
				else
					Snake_Direction(Direction);
				break;
			case DOWN:
				if (Direction == LEFT || Direction == RIGHT)
				{
					snake_next.x = snake.x;
					snake_next.y = snake.y + 1;
					Direction = DOWN;
				}
				else
					Snake_Direction(Direction);
				break;
			case LEFT:
				if (Direction == UP || Direction == DOWN)
				{
					snake_next.x = snake.x - 1;
					snake_next.y = snake.y;
					Direction = LEFT;
				}
				else
					Snake_Direction(Direction);
				break;
			case RIGHT:
				if (Direction == UP || Direction == DOWN)
				{
					snake_next.x = snake.x + 1;
					snake_next.y = snake.y;
					Direction = RIGHT;
				}
				else
					Snake_Direction(Direction);
				break;
			default :
				break;
		}
	}
	else
	{
		Snake_Direction(Direction);
	}
}

void Snake_Direction(int Direction)
{
	switch (Direction)
	{
	case UP:
		snake_next.x = snake.x;
		snake_next.y = snake.y - 1;
		break;
	case DOWN:
		snake_next.x = snake.x;
		snake_next.y = snake.y + 1;
		break;
	case LEFT:
		snake_next.x = snake.x - 1;
		snake_next.y = snake.y;
		break;
	case RIGHT:
		snake_next.x = snake.x + 1;
		snake_next.y = snake.y;
	}
}

void Move_Snake()
{	
	tail[0].x = body[snake.length - 1].x, tail[0].y = body[snake.length - 1].y;
	for (int i = snake.length -1; i > 0; i--)
	{
		body[i].x = body[i - 1].x;
		body[i].y = body[i - 1].y;
		status[body[i].x][body[i].y] = BODY;
	}
	body[0].x = snake.x;
	body[0].y = snake.y;
	status[body[0].x][body[0].y] = BODY;
	snake.x = snake_next.x;
	snake.y = snake_next.y;

	if (Flag == 0)
	{
		status[tail[0].x][tail[0].y] = EMPTY;
	}
}

void Snake_Status()
{
	if (status[snake_next.x][snake_next.y] == FOOD)
	{
		Flag = 1;
		Food = 0;
		snake.length++;
	}
	else if (status[snake_next.x][snake_next.y] == BODY || status[snake_next.x][snake_next.y] == WALL)
	{
		system("cls");
		GotoXY(COL, ROW / 2);                                
		printf("game over");
		exit(0);
	}
}

int main()
{
	HideCursor();
	Init_UI();
	Init_Snake();
	while (1)
	{
		Snake_Action();
		Snake_Status();
		Move_Snake();
		Draw_Snake();
		Draw_Food();
		Sleep(200);
	}
	return 0;
}