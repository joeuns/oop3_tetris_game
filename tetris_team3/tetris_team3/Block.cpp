#include "Block.h"
#include "Board.h" // shapes_ 정의 시 Board 정보가 필요하진 않지만, Block 클래스가 Board를 사용하므로 포함

// 정적 블럭 모양 초기화 (7종, 4회전, 4x4)
// 중요: 이 모양들은 표준 테트리스를 기준으로 다시 정의해야 합니다.
// 아래는 I, O, T 모양에 대한 예시이며, 나머지도 유사하게 수정해야 합니다.
// 값 1은 블록이 채워진 칸, 0은 빈 칸을 의미합니다.
// 각 모양은 4x4 배열 내에서 상대적인 위치를 가집니다.
const char Block::shapes_[7][4][4][4] = {
    // Shape 0: I (막대)
    {
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} }, // 0도 (수평)
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }, // 90도 (수직)
        { {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0} }, // 180도 (수평) - SRS에서는 이 위치가 약간 다를 수 있음
        { {0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0} }  // 270도 (수직) - SRS에서는 이 위치가 약간 다를 수 있음
    },
    // Shape 1: O (네모)
    {
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }, // 모든 회전 동일
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }
    },
    // Shape 2: T
    {
        { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, // 0도
        { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} }, // 90도
        { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} }, // 180도
        { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }  // 270도
    },
    // Shape 3: L
     {
        { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} },
        { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }
    },
    // Shape 4: J (L의 대칭)
    {
        { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} }
    },
    // Shape 5: S
    {
        { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} }, // 180도 (0도와 같거나 약간 다름)
        { {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }  // 270도 (90도와 같거나 약간 다름)
    },
    // Shape 6: Z (S의 대칭)
    {
        { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} }, // 180도
        { {0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0} }  // 270도
    }
};

// 생성자: 블록 모양, 각도, 초기 위치 설정
// y_ 시작 위치는 블록이 화면 상단에서 내려오도록 음수 값으로 설정될 수 있습니다.
// Board::COLS / 2 - 2 는 대략 중앙에서 시작하도록 함 (블록 4x4 기준)
Block::Block(int shape) : shape_(shape), angle_(0), x_(Board::COLS / 2 - 2), y_(-2) {}

void Block::reset(int shape, int startX, int startY) {
    shape_ = shape;
    angle_ = 0;
    x_ = startX;
    y_ = startY;
}

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

void Block::rotate(const Board& board) {
    int newAngle = (angle_ + 1) % 4; // 0, 1, 2, 3 순환

    // Wall Kick offset 목록: 현재 위치에서 회전 가능한지 확인할 좌표 이동 후보들
    // 순서대로 시도됨
    const int offsetCount = 5;
    const int dx[offsetCount] = { 0, -1, 1, -2, 2 };
    const int dy[offsetCount] = { 0, 0, 0, 0, 0 };

    for (int i = 0; i < offsetCount; ++i) {
        int newX = x_ + dx[i];
        int newY = y_ + dy[i];

        if (!board.strikeCheck(shape_, newAngle, newX, newY)) {
            // 성공: 회전 적용
            angle_ = newAngle;
            x_ = newX;
            y_ = newY;
            return;
        }
    }

    // 회전 불가능하면 아무 일도 하지 않음
}


void Block::rotate() {
    int newAngle = (angle_ + 1) % 4; // 0, 1, 2, 3 순환
    angle_ = newAngle;
}

int Block::getShape() const {
    return shape_;
}

int Block::getAngle() const {
    return angle_;
}

int Block::getX() const {
    return x_;
}

int Block::getY() const {
    return y_;
}

char Block::getBlockShapeData(int shape, int angle, int row, int col) {
    if (shape < 0 || shape >= 7 || angle < 0 || angle >= 4 || row < 0 || row >= 4 || col < 0 || col >= 4) {
        return 0; // 잘못된 접근이면 빈 칸으로 처리
    }
    return shapes_[shape][angle][row][col];
}