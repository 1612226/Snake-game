#include<iostream>
#include<time.h>
#include<Windows.h>
#include<thread>
#include<conio.h>
using namespace std;

#define MAX_SIZE_SNAKE 10
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 3
POINT snake[10];
POINT food[4];
int CHAR_LOCK;
int MOVING;
int SPEED;
int HEIGHT_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;

//Dieu chinh kich thuoc man hinh phu hop
void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
//Dua con tro den vi tri x, y tren man hinh
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Kiem tra vi tri thuc an co trung voi ran khong
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		if (snake[i].x == x && snake[i].y == y)
			return false;
	}
	return true;
}
//Tao thuc an cho ran
void GenerateFood() {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = rand() % (WIDTH_CONSOLE - 1) + 1;
			y = rand() % (HEIGHT_CONSOLE - 1) + 1;
		} while (IsValid(x, y) == false);
		food[i] = { x,y };
	}
}
//Khoi phuc lai du lieu ban dau
void ResetData() {
	//Khoi tao gia tri toan cuc
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1, HEIGHT_CONSOLE = 70, WIDTH_CONSOLE = 40, FOOD_INDEX = 1, SIZE_SNAKE = 6;
	//Khoi tao cac gia tri mac dinh cho snake
	for (int i = 10; i < 16; i++) {
		snake[i].x = i;
		snake[i].y = 5;
	}
	GenerateFood();
}

//Ve hinh chu nhat xung quanh khu vuc choi
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0) {
	GotoXY(x, y); cout << '#';
	for (int i = 1; i < width; i++)cout << '#';
	cout << '#';
	GotoXY(x, height + y); cout << '#';
	for (int i = 1; i < width; i++)cout << '#';
	cout << '#';
	for (int i = y + 1; i < height + y; i++) {
		GotoXY(x, i); cout << '#';
		GotoXY(x + width, i); cout << '#';
	}
	GotoXY(curPosX, curPosY);
}
//Thiet lap tro choi
void StartGame() {
	system("cls");
	ResetData();
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	STATE = 1;
}

//Thoat game
void ExitGame(HANDLE t) {
	system("cls");
	TerminateThread(t, 0);
}
//Dung game
void PauseGame(HANDLE t) {
	SuspendThread(t);
}
//Xu ly an food (chua xu ly khi food nam trung voi snake)
void Eat() {
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1) {
		FOOD_INDEX = 0;
		SIZE_SNAKE = 6;
		if (SPEED == MAX_SPEED)
			SPEED = 1;
		else
			SPEED++;
		GenerateFood();
	}
	else {
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}
//Xu ly snake chet
void ProcessDead() {
	STATE = 0;
	GotoXY(0, 2 + HEIGHT_CONSOLE);
	cout << "Dead, press y to continue or anykey to exit!";
}
//Ve ran va thuc an
void DrawSnakeAndFood(char *str) {
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	cout << str;
	for (int i = 0; i < SIZE_SNAKE; i++) {
		GotoXY(snake[i].x, snake[i].y);
		cout << str;
	}
}
//Di phai
void MoveRight() {
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE)
		ProcessDead();
	else {
		if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
			Eat();
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
	}
}
//Di trai
void MoveLeft() {
	if (snake[SIZE_SNAKE - 1].x - 1 == 0)
		ProcessDead();
	else {
		if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
			Eat();
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
	}
}
//Di xuong
void MoveDown() {
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGHT_CONSOLE) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y)
			Eat();
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
	}
}
//Di len
void MoveUp() {
	if (snake[SIZE_SNAKE - 1].y - 1 == 0) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y)
			Eat();
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
	}
}
//Xu ly cham vao than
bool BiteSnake() {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		for (int j = i + 1; j < SIZE_SNAKE; j++) {
			if (snake[i].x == snake[j].x&&snake[i].y == snake[j].y)
				return true;
		}
	}
	return false;
}
//Thu tuc cho thread
void ThreadFunc() {
	while (1) {
		if (STATE == 1) {//Nếu vẫn còn snake vẫn còn sống
			DrawSnakeAndFood(" ");
			switch (MOVING) {
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			DrawSnakeAndFood("0");
			Sleep(5000 / SPEED);//Hàm ngủ theo tốc độ SPEEED
		}
	}
}
void main() {
	int temp;
	FixConsoleWindow();
	StartGame();
	thread t1(ThreadFunc); //Tao thread cho snake
	HANDLE handle_t1 = t1.native_handle(); //Lay handle cua thread
	while (1) {
		temp = toupper(getch());
		if (STATE == 1) {
			if (temp == 'P' ) {
				PauseGame(handle_t1);
			}
			else if (temp == 27) {
				ExitGame(handle_t1);
				return;
			}
			else {
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else {
			if (temp == 'Y') StartGame();
			else {
				ExitGame(handle_t1);
				return;
			}
		}
	}
}