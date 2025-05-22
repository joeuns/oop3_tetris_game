#pragma once

#include <vector>
#include <cstdlib>
#include <ctime>
#include "Board.h"
#include "Block.h"
#include "Renderer.h"
#include <limits>  
#include <algorithm>

struct Stage {
    int speed;
    int stickRate;
    int clearLine;
};

class TetrisGame {
public:
    TetrisGame();
    ~TetrisGame();
    void init();
    void run();

private:
    Board board_;
    Renderer renderer_;
    Block* currentBlock_;
    Block* nextBlock_;

    std::vector<Stage> stages_;
    int level_;
    int score_;
    int lines_;

    bool isGameOver_;

    void setupStages();
    void inputData();
    bool spawnBlock();
    void processInput();
    void update();
};