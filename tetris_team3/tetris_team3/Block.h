#pragma once

#include "Board.h"
#include <array>

class Block {
public:
    Block(int shape);
    void reset(int shape, int startX, int startY);
    bool move(int dx, int dy, const Board& board);
    void rotate(const Board& board);
    void rotate();

    int getShape() const;
    int getAngle() const;
    int getX() const;
    int getY() const;
    int getBlockSpeed();

    static char getBlockShapeData(int shape, int angle, int row, int col);
    static const char shapes_[21][4][4][4];

private:
    int shape_;
    int angle_;
    int x_, y_;
};