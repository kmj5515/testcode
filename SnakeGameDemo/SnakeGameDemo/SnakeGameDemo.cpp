// Project1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define MAP_X 19
#define MAP_Y 17
#define MAX_COUNT 7
#define EXIT -1

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
#define ESC 27

#define MAX_NUM 20
int num[MAX_NUM];
int length;
int dir;
int speed;
int snakeX[255];
int snakeY[255];
int foodX;
int foodY;
int key;
int iSstart;
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
void BubbleSort()
{
	int temp = 0;
	for (int i = 0; i < MAX_NUM - 1; i++)
	{
		for (int j = 0; j < MAX_NUM - i - 1; j++)
		{
			if (num[j] > num[j + 1])
			{
				temp = num[j];
				num[j] = num[j + 1];
				num[j + 1] = temp;
			}
		}
	}
}
void ClearMap()
{
	for (int i = 1; i < MAP_Y - 1; i++)
	{
		for (int j = 0; j < MAP_X - 1; j++)
		{
			gotoxy(j, i); printf("  ");
		}
	}
}
void DrawMap()
{
	for (int i = 0; i < MAP_X; i++)
	{
		gotoxy(i, 0); printf("■");
		gotoxy(i, MAP_Y - 1); printf("■");
	}
	for (int i = 1; i < MAP_Y - 1; i++)
	{
		gotoxy(0, i); printf("■");
		gotoxy(MAP_X - 1, i); printf("■");
	}
}
void Initalized()
{
	length = 4;
	dir = RIGHT;
	speed = 200;  // 뱀의 이동속도
	iSstart = false;

	foodX = MAP_X / 2 + 4;
	foodY = MAP_Y / 2;

	gotoxy(foodX, foodY); printf("♥");
	for (int i = 0; i < length; i++)
	{
		if (i == 0)
		{
			snakeX[0] = MAP_X / 2 - 4;
			snakeY[0] = MAP_Y / 2;
			gotoxy(snakeX[0], snakeY[0]); printf("◈");
		}
		else
		{
			snakeX[i] = snakeX[0] - i;
			snakeY[i] = snakeY[0];
			gotoxy(snakeX[i], snakeY[i]); printf("●");
		}
	}
}
void DrawSnake()
{
	if (iSstart)
	{
		gotoxy(snakeX[0], snakeY[0]); printf("●");
		gotoxy(snakeX[length - 1], snakeY[length - 1]); printf("  ");

		for (int i = length; i > 0; i--)
		{
			snakeX[i] = snakeX[i - 1];
			snakeY[i] = snakeY[i - 1];
		}

		switch (dir)
		{
		case LEFT:
			snakeX[0]--;
			break;
		case RIGHT:
			snakeX[0]++;
			break;
		case UP:
			snakeY[0]--;
			break;
		case DOWN:
			snakeY[0]++;
			break;
		}
		gotoxy(snakeX[0], snakeY[0]); printf("◈");
	}
}
void KeyProecess()
{
	if (kbhit())
	{
		iSstart = true;
		gotoxy(0, MAP_Y); printf("          ");
		gotoxy(25, 10); printf("                                   ");//
		do
		{
			key = getch();
		} while (key == 224);

		switch (key)
		{
		case LEFT:
		case RIGHT:
		case UP:
		case DOWN:
			if ((dir == LEFT && key != RIGHT) ||
				(dir == RIGHT && key != LEFT) ||
				(dir == UP && key != DOWN) ||
				(dir == DOWN && key != UP))
				dir = key;
			break;
		case ESC:
			exit(0);
			break;
		default:
			break;
		}
	}
}
void CreateFood()
{
	int isReTry = false;
	do
	{
		isReTry = false;
		foodX = rand() % (MAP_X - 2) + 1;
		foodY = rand() % (MAP_Y - 2) + 1;
		for (int i = 0; i < length; i++)
		{
			if (snakeX[i] == foodX && snakeY[i] == foodY)
			{
				isReTry = true;
				break;
			}
		}
	} while (isReTry);
	speed -= 10;
	if (speed < 10) speed = 10;

	length++;
	snakeX[length - 1] = snakeX[length - 2];  //새로생긴 꼬리는 원래있던 꼬리좌표를 가져야한다.
	snakeY[length - 1] = snakeY[length - 2];

	gotoxy(foodX, foodY); printf("♥");
}
void GameRecord()
{
	end = clock(); //
	secTime = (float)(end - start) / 1000; 
	gotoxy(25, 10); printf("누적 플레이시간 : %0.3f 초 입니다.", secTime);//
	end = 0, start = 0, secTime = 0;
}
void GameOver()
{	
	GameRecord();
	ClearMap();
	DrawMap();
	Initalized();
	gotoxy(0, MAP_Y); printf("GAME OVER");
}
void GameManual()
{
	gotoxy(22, 2); printf("KMJ Snake Game 설명서");
	gotoxy(22, 4); printf("1. 방향키를 입력하면 뱀이 출발합니다.");
	gotoxy(22, 5); printf("2. 맵에 있는 먹이'♥'를 먹으면 뱀의 길이가 한칸 늘어납니다.");
	gotoxy(22, 6); printf("3. 시간이 지날수록 뱀의 이동속도는 빨라집니다.");
	gotoxy(22, 7); printf("4. 뱀의 몸통 또는 벽에 부딪힌다면 게임 오버입니다.");
}
int main()
{
	setcursortype(NOCURSOR);
	DrawMap();
	Initalized();
	GameManual();
	while (true)
	{
		KeyProecess();

		int isReTry = 0;
		if (snakeX[0] == foodX && snakeY[0] == foodY) //머리가 음식을 먹으면 랜덤으로 먹이 생성
		{
			CreateFood();
		}
		if (snakeX[0] <= 0 || snakeX[0] >= MAP_X - 1 || snakeY[0] <= 0 || snakeY[0] >= MAP_Y - 1) //벽에 충돌시 
		{
			GameOver();
		}
		for (int i = 1; i < length; i++)
		{
			if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) //머리가 꼬리들과 겹칠시
			{
				GameOver();
				break;
			}
		}
		DrawSnake();
		Sleep(speed);
	}

	return 0;
}




// 2. 난이도 조절
// snakespeed를 각 난이도에 맞춰 수치를 조정 

// 추가적인 속도 조절 일정량 비례로 바꾸기