#pragma once

#include <vector>
#include <cstdlib>  // rand, srand
#include <ctime>
#include "Board.h"
#include "Block.h"
#include "Renderer.h"

// 스테이지 정보 구조체
struct Stage {
    int speed; // 스테이지 속도 (낮을수록 빠름)
    int stickRate; // 막대 블럭 등장 확률 (%)
    int clearLine; // 해당 스테이지 클리어 조건 (줄 수)
};

// 전체 게임의 흐름과 상태를 관리하는 클래스
class TetrisGame {
public:
    TetrisGame();
    void init();  // 게임 전체 초기화 (랜덤 시드, 레벨 선택, 콘솔 초기화 등)
    void run();

private:
    Board board_; // 게임판
    Renderer renderer_; // 화면 출력
    Block* currentBlock_; // 현재 블럭 (동적 할당)
    Block* nextBlock_; // 다음 블럭 (동적 할당)

    std::vector<Stage> stages_;  // 각 스테이지 정보
    int level_;
    int score_;
    int lines_;

    void setupStages(); // 스테이지 설정
    void inputData(); // 사용자 입력 처리 (레벨 선택 등)
    void spawnBlock(); // 새 블럭 생성
    void processInput(); // 키 입력 처리
    void update(); // 상태 갱신 (충돌, 병합, 라인 등)
};