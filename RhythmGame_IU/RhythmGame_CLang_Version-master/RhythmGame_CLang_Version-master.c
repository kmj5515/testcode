﻿#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>
#include<time.h>
#include"Screen.h""
#include<memory.h>

#define ALLNOTE 1000

// 소리 출력 PlaySound함수
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")
//PlaySound(TEXT("Festival_of_Ghost.wav"), NULL, SND_ASYNC | SND_LOOP);

int n = 0;

int nScore = 0;
char strScore[20] = "  ";
int nCombo = 0;
//
clock_t RunningTime;

// 노트 판별 존
typedef struct _NOTECOUNT {
	int nXofA;   //(2,29)
	int nXofS;
	int nXofD;
	int nXofJ;
	int nXofK;
	int nXofL;
}NOTECOUNT;
NOTECOUNT Count;

// 스테이지 구성
typedef enum _STAGE {
	READY, RUNNING, PAUSE, RESULT
}STAGE;
STAGE Stage;

void record(int nScore) {
	char* arr[20];
	// 현재 점수 입력
	FILE *fp = fopen("record.txt", "w");
	fprintf(fp, "%d\n", nScore);
	fclose(fp);
}

void recordShow(void) {
	int i = 0;
	char* arr[20]; //20위까지 표시
	FILE *fp = fopen("record.txt", "r");
	while ((fscanf(fp, "%s", arr[i++]) != EOF));
	char ch[20] = { 0, };
	for (int j = 0; j < i; i++) {
		sprintf(ch, "%s", arr[j]);
		ScreenPrint(10, 10 + j, ch);
	}
	fclose(fp);
}


// 스테이지 기본 틀
void Map(void) {
	int nNum = 0;
	ScreenPrint(0, 0, "□□□□□□□□□□□□□□□□□□□□□");
	for (int i = 1; i < 29; i++) {
		ScreenPrint(0, i, "□\t\t\t\t\t□");
	}
	ScreenPrint(0, 29, "□□□□□□□□□□□□□□□□□□□□□");
	ScreenPrint(2, 26, "______________________________________");
}

// 우측 점수 출력틀
void ScoreMap() {
	// 경과시간
	char nTime[20];
	sprintf(nTime, "시간 : %d.%d초", RunningTime / 1000, RunningTime % 1000);
	ScreenPrint(44, 2, nTime);
	// 점수 목록
	ScreenPrint(44, 10, strScore);
	ScreenPrint(44, 22, "Great : 300점");
	ScreenPrint(44, 23, "Perfect : 500점");
	//ScreenPrint(44, 25, "\'p\' to Pause");
	// 점수
	char nScoree[20];
	sprintf(nScoree, "점수 : %d 점", nScore);
	ScreenPrint(44, 4, nScoree);
	ScreenPrint(44, 27, "<<< 히트 구간(G)");
	ScreenPrint(44, 28, "<<< 히트 구간(P)");
	ScreenPrint(44, 29, "<<< 히트 구간(G)");
	//콤보
	char strCombo[20];
	sprintf(strCombo, "%d 콤보", nCombo);
	ScreenPrint(44, 13, strCombo);
}



// 시간 컨트롤러
typedef struct  CONTROL {
	clock_t MovTime; // 싱크 맞추는 이동시간
	clock_t OldTime; // 이전 이동시간
	int nMagic; // 싱크 조율 변수

}CONTROL;
CONTROL Control;

// 시작화면
void ReadyMap() {
	ScreenPrint(15, 10, "IU - 밤편지");
	ScreenPrint(2, 26, "□□□■■■□□□  ■■■□□□■■■");
	ScreenPrint(2, 27, "  A     S     D       J     K      L");

}

void ReadyMap1() {
	ScreenPrint(10, 15, "Press Enter to Start");
}



// 노트에 해당하는 변수 선언
char* nKeyNone = "                                      ";
char* nKeyL = "                                ■■■";
char* nKeyK = "                          ■■■";
char* nKeyJ = "                    ■■■";
char* nKeyD = "            ■■■";
char* nKeyS = "      ■■■";
char* nKeyA = "■■■";
char* nKeyAJ = "■■■              ■■■";
char* nKeySK = "      ■■■              ■■■";
char* nKeyDL = "            ■■■              ■■■";


// 19개 / 3 / 3 / 3 / 1 / 3 / 3 / 3
char* Note[ALLNOTE];
void NoteCheck(void);



// 2차원 배열을 아래로 떨어지게끔 해주는 함수
char* Check;
char* Check1;
char* Check2;
void ShowNote(int n) {
	for (int i = 0; i < 28; i++) {
		ScreenPrint(2, i + 1, Note[27 - i + n]);

	}
	Check = Note[n];   // 2차원배열 0.5초마다 Note의 배열 중  28행에 떨어지는 값을 저장받는다. 
	Check1 = Note[n - 1]; // 0.5초 지난 후 입력받더라도 great으로 인정해주는 부분
	Check2 = Note[n + 1]; // rmrm
}



// 입력 키 판별해주는 함수
void CheckL(int n);
void CheckK(int n);
void CheckJ(int n);
void CheckD(int n);
void CheckS(int n);
void CheckA(int n);




void init() {
	Control.MovTime = 52;
	Control.OldTime = 0;
	Control.nMagic = 0;
	Stage = READY;
	NoteCheck();
	for (int i = 0; i < ALLNOTE; i++) {
		Note[i] = " ";
	}
	RunningTime = 0;

	Count.nXofA = 2;   //(2,29)
	Count.nXofS = 8;
	Count.nXofD = 14;
	Count.nXofJ = 21;
	Count.nXofK = 27;
	Count.nXofL = 33;

	Stage = READY;

}


clock_t Oldtime = 0;
void Update() {
	clock_t Curtime = clock();
	Control.nMagic = 1;
	switch (Stage) {
	case READY:
		Oldtime = Curtime;
		break;
	case RUNNING:
		// 게임 시작 후 시간 측정변수
		RunningTime = clock() - Oldtime;
		if (RunningTime > 30000) {
			Stage = RESULT;
		}
		NoteCheck();
		break;
	case PAUSE:
		break;
	case RESULT:
		record(nScore);
		break;
	}
}





clock_t Oldtime1 = 0;
void Render() {
	clock_t Curtime = clock(); // 0.5초 마다 1colum씩 내린다.
	ScreenClear();
	//출력코드
	Map();
	ScoreMap();
	switch (Stage) {
	case READY:
		Oldtime1 = Curtime;
		ReadyMap();
		if (Curtime % 1000 > 500) {
			ReadyMap1();
		}
		break;
	case PAUSE:
	case RUNNING:
		if (RunningTime > 3100) {
			if (Curtime - Control.OldTime > Control.MovTime) {
				Control.OldTime = Curtime;
				n++;
			}
			ShowNote(n);

		}

		break;

	case RESULT:
		recordShow();
		break;
	}





	ScreenFlipping();
}

void Release() {

}

int main(void) {
	int nKey;
	ScreenInit();
	init(); // 초기화
	PlaySound(TEXT("opening.wav"), NULL, SND_ASYNC | SND_LOOP);
	while (1) {

		if (_kbhit())
		{
			nKey = _getch();
			if (nKey == 13) 
			{
				Stage = RUNNING; // 엔터 입력 시 running시작 음악 호출
				PlaySound(TEXT("IU-Through the Night.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			if (nKey == 'p') 
			{
				Stage = PAUSE;
			}
			switch (nKey)
			{
			case 'a':
				CheckA(n);
				break;
			case 's':
				CheckS(n);
				break;
			case 'd':
				CheckD(n);
				break;
			case 'j':
				CheckJ(n);
				break;
			case 'k':
				CheckK(n);
				break;
			case 'l':
				CheckL(n);
				break;

			}
		}

		Update();  // 데이터 갱신
		Render();  // 화면출력


	}
	Release(); // 해제
	ScreenRelease();
	return 0;
}




// 악보
void NoteCheck(void) 
{
	for (int i = 0; i < 30; i++)
	{
		Note[i] = " ";
	}

	Note[122 + Control.nMagic] = nKeyS;	//이
	Note[127 + Control.nMagic] = nKeyD;	//밤~

	Note[168 + Control.nMagic] = nKeyL;	//그~

	Note[178 + Control.nMagic] = nKeyL;	//날
	Note[183 + Control.nMagic] = nKeyS;	//의~

	Note[219 + Control.nMagic] = nKeyL;	//반
	Note[226 + Control.nMagic] = nKeyD;;	//딧
	Note[233 + Control.nMagic] = nKeyL;	//불
	Note[240 + Control.nMagic] = nKeyD;	//을~

	Note[285 + Control.nMagic] = nKeyL; //당
	Note[292 + Control.nMagic] = nKeyS;	//신
	Note[303 + Control.nMagic] = nKeyAJ;//의~

	Note[310 + Control.nMagic] = nKeySK;//창

	Note[320 + Control.nMagic] = nKeyDL;//가

	Note[330 + Control.nMagic] = nKeySK;//까
	Note[340 + Control.nMagic] = nKeyAJ;//이
	Note[360 + Control.nMagic] = nKeySK;//보
	Note[380 + Control.nMagic] = nKeyDL;//낼
	Note[390 + Control.nMagic] = nKeySK;//게
	Note[400 + Control.nMagic] = nKeyAJ;//요
}


// 충돌처리
// main에서 해당 키 입력시 호출되는 함수들

void CheckL(int n) { // (28, 2) 부분이 " " 인지 "ㅁ" 인지 판별
	if (Check == "                                ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "                                ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "                                ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}

	else if (Check == "            ■■■              ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "            ■■■              ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "            ■■■              ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}

}

void CheckK(int n) { // (28, 2) 부분이 " " 인지 "ㅁ" 인지 판별
	if (Check == "                          ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "                          ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "                          ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check == "      ■■■              ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "      ■■■              ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "      ■■■              ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}
}

void CheckJ(int n) { // (28, 2) 부분이 " " 인지 "ㅁ" 인지 판별
	if (Check == "                    ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "                    ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "                    ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check == "■■■              ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "■■■              ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "■■■              ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}
}

void CheckD(int n) { // (28, 2) 부분이 " " 인지 "ㅁ" 인지 판별
	if (Check == "            ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "            ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "            ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check == "            ■■■              ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "            ■■■              ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "            ■■■              ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}
}

void CheckS(int n) { // (28, 2) 부분이 " " 인지 "ㅁ" 인지 판별
	if (Check == "      ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "      ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "      ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check == "      ■■■              ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "      ■■■              ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "      ■■■              ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}
}

void CheckA(int n) { // (28, 2) 부분이 " " 인지 "ㅁ" 인지 판별
	if (Check == "■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check == "■■■              ■■■") { // 28행 부분 - 이 부분에서 hit 시 Perfect
		nScore += 500; nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if (Check1 == "■■■              ■■■") {	//29행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else if (Check2 == "■■■              ■■■") {	//27행 부분 - 이 부분에서 hit 시 great
		nScore += 300; nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}
}

