#pragma once

#include "Board.h" // Board.h를 먼저 포함하는 것이 좋을 수 있습니다 (의존성 순서).
#include <array>

/* 하나의 블럭을 표현하고 회전, 이동 등의 동작을 담당하는 클래스 */
class Block {
public:
    Block(int shape); // 블럭 생성자: 모양 번호로 초기화
    void reset(int shape, int startX, int startY); // 블럭을 새로 설정하고 시작 위치로 초기화
    bool move(int dx, int dy, const Board& board); // 블럭을 (dx, dy)만큼 이동시도. 이동 가능하면 true
    void rotate(const Board& board); // 블럭을 회전시도. 가능하면 적용
    void rotate(); // 충돌 검사를 하지 않고, 무조건 회전 적용.

    // 블럭 상태 조회 (getter들)
    int getShape() const;
    int getAngle() const;
    int getX() const;
    int getY() const;

    // 4x4 그리드 내의 블록 모양 데이터 접근
    static char getBlockShapeData(int shape, int angle, int row, int col);

    // 7종 블럭의 회전 상태를 포함한 정적 데이터
    // 실제 게임에서는 이 데이터를 올바르게 정의해야 합니다.
    // 예시로 Block.cpp에 정의되어 있습니다.
    static const char shapes_[7][4][4][4]; // 선언만 하고 정의는 cpp 파일에서

private:
    int shape_; // 블럭 모양 종류 (0~6)
    int angle_; // 회전 상태 (0~3)
    int x_, y_; // 현재 위치 (블록 4x4 배열의 좌상단 기준 보드 좌표)
};