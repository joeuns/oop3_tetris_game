#pragma once
#include "Board.h"
#include "Block.h"
#include <string>
#include <vector>

class Renderer {
public:
    Renderer();

    void initConsole(bool isTwoPlayerMode);
    void gotoXY(int x, int y);
    void setColor(int color);
    void hideCursor();

    void drawBoard(const Board& board, int playerIndex);
    void drawBlock(const Block& block, int playerIndex, int customOffsetX = -1, int customOffsetY = -1);
    void eraseBlock(const Block& block, int playerIndex, int customOffsetX = -1, int customOffsetY = -1);
    void drawNextBlockArea(const Block& nextBlock, int playerIndex);
    void drawStats(int level, int score, int lines, int playerIndex);
    void drawLogo();
    void drawGameOver();
    void drawPlayerSelectionScreen(int& selection);
    void drawPlayerMessage(int playerIndex, const std::string& message);
    void drawOptionSelectionScreen(int& selection);
    void drawStory();
    void drawHowToPlay();

    int getPlayerBoardOffsetX(int playerIndex) const;
    int getPlayerBoardOffsetY(int playerIndex) const;
    int getPlayerNextAreaBaseX(int playerIndex) const;
    int getPlayerNextAreaBaseY(int playerIndex) const;
    int getPlayerStatsAreaBaseX(int playerIndex) const;
    int getPlayerStatsAreaBaseY(int playerIndex) const;
    static Renderer& get();
    void drawLineClearEffect(int row, int stage);
    void drawLineClearEffect2p(int row, int stage, int playerIndex);

private:
    static constexpr int P1_BOARD_OFFSET_X = 2;
    static constexpr int P1_BOARD_OFFSET_Y = 1;
    static constexpr int P1_NEXT_X_REL = Board::COLS * 2 + 3;
    static constexpr int P1_NEXT_Y_ABS = 1;
    static constexpr int P1_STATS_X_REL = Board::COLS * 2 + 3;
    static constexpr int P1_STATS_Y_ABS = 7;

    static constexpr int P2_BOARD_OFFSET_X = P1_BOARD_OFFSET_X + P1_NEXT_X_REL + 12 + 10;
    static constexpr int P2_BOARD_OFFSET_Y = 1;
    static constexpr int P2_NEXT_X_REL = Board::COLS * 2 + 3;
    static constexpr int P2_NEXT_Y_ABS = 1;
    static constexpr int P2_STATS_X_REL = Board::COLS * 2 + 3;
    static constexpr int P2_STATS_Y_ABS = 7;

    static constexpr int NEXT_BLOCK_BOX_WIDTH = 6;
    static constexpr int NEXT_BLOCK_BOX_HEIGHT = 6;

    static constexpr int STATS_LABEL_X_LEVEL = 0;
    static constexpr int STATS_VALUE_X_LEVEL = 6;
    static constexpr int STATS_Y_OFFSET_SCORE = 2;
    static constexpr int STATS_Y_OFFSET_LINES = 4;
};