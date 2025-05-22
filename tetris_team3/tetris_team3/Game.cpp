#include "Game.h"
#include "Color.h"
#include <conio.h> 
#include <Windows.h>
#include <iostream>
#include <string>   
#include <algorithm>
#include <limits>

#define EXT_KEY         0xE0
#define KEY_LEFT        0x4B
#define KEY_RIGHT       0x4D
#define KEY_UP          0x48
#define KEY_DOWN        0x50
#define KEY_SPACE       0x20
#define KEY_ESC         0x1B

TetrisGame::TetrisGame()
    : currentBlock_(nullptr), nextBlock_(nullptr),
    level_(0), score_(0), lines_(0),
    renderer_(2, 1), // 게임 보드 좌상단 콘솔 오프셋 (X=2, Y=1)
    isGameOver_(false) {
}

TetrisGame::~TetrisGame() {
    delete currentBlock_;
    delete nextBlock_;
    currentBlock_ = nullptr;
    nextBlock_ = nullptr;
}

void TetrisGame::init() {
    srand(static_cast<unsigned>(time(nullptr)));

    renderer_.initConsole();
    renderer_.hideCursor();

    setupStages();
    inputData(); // 레벨 선택

    renderer_.initConsole(); // 레벨 선택 후 다시 화면 정리
    board_.init();

    renderer_.drawBoard(board_);
    // 통계 표시는 renderer_의 offsetX_를 기준으로 계산된 절대 위치에 그림
    renderer_.drawStats(level_, score_, stages_[level_].clearLine - lines_);


    if (!spawnBlock()) {
        isGameOver_ = true;
    }
    if (!isGameOver_ && nextBlock_) {
        renderer_.drawNextBlockArea(*nextBlock_);
    }
}

void TetrisGame::setupStages() {
    stages_ = {
        {40, 20, 2}, {35, 20, 2}, {30, 20, 15}, {25, 17, 20}, {20, 16, 25},
        {15, 14, 30}, {10, 14, 35}, {8, 13, 40},  {6, 12, 45}, {4, 11, 50}
    };
}

void TetrisGame::inputData() {
    int input = 0;
    renderer_.setColor(GRAY);
    do {
        // 보드 오른쪽 영역에 레벨 선택 프롬프트 표시
        // renderer_.getOffsetX()와 Board::COLS (셀 단위)를 사용하여 X 좌표 계산
        // Board::COLS * 2는 보드의 문자 너비
        int promptX = renderer_.getOffsetX() + Board::COLS * 2 + 3; // 보드 옆 약간의 간격
        int promptY = renderer_.getOffsetY() + 5; // 보드 상단에서 약간 아래

        renderer_.gotoXY(promptX, promptY);
        std::cout << "Select Start level [1-" << stages_.size() << "]: ";
        std::cin >> input;


        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(99999, '\n');
            input = 0;
            renderer_.gotoXY(promptX, promptY + 1);
            std::cout << "Invalid input. Please enter a number.";
            Sleep(1000);
            renderer_.initConsole();
            renderer_.gotoXY(promptX, promptY + 1);
            std::cout << "                                     ";
        }
        else if (input < 1 || input > static_cast<int>(stages_.size())) {
            renderer_.gotoXY(promptX, promptY + 1);
            std::cout << "Level out of range. Try again.   "; // 이전 메시지 덮도록 충분한 공백
            Sleep(1000);
            renderer_.gotoXY(promptX, promptY + 1);
            std::cout << "                                     ";
            input = 0;
        }

    } while (input < 1 || input > static_cast<int>(stages_.size()));

    level_ = input - 1;
}

void TetrisGame::run() {
    int tick = 0;

    renderer_.drawLogo();

    while (true) {
        init();
        isGameOver_ = 0;
        while (!isGameOver_) {
            if (_kbhit()) {
                processInput();
                if (isGameOver_) {
                    renderer_.initConsole();
                    renderer_.drawGameOver();
                    continue;
                }
            }

            int currentSpeed = stages_[min(level_, static_cast<int>(stages_.size()) - 1)].speed;
            if (tick % currentSpeed == 0) {
                update();
                if (isGameOver_) {
                    renderer_.initConsole();
                    renderer_.drawGameOver();
                    continue;
                }
            }

            int clearLineForLevelUp = stages_[min(level_, static_cast<int>(stages_.size()) - 1)].clearLine;
            if (lines_ >= clearLineForLevelUp && level_ < static_cast<int>(stages_.size()) - 1) {
                level_++;
                lines_ = 0;
                renderer_.drawStats(level_, score_, stages_[level_].clearLine - lines_);
            }

            Sleep(15);
            tick++;
        }
    }
}

bool TetrisGame::spawnBlock() {
    delete currentBlock_;

    // currentBlock_ 설정
    if (nextBlock_ == nullptr) {
        int shape = (rand() % 100 < stages_[level_].stickRate) ? 0 : (rand() % 6 + 1);
        currentBlock_ = new Block(shape);
        currentBlock_->reset(shape, Board::COLS / 2 - 2, 0);
    }
    else {
        currentBlock_ = nextBlock_;
        currentBlock_->reset(currentBlock_->getShape(), Board::COLS / 2 - 2, 0);
    }

    // nextBlock_ 설정
    int nextShape = (rand() % 100 < stages_[level_].stickRate) ? 0 : (rand() % 6 + 1);
    nextBlock_ = new Block(nextShape);
    // 위치는 필요 없고 모양만 전달 → reset 생략 가능

    if (board_.strikeCheck(currentBlock_->getShape(), currentBlock_->getAngle(), currentBlock_->getX(), currentBlock_->getY())) {
        // 게임 오버 상황에서는 currentBlock_을 그리지 않거나, 마지막 충돌 모습으로 그림
        // renderer_.drawBlock(*currentBlock_); // 옵션: 충돌하는 블록 표시
        return false;
    }

    if (currentBlock_) renderer_.drawBlock(*currentBlock_);
    if (nextBlock_) renderer_.drawNextBlockArea(*nextBlock_);

    return true;
}

void TetrisGame::processInput() {
    if (!currentBlock_ || isGameOver_) return;

    int key = _getche();

    if (key == EXT_KEY || key == 0) {
        key = _getche();
    }

    renderer_.eraseBlock(*currentBlock_);

    switch (key) {
    case KEY_LEFT:
        currentBlock_->move(-1, 0, board_);
        break;
    case KEY_RIGHT:
        currentBlock_->move(1, 0, board_);
        break;
    case KEY_UP:
        currentBlock_->rotate(board_);
        break;
    case KEY_DOWN:
        if (!currentBlock_->move(0, 1, board_)) {
            update();
        }
        else {
            renderer_.drawStats(level_, score_, stages_[level_].clearLine - lines_);
        }
        break;
    case KEY_SPACE:
        while (currentBlock_->move(0, 1, board_)) {
        }
        renderer_.drawStats(level_, score_, stages_[level_].clearLine - lines_);
        update();
        break;
    case KEY_ESC:
        isGameOver_ = true;
        break;
    }

    if (!isGameOver_ && currentBlock_) {
        renderer_.drawBlock(*currentBlock_);
    }
}

void TetrisGame::update() {
    if (!currentBlock_ || isGameOver_) return;

    renderer_.eraseBlock(*currentBlock_);

    if (!currentBlock_->move(0, 1, board_)) {
        bool mergedSuccessfully = board_.mergeBlock(currentBlock_->getShape(), currentBlock_->getAngle(), currentBlock_->getX(), currentBlock_->getY());

        // Y 좌표가 음수인 상태에서 병합 실패 또는 병합 자체가 실패하면 게임오버
        if (!mergedSuccessfully || (currentBlock_ != nullptr && currentBlock_->getY() < 0)) {
            isGameOver_ = true;
            if (currentBlock_) renderer_.drawBlock(*currentBlock_); // 마지막 모습 그림 (선택)
            delete currentBlock_; currentBlock_ = nullptr;
            delete nextBlock_; nextBlock_ = nullptr;
            return;
        }

        renderer_.drawBoard(board_); // 병합 후 보드 다시 그림

        int cleared = board_.clearFullLines();
        if (cleared > 0) {
            lines_ += cleared;
            switch (cleared) {
            case 1: score_ += 40 * (level_ + 1); break;
            case 2: score_ += 100 * (level_ + 1); break;
            case 3: score_ += 300 * (level_ + 1); break;
            case 4: score_ += 1200 * (level_ + 1); break;
            }
            renderer_.drawBoard(board_); // 라인 클리어 후 보드 다시 그리기
            renderer_.drawStats(level_, score_, stages_[level_].clearLine - lines_);
        }
        else {
            renderer_.drawStats(level_, score_, stages_[level_].clearLine - lines_);
        }

        if (!spawnBlock()) {
            isGameOver_ = true;
            delete currentBlock_; currentBlock_ = nullptr; // 안전하게 처리
            delete nextBlock_; nextBlock_ = nullptr;
            return;
        }

    }
    else {
        if (currentBlock_) renderer_.drawBlock(*currentBlock_);
    }
}