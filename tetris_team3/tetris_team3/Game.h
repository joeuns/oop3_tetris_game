#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Board.h"
#include "Block.h"
#include "Renderer.h"

enum class GameMode { ONE_PLAYER, TWO_PLAYER };
enum class PlayerAction { MOVE_LEFT, MOVE_RIGHT, ROTATE, SOFT_DROP, HARD_DROP, NONE };

struct Stage {
    int speed;
    int stickRate;
    int clearLine;
};

class TetrisGame {
public:
    TetrisGame();
    ~TetrisGame();
    void run();

private:
    static constexpr int TOTAL_SHAPES = 21;
    static constexpr int I_BLOCK_4_INDEX = 4;

    Renderer renderer_;
    GameMode gameMode_;
    int startLevel_;

    Board board_[2];
    Block* currentBlock_[2];
    Block* nextBlock_[2];
    std::vector<Stage> stages_;
    int level_[2];
    int score_[2];
    int totalscore_[2];
    int lines_[2];
    bool isGameOver_[2];
    int totalLinesCleared_[2];

    int stageWins_[2];
    int currentStage_;
    bool gameWon_[2];

    void selectGameMode();
    void initCommon();
    void initPlayer(int playerIndex);
    void initPlayerForStage(int playerIndex, int stageIdx);

    bool spawnBlock(int playerIndex);
    void processInput();
    void handlePlayerAction(int playerIndex, PlayerAction action);
    void updatePlayer(int playerIndex);
    void checkLevelUp(int playerIndex);

    int getRandomShapeIndex(int playerIndex);

};

namespace KeyCodes {
    static constexpr int EXT_KEY = 0xE0;
    static constexpr int KEY_ESC = 0x1B;
    static constexpr int KEY_ENTER = 0x0D;
    static constexpr int P1_KEY_LEFT_ARROW = 0x4B;
    static constexpr int P1_KEY_RIGHT_ARROW = 0x4D;
    static constexpr int P1_KEY_UP_ARROW = 0x48;
    static constexpr int P1_KEY_DOWN_ARROW = 0x50;
    static constexpr int P1_KEY_SPACE = 0x20;
    static constexpr char P2_KEY_LEFT = 'a';
    static constexpr char P2_KEY_RIGHT = 'd';
    static constexpr char P2_KEY_UP = 'w';
    static constexpr char P2_KEY_DOWN = 's';
    static constexpr char P2_KEY_TAB = '\t';
}