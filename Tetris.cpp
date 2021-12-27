#include "Tetris.h"

#define LEFT 75 //�ƽ�Ű�ڵ� ����?
#define RIGHT 77 //�ƽ�Ű�ڵ� ����?
#define UP 72 //�ƽ�Ű�ڵ� ����?
#define DOWN 80 //�ƽ�Ű�ڵ� ����?
#define ESC 27
#define BX 5
#define BY 1
#define BW 10 //��Ʈ���� ������ ����  
#define BH 25 //��Ʈ���� ������ ����
class Tetris {
private:
	int combo, counter; // �޺�, �ӵ�ī���� �߰�
	int score; // ���ھ� �߰�
	int gametime; // ������ �ð� �߰�
	int Level; //���� �ý����߰�
public:
	Tetris() {
		combo = 0;
		counter = 0; 
		score = 0; 
		gametime = 20; 
		Level = 1; 
	}
	void Combo(); //�޺� �����Լ�
	void Speedup();//�ӵ� ��� �Լ�
	void DrawScore(); //���ھ�� ������ �����ִ� �Լ�
	void PlusComboCounter() {
		combo++;
		counter++;
		score = score + 1000;//����3 : �� �ٸ��� ���Ž� 1000�� �߰�
	}
	void Plusgametime() {
		delay(1000 / gametime); //�������� �ӵ� ���� gametime������ Ŀ������ ���� �����´�.
	}
};
void Tetris::Combo()
{
	if (combo >= 2) { //2�� �̻� ���ÿ� ���Ž� (���� + (1000 * (�޺� - 1) ) )
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
	gotoxy(5, 28); printf("��������������������������������������������\n");
	gotoxy(5, 29); printf("��      Level : %ld     ��\n", Level);
	gotoxy(5, 30); printf("��������������������������������������������\n");
	gotoxy(35, 20); printf("�� ���� : %07d                  ��\n", score);
	gotoxy(35, 21); printf("����������������������������������������������������������������������\n");
}

void TestFull();
void DrawScreen();//���θ޴��� �����ִ� �Լ�
void DrawBoard(); //���Ӻ��带 ����ϴ� �Լ�
void NextDrawBoard(); //�����긯 ����Լ�

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
const char *arTile[] = { "  ","��","��" };//���� : ". "�� �����Ͽ� �������ϰ� ����
const char *arTile2[] = { "  ","��","��" }; //���� : "������� �����Լ�"
int board[BW + 2][BH + 2];
int nx, ny;
int brick, nbrick, rot; // ���ݳ����º�� , ���߿����ú�� , ��

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
	{	//���ѷ��� 
	    //�긯 ������� ����
		Ter1.DrawScore();
		brick = nbrick;
		nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
		NextDrawBoard();
		nx = BW / 2;    									 // �긯 ��ġ ��ġ ����
		ny = 3;	   									 // �긯 ��ġ ��ġ ����
		rot = 0;

		PrintBrick(TRUE);//�긯��ġ
		if (GetAround(nx, ny, brick, rot) != EMPTY) {//��Ʈ���� ���� �� á���� ���ѷ��� Ż��
			break;
		}
		nStay = nFrame;
		for (; 2;)   //���ѷ���
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
	setcursortype(NOCURSOR); //����1. NORMALCURSOR��ſ� NOCURSOR�� ����Ͽ� ������ ����
}

void DrawScreen() //��Ʈ���� ������ ����
{
	int x, y;
	for (x = 0; x<BW + 2; x++) {
		for (y = 0; y<BH + 2; y++) {
			gotoxy(BX + x * 2, BY + y);
			puts(arTile[board[x][y]]);
		}
	}
	gotoxy(41, 2); printf("����������������������������������������������");
	gotoxy(41, 3); printf("��      Next Brick     ��");
	gotoxy(41, 4); printf("����������������������������������������������");
	gotoxy(41, 13); printf("����������������������������������������������");
	gotoxy(41, 14); puts("�� Dabin Tetris Ver 1.0��");
	gotoxy(41, 15); puts("����������������������������������������������");
	gotoxy(35, 16); printf("����������������������������������������������������������������������\n");
	gotoxy(35, 17); puts("�� �̵� :�� ��, ȸ�� :��, ���� :�� ��");
	gotoxy(35, 18); puts("�� ������ ���� : �����̽���        ��");
	gotoxy(35, 19); printf("����������������������������������������������������������������������\n");
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
void NextDrawBoard()//������ ����
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

void PrintBrick(BOOL Show)//�긯���
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

BOOL MoveDown()//�Ʒ��� ���������ϴ� �Լ�
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
void TestFull()//�� ���� ���� ä�������� Ȯ���ϴ� �Լ�
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
			for (ty = y; ty>1; ty--) { //�� �� ���� ����ó�� �κ�
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


