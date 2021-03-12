// C_Project_2021_01_20_00.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define KNIGHT	1
#define MAGE	2
#define THEIF	3
#define MAX	10

#define MAP_WIDTH	20
#define MAP_HEIGHT	20

#define ROAD	0
#define WALL	1
#define GEM		2
#define MONSTER	3

#define UP		1
#define DOWN	2
#define LEFT	3
#define RIGHT	4
//#define EXIT	0

#define SCISSORS	0
#define ROCK		1
#define PAPER		2
#define EXIT		-1

#define WIN		0
#define LOSE	1
#define DRAW	2

#define LEFT	75
#define RIGHT	77
#define UP		72
#define DOWN	80

#define ESC		27
#define SPACE	32

float secTime;

clock_t start, end;

enum CURSOR_TYPE { NOCURSOR, SOLIDCURSOR, NORMALCURSOR };
void gotoxy(int x, int y)  //goto xy 함수
{
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setcursortype(CURSOR_TYPE c) //커서 숨기는 함수
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c)
	{
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
void gameMenual()
{
	//게임설명서 출력
	gotoxy(25, 2); printf("KMJ FindingCrystalGame 설명서");
	gotoxy(25, 4); printf("1. 방향키를 입력하면 시작지점의 '♠'이 출발합니다.");
	gotoxy(25, 5); printf("2. 상 하 좌 우를 이용하여 보석을 찾으면 게임이 끝납니다.");
	gotoxy(25, 6); printf("3. 단 벽은 통과할수 없습니다.");
	gotoxy(25, 6); printf("4. '◎' 몬스터와 만난다면 도망치세요.");
	gotoxy(25, 7); printf("5. 나가기는 ESC버튼 입니다.");
}
int main()
{
	int key = 0;
	int select = -1;
	int playerX = 0;
	int playerY = 0;

	setcursortype(NOCURSOR);

	int map[MAP_HEIGHT][MAP_WIDTH] = {
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
		{ 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 2, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, }
	};

	gameMenual();

	while (true)
	{
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				gotoxy(j, i); //커서를 지정하여 빠르게 갱신시킬수 있다. (cls, sleep 불필요)
				if (map[i][j] == ROAD)
				{
					if (playerY == i && playerX == j)
					{
						printf("♠");
					}
					else
					{
						printf("  ");
					}
				}
				else if (map[i][j] == WALL)
				{
					printf("▧");
				}
				else if (map[i][j] == GEM)
				{
					printf("◈");
				}
				else if (map[i][j] == MONSTER)
				{
					printf("◎");
				}
			}			
		}
		select = -1;
		if (kbhit())
		{
			do
			{
				key = getch();
			} while (key == 224);

			select = key;
			switch (key)
			{
			case ESC:
				exit(0);
				break;
			default:
				break;
			}
		}

		

		switch (select)
		{
		case UP:
			if (playerY - 1 >= 0)
			{
				if (map[playerY - 1][playerX] == ROAD)
				{
					playerY--;
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
				}
				else if (map[playerY - 1][playerX] == WALL)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
				}
				else if (map[playerY - 1][playerX] == GEM)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("보석을 찾았습니다.\n");
					playerX = 0;
					playerY = 0;
				}
			}
			else
			{
				gotoxy(0, MAP_HEIGHT); printf("                      \n");
				gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
			}
			break;
		case DOWN:
			if (playerY + 1 <= MAP_HEIGHT - 1)
			{
				if (map[playerY + 1][playerX] == ROAD)
				{
					playerY++;
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
				}
				else if (map[playerY + 1][playerX] == WALL)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
				}
				else if (map[playerY + 1][playerX] == GEM)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("보석을 찾았습니다.\n");
					playerX = 0;
					playerY = 0;
				}
			}
			else
			{
				gotoxy(0, MAP_HEIGHT); printf("                      \n");
				gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
			}
			break;
		case LEFT:
			if (playerX - 1 >= 0)
			{
				if (map[playerY][playerX - 1] == ROAD)
				{
					playerX--;
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
				}
				else if (map[playerY][playerX - 1] == WALL)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
				}
				else if (map[playerY][playerX - 1] == GEM)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("보석을 찾았습니다.\n");
					playerX = 0;
					playerY = 0;
				}
			}
			else
			{
				gotoxy(0, MAP_HEIGHT); printf("                      \n");
				gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
			}
			break;
		case RIGHT:
			if (playerX + 1 <= MAP_WIDTH - 1)
			{
				if (map[playerY][playerX + 1] == ROAD)
				{
					playerX++;
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
				}
				else if (map[playerY][playerX + 1] == WALL)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
				}
				else if (map[playerY][playerX + 1] == GEM)
				{
					gotoxy(0, MAP_HEIGHT); printf("                      \n");
					gotoxy(0, MAP_HEIGHT); printf("보석을 찾았습니다.\n");
					playerX = 0;
					playerY = 0;
				}
			}
			else
			{
				gotoxy(0, MAP_HEIGHT); printf("                      \n");
				gotoxy(0, MAP_HEIGHT); printf("이동 할 수 없습니다.\n");
			}
			break;
		case ESC:
			exit(0);
			break;
		default:
			if (select != -1)
			{
				gotoxy(0, MAP_HEIGHT); printf("                      \n");
				gotoxy(0, MAP_HEIGHT); printf("다시 입력해주세요!\n");
			}
			break;
		}
	}

}
// MONSTER = 3 인데 이걸 일정 패턴으로움직일수 있게 하기
// 몬스터와 닿으면 게임 종료//////
/////

