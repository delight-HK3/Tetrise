#include "Tetris.h"

#define LEFT 75 //아스키코드 관련?
#define RIGHT 77 //아스키코드 관련?
#define UP 72 //아스키코드 관련?
#define DOWN 80 //아스키코드 관련?
#define ESC 27
#define BX 5
#define BY 1
#define BW 10 //테트리스 게임판 가로  
#define BH 25 //테트리스 게임판 세로
class Tetris {
private:
	int combo, counter; // 콤보, 속도카운터 추가
	int score; // 스코어 추가
	int gametime; // 딜레이 시간 추가
	int Level; //레벨 시스템추가
public:
	Tetris() {
		combo = 0;
		counter = 0; 
		score = 0; 
		gametime = 20; 
		Level = 1; 
	}
	void Combo(); //콤보 제어함수
	void Speedup();//속도 상승 함수
	void DrawScore(); //스코어와 레벨을 보여주는 함수
	void PlusComboCounter() {
		combo++;
		counter++;
		score = score + 1000;//변경3 : 한 줄마다 제거시 1000점 추가
	}
	void Plusgametime() {
		delay(1000 / gametime); //내려오는 속도 제어 gametime변수가 커질수록 빨리 내려온다.
	}
};
void Tetris::Combo()
{
	if (combo >= 2) { //2개 이상 동시에 제거시 (점수 + (1000 * (콤보 - 1) ) )
		combo--;
		score = score + (1000 * combo);
	}
	while (combo != 0) {
		combo--;
	}
}
void Tetris::Speedup()
{
	if (counter >= 5) {
		gametime = gametime + 30;
		Level++;
		while (counter != 0) {
			counter--;
		}
	}
}
void Tetris::DrawScore()
{
	gotoxy(5, 28); printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
	gotoxy(5, 29); printf("┃      Level : %ld     ┃\n", Level);
	gotoxy(5, 30); printf("┗━━━━━━━━━━━━━━━━━━━━┛\n");
	gotoxy(35, 20); printf("┃ 점수 : %07d                  ┃\n", score);
	gotoxy(35, 21); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}

void TestFull();
void DrawScreen();//메인메뉴를 보여주는 함수
void DrawBoard(); //게임보드를 출력하는 함수
void NextDrawBoard(); //다음브릭 출력함수

BOOL ProcessKey();
void PrintBrick(BOOL Show);
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();

struct Point {
	int x, y;
};
struct Point Shape[][4][4] = {
	{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },
	{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },
	{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },
	{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },
	{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },
	{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },
	{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },
};

enum { EMPTY, BRICK, WALL };
const char *arTile[] = { "  ","■","□" };//변경 : ". "을 삭제하여 보기편하게 개선
const char *arTile2[] = { "  ","■","□" }; //변경 : "다음블록 예측함수"
int board[BW + 2][BH + 2];
int nx, ny;
int brick, nbrick, rot; // 지금나오는블록 , 나중에나올블록 , 줄

Tetris Ter1;
void main(BOOL Show)
{
	int nFrame, nStay;
	int x, y;

	setcursortype(NOCURSOR);
	randomize();

	clrscr();
	for (x = 0; x<BW + 2; x++) {
		for (y = 0; y<BH + 2; y++) {
			board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
		}
	}
	DrawScreen();
	nFrame = 20;
	nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
	NextDrawBoard();
	for (; 1;)
	{	//무한루프 
	    //브릭 모양임의 지정
		Ter1.DrawScore();
		brick = nbrick;
		nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
		NextDrawBoard();
		nx = BW / 2;    									 // 브릭 배치 위치 연산
		ny = 3;	   									 // 브릭 배치 위치 연산
		rot = 0;

		PrintBrick(TRUE);//브릭배치
		if (GetAround(nx, ny, brick, rot) != EMPTY) {//테트리스 블럭이 꽉 찼을때 무한루프 탈출
			break;
		}
		nStay = nFrame;
		for (; 2;)   //무한루프
		{

			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDown()) {
					break;
				}
			}
			if (ProcessKey()) {
				break;
			}
			Ter1.Plusgametime();
		}
	}
	clrscr();
	gotoxy(30, 12); puts("G A M E  O V E R");
	setcursortype(NOCURSOR); //변경1. NORMALCURSOR대신에 NOCURSOR을 사용하여 깜빡임 제거
}

void DrawScreen() //테트리스 게임판 제작
{
	int x, y;
	for (x = 0; x<BW + 2; x++) {
		for (y = 0; y<BH + 2; y++) {
			gotoxy(BX + x * 2, BY + y);
			puts(arTile[board[x][y]]);
		}
	}
	gotoxy(41, 2); printf("┏━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(41, 3); printf("┃      Next Brick     ┃");
	gotoxy(41, 4); printf("┗━━━━━━━━━━━━━━━━━━━━━┛");
	gotoxy(41, 13); printf("┏━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(41, 14); puts("┃ Dabin Tetris Ver 1.0┃");
	gotoxy(41, 15); puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
	gotoxy(35, 16); printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	gotoxy(35, 17); puts("┃ 이동 :← →, 회전 :↑, 내림 :↓ ┃");
	gotoxy(35, 18); puts("┃ 끝까지 내림 : 스페이스바        ┃");
	gotoxy(35, 19); printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
} 
void DrawBoard()
{
	int x, y;
	for (x = 1; x<BW + 1; x++) {
		for (y = 1; y<BH + 1; y++) {
			gotoxy(BX + x * 2, BY + y);
			puts(arTile[board[x][y]]);
		}
	}
}
void NextDrawBoard()//다음블럭 예측
{
	int x, y, i;

	for (x = 42; x <= 62; x += 2) {
		for (y = 5; y <= 12; y++) {
			gotoxy(x, y);
			puts(arTile2[(x == 42 || x == 62 || y == 5 || y == 12) ? WALL : EMPTY]);
		}
	}
	for (i = 0; i < 4; i++) {
		gotoxy(50 + (Shape[nbrick][0][i].x) * 2, 8 + Shape[nbrick][0][i].y);
		puts(arTile2[BRICK]);
	}

}
BOOL ProcessKey()
{
	int ch, trot;

	if (kbhit()) {
		ch = getch();
		if (ch == 0xE0 || ch == 0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx--;
					PrintBrick(TRUE);
				}
				break;
			case RIGHT:
				if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx++;
					PrintBrick(TRUE);
				}
				break;
			case UP:
				trot = (rot == 3 ? 0 : rot + 1);
				if (GetAround(nx, ny, brick, trot) == EMPTY) {
					PrintBrick(FALSE);
					rot = trot;
					PrintBrick(TRUE);
				}
				break;
			case DOWN:
				if (MoveDown()) {
					return TRUE;
				}
				break;
			}
		}
		else {
			switch (ch) {
			case ' ':
				while (MoveDown() == FALSE) { ; }
				return TRUE;
			}
		}
	}
	return FALSE;
}

void PrintBrick(BOOL Show)//브릭출력
{
	int i;
	for (i = 0; i<4; i++) {
		gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
		puts(arTile[Show ? BRICK : EMPTY]);
	}
}

int GetAround(int x, int y, int b, int r)
{
	int i, k = EMPTY;
	for (i = 0; i<4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()//아래로 내려가게하는 함수
{
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
		TestFull();
		return TRUE;
	}
	PrintBrick(FALSE);
	ny++;
	PrintBrick(TRUE);
	return FALSE;
}
void TestFull()//한 줄이 전부 채워졌는지 확인하는 함수
{
	int i, x, y, ty;

	for (i = 0; i<4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
	}
	for (y = 1; y<BH + 1; y++)
	{
		for (x = 1; x<BW + 1; x++) {
			if (board[x][y] != BRICK) break;
		}
		if (x == BW + 1) {
			for (ty = y; ty>1; ty--) { //한 줄 제거 연산처리 부분
				for (x = 1; x<BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			Ter1.PlusComboCounter();
			DrawBoard();
			delay(200);
		}
	}
	Ter1.Combo();
	Ter1.Speedup();
}


