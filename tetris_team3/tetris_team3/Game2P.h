#pragma once
#include "Game.h"
class TetrisGame2P
{
private:
	TetrisGame player1;
	TetrisGame player2;
	//bool isGameover1 = false;
	//bool isGameover2 = false;
public:
	TetrisGame2P() = default;
	~TetrisGame2P() = default;
	void run();
};

