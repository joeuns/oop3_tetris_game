#pragma once

#include "Board.h"
#include "Block.h"
#include <string>

class Renderer {
public:
    Renderer(int offsetX, int offsetY);

    void initConsole();
    void gotoXY(int x, int y);
    void setColor(int color);
    void hideCursor();

    void drawBoard(const Board& board);
    void drawBlock(const Block& block, int customOffsetX = -1, int customOffsetY = -1);
    void eraseBlock(const Block& block, int customOffsetX = -1, int customOffsetY = -1);
    void drawNextBlockArea(const Block& nextBlock);
    void drawStats(int level, int score, int lines);
    void drawLogo();
    void drawGameOver();
    void clearArea(int x, int y, int widthChars, int heightChars);

    // offsetX_와 offsetY_를 위한 public getter 함수 추가
    int getOffsetX() const { return offsetX_; }
    int getOffsetY() const { return offsetY_; }

private:
    int offsetX_, offsetY_;

    static constexpr int NEXT_BLOCK_AREA_X = 33;
    static constexpr int NEXT_BLOCK_AREA_Y = 1;
    static constexpr int NEXT_BLOCK_BOX_WIDTH = 6;
    static constexpr int NEXT_BLOCK_BOX_HEIGHT = 6;

    static constexpr int STATS_AREA_X = 35; // Renderer 내부에서 offsetX_를 더해서 사용할 수 있음
    static constexpr int STATS_LABEL_Y_LEVEL = 7;
    static constexpr int STATS_VALUE_Y_LEVEL = 7;
    static constexpr int STATS_LABEL_X_LEVEL = 0; // STATS_AREA_X 기준 상대 X
    static constexpr int STATS_VALUE_X_LEVEL = 6; // STATS_AREA_X 기준 상대 X

    static constexpr int STATS_LABEL_Y_SCORE = 9;
    static constexpr int STATS_VALUE_Y_SCORE = 10;
    static constexpr int STATS_LABEL_X_SCORE = 0;
    static constexpr int STATS_VALUE_X_SCORE = 0;

    static constexpr int STATS_LABEL_Y_LINES = 12;
    static constexpr int STATS_VALUE_Y_LINES = 13;
    static constexpr int STATS_LABEL_X_LINES = 0;
    static constexpr int STATS_VALUE_X_LINES = 0;

    bool statsLabelsPrinted_ = false;
};