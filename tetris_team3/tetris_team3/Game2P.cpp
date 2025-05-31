#include "Game2P.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>

#define EXT_KEY         0xE0
#define KEY_LEFT        0x4B
#define KEY_RIGHT       0x4D
#define KEY_UP          0x48
#define KEY_DOWN        0x50
#define KEY_SPACE       0x20
#define KEY_ESC         0x1B


void TetrisGame2P::run()
{
	player1.setOffset(2, 1);
	player1.setOffset(55, 1);

	//player1.init();
	//player2.init();

	int tick = 0;
	//while (!player1.isGameover() && !player2.isGameover()) {
	//	if (_kbhit()) {
	//		int key = _getch();
	//		if (key == 'w' || key == 'a' || key == 's' || key == 'd' || key == 'q')
	//			player2.processInput(key);
	//		else if (key == EXT_KEY || key == 0) {
	//			int extKey = _getch();
	//			if (extKey == KEY_SPACE || extKey == KEY_LEFT || extKey == KEY_RIGHT || extKey == KEY_DOWN || extKey == KEY_UP || extKey == KEY_ESC)
	//				player1.processInput(extKey);
	//		}
	//	}

	//	if (tick % player1.getTickSpeed() == 0) player1.update();
	//	if (tick % player2.getTickSpeed() == 0) player2.update();

	//	Sleep(15);
	//	++tick;
	//}
	//int winner = player1.isGameover() ? 2 : 1;
	//std::cout << "\nPlayer " << winner << " wins!\n";
	//_getch();
	

	while (true) {
		
		int lvl = player1.inputData();
		player2.setStageLevel(lvl);

		player1.init();
		player2.init();
		player1.setIsGameover(false);
		player2.setIsGameover(false);
		
		while (!player1.isGameover() && !player2.isGameover()) {
			if (_kbhit()) {
				int key = _getch();
				if (key == 'w' || key == 'a' || key == 's' || key == 'd' || key == 'q') {
					switch (key)
					{
					case 'w': player2.processInput(KEY_UP); break;
					case 'a': player2.processInput(KEY_LEFT); break;
					case 's': player2.processInput(KEY_DOWN); break;
					case 'd': player2.processInput(KEY_RIGHT); break;
					case 'q': player2.processInput(KEY_SPACE); break;
					default: break;
					}
				}
					
				else if (key == EXT_KEY || key == 0) {
					int extKey = _getch();
					if (extKey == KEY_SPACE || extKey == KEY_LEFT || extKey == KEY_RIGHT || extKey == KEY_DOWN || extKey == KEY_UP || extKey == KEY_ESC)
						player1.processInput(extKey);
				}

				if (player1.isGameover() || player2.isGameover()) {
					player1.rendererReset();
					player2.rendererReset();
					continue;
				}
			}

			if (tick % player1.getTickSpeed() == 0) {
				player1.update();
				if (player1.isGameover()) {
					player1.rendererReset();
					continue;
				}
			}
			if (tick % player2.getTickSpeed() == 0) {
				player2.update();
				if (player2.isGameover()) {
					player2.rendererReset();
					continue;
				}
			}
			
			player1.clearLineForLevelUp();
			player2.clearLineForLevelUp();

			Sleep(15);
			tick++;
		}
	}

	int winner = player1.isGameover() ? 2 : 1;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 40;
	pos.Y = 30;
	SetConsoleCursorPosition(hConsole, pos);
	std::cout << "\nPlayer " << winner << " wins!\n";
	while (!_kbhit()) { // 키 입력 대기
		Sleep(100);
	}
	_getche(); // 키 입력 소모
	system("cls"); // 종료 전 화면 정리
}
