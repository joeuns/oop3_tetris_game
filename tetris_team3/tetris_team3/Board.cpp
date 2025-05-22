#include "Board.h"
#include "Block.h"
#include <iostream>

Board::Board() {
    init();
}

void Board::init() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (j == 0 || j == COLS - 1 || i == ROWS - 1)
                grid_[i][j] = WALL_CELL;  // 벽 또는 바닥
            else
                grid_[i][j] = EMPTY_CELL;
        }
    }
}

// 블록이 보드에 충돌하는지 검사
bool Board::strikeCheck(int shape, int angle, int x, int y) const {
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                int boardR = y + r;
                int boardC = x + c;

                if (boardC < 0 || boardC >= COLS || boardR >= ROWS)
                    return true;

                if (boardR >= 0 && grid_[boardR][boardC] != EMPTY_CELL)
                    return true;
            }
        }
    }
    return false;
}

// 블록을 보드에 병합
bool Board::mergeBlock(int shape, int angle, int x, int y) {
    bool canMerge = true;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                int boardR = y + r;
                int boardC = x + c;

                if (boardR >= 0 && boardR < ROWS && boardC >= 0 && boardC < COLS) {
                    if (grid_[boardR][boardC] != EMPTY_CELL)
                        canMerge = false;
                    grid_[boardR][boardC] = WALL_CELL;
                }
                else if (boardR < 0) {
                    canMerge = false;
                }
            }
        }
    }
    return canMerge;
}

// 가득 찬 줄을 제거하고 위를 내림
int Board::clearFullLines() {
    int clearedLines = 0;
    for (int i = ROWS - 2; i >= 0; --i) {
        bool lineFull = true;
        for (int j = 1; j < COLS - 1; ++j) {
            if (grid_[i][j] == EMPTY_CELL) {
                lineFull = false;
                break;
            }
        }

        if (lineFull) {
            ++clearedLines;
            for (int k = i; k > 0; --k)
                for (int j = 1; j < COLS - 1; ++j)
                    grid_[k][j] = grid_[k - 1][j];

            for (int j = 1; j < COLS - 1; ++j)
                grid_[0][j] = EMPTY_CELL;

            ++i; // 같은 줄 재검사
        }
    }
    return clearedLines;
}

const std::array<std::array<char, Board::COLS>, Board::ROWS>& Board::getGrid() const {
    return grid_;
}