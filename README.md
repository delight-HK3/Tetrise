# Tetrise
* 기능
  * 기본적인 테트리스 기능 
  * 1줄 제거시 점수 휙득 
  * 2줄 이상 제거시 점수 추가 휙득
  * 레벨 증가시 블록이 내려오는 속도 증가
  * 다음 블록이 어떤 블록인지 확인하는 기능

### 스코어 추가함수
```C++
void PlusComboCounter() {
	combo++;
	counter++;
	score = score + 1000;//한 줄마다 제거시 1000점 추가
}
```

### 콤보함수
```C++
void Tetris::Combo()
{
	if (combo >= 2) { //score + (1000 * (combo - 1) ) )
		combo--;
		score = score + (1000 * combo);
	}
	while (combo != 0) {
		combo--;
	}
}
```

### 속도증가 함수
```C++
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
```

### 다음블럭 출력 함수
```C++
void NextDrawBoard()
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
```

### 게임화면
![게임수정후](https://user-images.githubusercontent.com/74585673/147439343-8eac3a96-985d-4cf7-93b7-04b2a3ec51e2.PNG)

