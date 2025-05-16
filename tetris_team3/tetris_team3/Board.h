#pragma once

#include <array>

/* 게임판의 상태(21x14 그리드)를 관리하고 
블럭 충돌, 병합, 라인 삭제 등의 로직을 처리하는 클래스 */

class Board {
public:
    static constexpr int ROWS = 21; // 세로 줄 수
    static constexpr int COLS = 14; // 가로 칸 수


    Board(); // 게임판을 초기화한다 (벽/바닥 생성 등)
    void init(); // 주어진 블럭 정보로 충돌 여부를 검사한다
    bool strikeCheck(int shape, int angle, int x, int y) const; // 블럭을 게임판에 병합한다 (충돌 시 false 반환)
    bool mergeBlock(int shape, int angle, int x, int y); // 가득 찬 줄을 제거하고 내려준다, 삭제한 줄 수 반환
    int clearFullLines(); // 가득 찬 줄을 제거하고 내려준다, 삭제한 줄 수 반환

    // 외부에서 게임판 그리드(읽기 전용)를 가져온다
    const std::array<std::array<char, COLS>, ROWS>& getGrid() const;

private:
    std::array<std::array<char, COLS>, ROWS> grid_; // 게임판 그리드 상태
};