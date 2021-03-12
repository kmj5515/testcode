// musicGame.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define MAP_X 30
#define MAP_Y 30
#define MAX_COUNT 7
#define EXIT -1

#define SPACE 32
#define ESC 27
#define D 68
#define F 70
#define K 75
#define L 76

int iSstart;
int noteX[255], noteY[255];

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
void DrawMap()
{
	for (int i = 1; i < MAP_X - 1 ; i++)
	{
		gotoxy(i, 0); printf("─");
		gotoxy(i, MAP_Y - 5); printf("─");
		gotoxy(i, MAP_Y - 1); printf("─");
	}
	for (int i = 1; i < MAP_Y - 1; i++)
	{
		gotoxy(0, i); printf("│");				// 0
		gotoxy(MAP_X - 23, i); printf("│");	// 7
		gotoxy(MAP_X - 15, i); printf("│");	// 15
		gotoxy(MAP_X - 8, i); printf("│");		// 22
		gotoxy(MAP_X - 1, i); printf("│");		// 29
	}
	getchar();
}
void DrawNote()
{

	
	for (int i = 1; i < MAP_Y - 1; i++)
	{
		gotoxy(MAP_X - 1, i + 1); printf("■■■■■■");
	}
	
}
int main()
{
	setcursortype(NOCURSOR);
	DrawMap();
	DrawNote();
	gotoxy(2, 2); printf("■■");

	return 0;
}

