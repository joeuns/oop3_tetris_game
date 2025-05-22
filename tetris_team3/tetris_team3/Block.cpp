#include "Block.h"
#include "Board.h"

// 블록 모양 정의 (7종, 각 4방향 회전)
const char Block::shapes_[7][4][4][4] = {
    // I (막대)
    {
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} },
        { {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0} }
    },
    // O (네모)
    {
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }
    },
    // T
    {
        { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }
    },
    // L
    {
        { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} },
        { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }
    },
    // J (L 대칭)
    {
        { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} }
    },
    // S
    {
        { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} },
        { {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }
    },
    // Z
    {
        { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0} }
    }
};

// 블록 생성자: 모양, 초기 각도, 위치 설정
Block::Block(int shape) : shape_(shape), angle_(0), x_(Board::COLS / 2 - 2), y_(-2) {}

void Block::reset(int shape, int startX, int startY) {
    shape_ = shape;
    angle_ = 0;
    x_ = startX;
    y_ = startY;
}

// 블록 이동 (충돌 검사 후 위치 갱신)
bool Block::move(int dx, int dy, const Board& board) {
    int newX = x_ + dx;
    int newY = y_ + dy;
    if (!board.strikeCheck(shape_, angle_, newX, newY)) {
        x_ = newX;
        y_ = newY;
        return true;
    }
    return false;
}

// 블록 회전 (단순 회전 + 좌우 Wall Kick 시도)
void Block::rotate(const Board& board) {
    int newAngle = (angle_ + 1) % 4;

    if (!board.strikeCheck(shape_, newAngle, x_, y_)) {
        angle_ = newAngle;
    }
    else {
        if (!board.strikeCheck(shape_, newAngle, x_ - 1, y_)) {
            x_ -= 1;
            angle_ = newAngle;
        }
        else if (!board.strikeCheck(shape_, newAngle, x_ + 1, y_)) {
            x_ += 1;
            angle_ = newAngle;
        }
    }
}

// Getter 함수들
int Block::getShape() const { return shape_; }
int Block::getAngle() const { return angle_; }
int Block::getX() const { return x_; }
int Block::getY() const { return y_; }

// 블록 데이터 접근 (좌표 범위 검사 포함)
char Block::getBlockShapeData(int shape, int angle, int row, int col) {
    if (shape < 0 || shape >= 7 || angle < 0 || angle >= 4 || row < 0 || row >= 4 || col < 0 || col >= 4) {
        return 0;
    }
    return shapes_[shape][angle][row][col];
}