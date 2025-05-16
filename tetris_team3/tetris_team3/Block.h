#pragma once

#include "Board.h"
#include <array>

/* 하나의 블럭을 표현하고 회전, 이동 등의 동작을 담당하는 클래스 */
class Block {
public:
    Block(int shape); // 블럭 생성자: 모양 번호로 초기화
    void reset(int shape, int startX, int startY); // 블럭을 새로 설정하고 시작 위치로 초기화
    bool move(int dx, int dy, const Board& board); // 블럭을 (dx, dy)만큼 이동시도. 이동 가능하면 true
    void rotate(const Board& board); // 블럭을 회전시도. 가능하면 적용

    // 블럭 상태 조회 (getter들)
    int getShape() const;
    int getAngle() const;
    int getX() const;
    int getY() const;

private:
    int shape_; // 블럭 모양 종류 (0~6)
    int angle_; // 회전 상태 (0~3)
    int x_, y_; // 현재 위치

    // 7종 블럭의 회전 상태를 포함한 정적 데이터
    static const std::array<std::array<std::array<char, 4>, 4>, 7> shapes_;
};
