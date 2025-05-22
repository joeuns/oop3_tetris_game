#pragma once

#include <array>
#include <vector> // clearFullLines에서 vector를 사용할 경우 필요 (현재는 미사용)

/* 게임판의 상태(21x14 그리드)를 관리하고
블럭 충돌, 병합, 라인 삭제 등의 로직을 처리하는 클래스 */

class Board {
public:
    static constexpr int ROWS = 21; // 세로 줄 수 (0~20)
    static constexpr int COLS = 14; // 가로 칸 수 (0~13)

    // grid_ 셀 상태 (예시)
    static constexpr char EMPTY_CELL = 0;
    static constexpr char WALL_CELL = 1; // 벽 또는 이미 쌓인 블록
    // 필요하다면 다른 블록 종류를 나타내는 값을 사용할 수 있습니다.

    Board();
    void init(); // 게임판을 초기화한다 (벽/바닥 생성 등)
    bool strikeCheck(int shape, int angle, int x, int y) const; // 주어진 블럭 정보로 충돌 여부를 검사한다
    bool mergeBlock(int shape, int angle, int x, int y); // 블럭을 게임판에 병합한다
    int clearFullLines(); // 가득 찬 줄을 제거하고 내려준다, 삭제한 줄 수 반환

    // 외부에서 게임판 그리드(읽기 전용)를 가져온다
    const std::array<std::array<char, COLS>, ROWS>& getGrid() const;

private:
    std::array<std::array<char, COLS>, ROWS> grid_; // 게임판 그리드 상태
};