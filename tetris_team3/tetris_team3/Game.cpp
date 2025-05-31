#define NOMINMAX

#include "Game.h"
#include "Color.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

TetrisGame::TetrisGame()
    : gameMode_(GameMode::ONE_PLAYER), startLevel_(0), currentStage_(0) {
    for (int i = 0; i < 2; ++i) {
        currentBlock_[i] = nullptr;
        nextBlock_[i] = nullptr;
        isGameOver_[i] = true;
        level_[i] = 0;
        score_[i] = 0;
        lines_[i] = 0;
        totalLinesCleared_[i] = 0;
        stageWins_[i] = 0;
        gameWon_[i] = false;
    }
}

TetrisGame::~TetrisGame() {
    for (int i = 0; i < 2; ++i) {
        delete currentBlock_[i];
        delete nextBlock_[i];
    }
}

int TetrisGame::getRandomShapeIndex(int playerIndex) {
    int shapeIdx;
    int currentLevelForStage = std::min(level_[playerIndex], static_cast<int>(stages_.size()) - 1);
    if (currentLevelForStage < 0) currentLevelForStage = 0;

    if (rand() % 100 < stages_[currentLevelForStage].stickRate) {
        shapeIdx = I_BLOCK_4_INDEX;
    }
    else {
        std::vector<int> otherShapeIndices;
        for (int i = 0; i < TOTAL_SHAPES; ++i) {
            if (i == I_BLOCK_4_INDEX) continue;
            otherShapeIndices.push_back(i);
        }
        if (otherShapeIndices.empty()) {
            shapeIdx = I_BLOCK_4_INDEX;
        }
        else {
            shapeIdx = otherShapeIndices[rand() % otherShapeIndices.size()];
        }
    }
    return shapeIdx;
}

void TetrisGame::initCommon() {
    srand(static_cast<unsigned>(time(nullptr)));
    stages_ = {
        {20, 15, 2},
        {15, 20, 7},
        {10, 25, 10}
    };
    startLevel_ = 0;
}

void TetrisGame::selectGameMode() {
    int selection = 1;
    system("mode con: cols=80 lines=25");
    system("cls");
    renderer_.hideCursor();
    renderer_.drawPlayerSelectionScreen(selection);
    gameMode_ = (selection == 1) ? GameMode::ONE_PLAYER : GameMode::TWO_PLAYER;
}

void TetrisGame::initPlayer(int playerIndex) {
    isGameOver_[playerIndex] = false;
    gameWon_[playerIndex] = false;
    level_[playerIndex] = startLevel_;
    score_[playerIndex] = 0;
    lines_[playerIndex] = 0;
    totalLinesCleared_[playerIndex] = 0;

    delete currentBlock_[playerIndex]; currentBlock_[playerIndex] = nullptr;
    delete nextBlock_[playerIndex];    nextBlock_[playerIndex] = nullptr;

    board_[playerIndex].init();
    renderer_.drawBoard(board_[playerIndex], playerIndex);

    nextBlock_[playerIndex] = new Block(getRandomShapeIndex(playerIndex));

    if (!spawnBlock(playerIndex)) {
        isGameOver_[playerIndex] = true;
    }
    int currentLevelForStage = std::min(level_[playerIndex], static_cast<int>(stages_.size()) - 1);
    if (currentLevelForStage < 0) currentLevelForStage = 0;

    renderer_.drawStats(level_[playerIndex], score_[playerIndex],
        stages_[currentLevelForStage].clearLine - lines_[playerIndex], playerIndex);
}

void TetrisGame::initPlayerForStage(int playerIndex, int stageIdx) {
    isGameOver_[playerIndex] = false;
    gameWon_[playerIndex] = false;
    level_[playerIndex] = stageIdx;
    lines_[playerIndex] = 0;

    board_[playerIndex].init();
    renderer_.drawBoard(board_[playerIndex], playerIndex);

    delete currentBlock_[playerIndex]; currentBlock_[playerIndex] = nullptr;
    delete nextBlock_[playerIndex];    nextBlock_[playerIndex] = nullptr;

    nextBlock_[playerIndex] = new Block(getRandomShapeIndex(playerIndex));

    if (!spawnBlock(playerIndex)) {
        isGameOver_[playerIndex] = true;
    }

    int currentLevelForStage = std::min(level_[playerIndex], static_cast<int>(stages_.size()) - 1);
    if (currentLevelForStage < 0) currentLevelForStage = 0;

    renderer_.drawStats(level_[playerIndex], score_[playerIndex],
        stages_[currentLevelForStage].clearLine - lines_[playerIndex], playerIndex);
}

void TetrisGame::run() {
    renderer_.drawLogo();
    int select = 1;
    renderer_.drawOptionSelectionScreen(select);
    selectGameMode();
    initCommon();

    renderer_.initConsole(gameMode_ == GameMode::TWO_PLAYER);

    if (gameMode_ == GameMode::ONE_PLAYER) {
        initPlayer(0);
        renderer_.gotoXY(renderer_.getPlayerBoardOffsetX(0) + Board::COLS - 5, renderer_.getPlayerBoardOffsetY(0) - 1);
        renderer_.setColor(YELLOW);
        std::cout << "Stage " << level_[0] + 1;
        Sleep(1500);
        renderer_.gotoXY(renderer_.getPlayerBoardOffsetX(0) + Board::COLS - 5, renderer_.getPlayerBoardOffsetY(0) - 1);
        std::cout << "          ";

        int tick = 0;
        while (!isGameOver_[0] && !gameWon_[0]) {
            if (_kbhit()) {
                processInput();
            }
            if (isGameOver_[0] || gameWon_[0]) break;

            int currentLevelForSpeed = std::min(level_[0], static_cast<int>(stages_.size()) - 1);
            if (currentLevelForSpeed < 0) currentLevelForSpeed = 0;

            int currentSpeedP0 = stages_[currentLevelForSpeed].speed + currentBlock_[0]->getBlockSpeed();
           
            if (tick % currentSpeedP0 == 0) {
                if (currentBlock_[0]) renderer_.eraseBlock(*currentBlock_[0], 0);
                updatePlayer(0);
            }
            Sleep(15);
            tick++;
        }
    }
    else {
        stageWins_[0] = 0;
        stageWins_[1] = 0;
        score_[0] = 0;
        score_[1] = 0;

        for (currentStage_ = 0; currentStage_ < stages_.size(); ++currentStage_) {
            system("cls");
            renderer_.initConsole(true);
            renderer_.hideCursor();

            std::string stageMsg = "STAGE " + std::to_string(currentStage_ + 1) + " START!";
            renderer_.gotoXY(60 - stageMsg.length() / 2, 5);
            renderer_.setColor(YELLOW);
            std::cout << stageMsg;
            Sleep(2000);
            renderer_.gotoXY(60 - stageMsg.length() / 2, 5);
            for (size_t i = 0; i < stageMsg.length(); ++i) std::cout << " ";

            initPlayerForStage(0, currentStage_);
            initPlayerForStage(1, currentStage_);

            int currentLvlP0ForStats = std::min(level_[0], static_cast<int>(stages_.size()) - 1); if (currentLvlP0ForStats < 0) currentLvlP0ForStats = 0;
            int currentLvlP1ForStats = std::min(level_[1], static_cast<int>(stages_.size()) - 1); if (currentLvlP1ForStats < 0) currentLvlP1ForStats = 0;

            renderer_.drawStats(level_[0], score_[0], stages_[currentLvlP0ForStats].clearLine - lines_[0], 0);
            renderer_.drawStats(level_[1], score_[1], stages_[currentLvlP1ForStats].clearLine - lines_[1], 1);

            int tick = 0;
            while (!isGameOver_[0] || !isGameOver_[1]) {
                if (_kbhit()) {
                    processInput();
                }

                int currentLvlP0ForSpeed = std::min(level_[0], static_cast<int>(stages_.size()) - 1); if (currentLvlP0ForSpeed < 0) currentLvlP0ForSpeed = 0;
                int currentLvlP1ForSpeed = std::min(level_[1], static_cast<int>(stages_.size()) - 1); if (currentLvlP1ForSpeed < 0) currentLvlP1ForSpeed = 0;

                if (!isGameOver_[0]) {
                    int currentSpeedP0 = stages_[currentLvlP0ForSpeed].speed + currentBlock_[0]->getBlockSpeed();
                    if (tick % currentSpeedP0 == 0) {
                        if (currentBlock_[0]) renderer_.eraseBlock(*currentBlock_[0], 0);
                        updatePlayer(0);
                    }
                }

                if (!isGameOver_[1]) {
                    int currentSpeedP1 = stages_[currentLvlP1ForSpeed].speed + currentBlock_[1]->getBlockSpeed();
                    if (tick % currentSpeedP1 == 0) {
                        if (currentBlock_[1]) renderer_.eraseBlock(*currentBlock_[1], 1);
                        updatePlayer(1);
                    }
                }

                Sleep(15);
                tick++;

                if (isGameOver_[0] && isGameOver_[1]) break;
            }

            if (isGameOver_[0] && !isGameOver_[1]) {
                stageWins_[1]++;
            }
            else if (!isGameOver_[0] && isGameOver_[1]) {
                stageWins_[0]++;
            }
            else {
                if (score_[0] > score_[1]) {
                    stageWins_[0]++;
                }
                else if (score_[1] > score_[0]) {
                    stageWins_[1]++;
                }
            }

            system("cls");
            renderer_.hideCursor();
            std::string stageResultMsg = "Stage " + std::to_string(currentStage_ + 1) + " Ended!";
            renderer_.gotoXY(60 - stageResultMsg.length() / 2, 8); renderer_.setColor(WHITE); std::cout << stageResultMsg;

            renderer_.gotoXY(45, 11); std::cout << "Player 1 Score: " << std::setw(7) << score_[0];
            renderer_.gotoXY(45, 12); std::cout << "Player 2 Score: " << std::setw(7) << score_[1];

            renderer_.gotoXY(45, 14); renderer_.setColor(YELLOW);
            if (isGameOver_[0] && !isGameOver_[1]) std::cout << "Player 2 wins Stage " << currentStage_ + 1 << "!";
            else if (!isGameOver_[0] && isGameOver_[1]) std::cout << "Player 1 wins Stage " << currentStage_ + 1 << "!";
            else {
                if (score_[0] > score_[1]) std::cout << "Player 1 wins Stage " << currentStage_ + 1 << " (Score)!";
                else if (score_[1] > score_[0])  std::cout << "Player 2 wins Stage " << currentStage_ + 1 << " (Score)!";
                else std::cout << "Stage " << currentStage_ + 1 << " is a Draw!";
            }

            renderer_.gotoXY(45, 16); renderer_.setColor(GRAY);
            std::cout << "Total Stage Wins -> P1: " << stageWins_[0] << " | P2: " << stageWins_[1];
            renderer_.gotoXY(45, 18); std::cout << "Press any key to continue...";
            _getch();
        }
    }

    system("cls");
    renderer_.hideCursor();
    int centerX = (gameMode_ == GameMode::ONE_PLAYER) ? 40 : 60;

    if (gameMode_ == GameMode::ONE_PLAYER) {
        if (gameWon_[0]) {
            renderer_.setColor(GREEN);
            std::string msg1 = "CONGRATULATIONS!";
            std::string msg2 = "ALL STAGES CLEARED!";
            std::string msg3 = "Final Score: " + std::to_string(score_[0]);
            renderer_.gotoXY(centerX - msg1.length() / 2, 10); std::cout << msg1;
            renderer_.gotoXY(centerX - msg2.length() / 2, 12); std::cout << msg2;
            renderer_.gotoXY(centerX - msg3.length() / 2, 14); std::cout << msg3;
        }
        else {
            renderer_.setColor(RED);
            std::string msg1 = "G A M E  O V E R";
            std::string msg2 = "Score: " + std::to_string(score_[0]);
            std::string msg3 = "Stage: " + std::to_string(level_[0] + 1);
            renderer_.gotoXY(centerX - msg1.length() / 2, 10); std::cout << msg1;
            renderer_.gotoXY(centerX - msg2.length() / 2, 12); std::cout << msg2;
            renderer_.gotoXY(centerX - msg3.length() / 2, 13); std::cout << msg3;
        }
    }
    else {
        renderer_.gotoXY(centerX - 10, 8); renderer_.setColor(WHITE);
        std::cout << "--- GAME FINISHED ---";

        renderer_.gotoXY(centerX - 20, 10);
        std::cout << "Player 1 Total Score: " << std::setw(7) << score_[0] << "  |  Stages Won: " << stageWins_[0];
        renderer_.gotoXY(centerX - 20, 12);
        std::cout << "Player 2 Total Score: " << std::setw(7) << score_[1] << "  |  Stages Won: " << stageWins_[1];

        renderer_.gotoXY(centerX - 15, 15); renderer_.setColor(YELLOW);
        if (stageWins_[0] > stageWins_[1]) {
            std::cout << "PLAYER 1 IS THE OVERALL WINNER!";
        }
        else if (stageWins_[1] > stageWins_[0]) {
            std::cout << "PLAYER 2 IS THE OVERALL WINNER!";
        }
        else {
            if (score_[0] > score_[1]) {
                std::cout << "PLAYER 1 WINS ON SCORE TIE-BREAK!";
            }
            else if (score_[1] > score_[0]) {
                std::cout << "PLAYER 2 WINS ON SCORE TIE-BREAK!";
            }
            else {
                renderer_.setColor(GRAY); std::cout << "OVERALL GAME IS A DRAW!";
            }
        }
    }

    renderer_.setColor(GRAY);
    std::string continueMsg = "Press any key to continue...";
    renderer_.gotoXY(centerX - continueMsg.length() / 2, 20);
    std::cout << continueMsg;

    while (!_kbhit()) { Sleep(100); }
    _getche();
    system("cls");
}

bool TetrisGame::spawnBlock(int playerIndex) {
    delete currentBlock_[playerIndex];
    currentBlock_[playerIndex] = nextBlock_[playerIndex];

    if (currentBlock_[playerIndex] == nullptr) {
        currentBlock_[playerIndex] = new Block(getRandomShapeIndex(playerIndex));
    }
    currentBlock_[playerIndex]->reset(currentBlock_[playerIndex]->getShape(), Board::COLS / 2 - 2, 0);

    nextBlock_[playerIndex] = new Block(getRandomShapeIndex(playerIndex));

    if (board_[playerIndex].strikeCheck(currentBlock_[playerIndex]->getShape(), currentBlock_[playerIndex]->getAngle(),
        currentBlock_[playerIndex]->getX(), currentBlock_[playerIndex]->getY())) {
        if (currentBlock_[playerIndex]) renderer_.drawBlock(*currentBlock_[playerIndex], playerIndex);
        delete nextBlock_[playerIndex];
        nextBlock_[playerIndex] = nullptr;
        renderer_.drawPlayerMessage(playerIndex, "GAME OVER");
        return false;
    }

    if (currentBlock_[playerIndex]) renderer_.drawBlock(*currentBlock_[playerIndex], playerIndex);
    if (nextBlock_[playerIndex]) renderer_.drawNextBlockArea(*nextBlock_[playerIndex], playerIndex);
    return true;
}

void TetrisGame::processInput() {
    int ch = _getch();
    PlayerAction p1Action = PlayerAction::NONE;
    PlayerAction p2Action = PlayerAction::NONE;

    if (gameMode_ == GameMode::ONE_PLAYER) {
        if (!isGameOver_[0] && !gameWon_[0]) {
            if (ch == KeyCodes::EXT_KEY || ch == 0) {
                ch = _getch();
                if (ch == KeyCodes::P1_KEY_LEFT_ARROW) p1Action = PlayerAction::MOVE_LEFT;
                else if (ch == KeyCodes::P1_KEY_RIGHT_ARROW) p1Action = PlayerAction::MOVE_RIGHT;
                else if (ch == KeyCodes::P1_KEY_UP_ARROW) p1Action = PlayerAction::ROTATE;
                else if (ch == KeyCodes::P1_KEY_DOWN_ARROW) p1Action = PlayerAction::SOFT_DROP;
            }
            else if (ch == KeyCodes::P1_KEY_SPACE) p1Action = PlayerAction::HARD_DROP;
        }
    }
    else if (gameMode_ == GameMode::TWO_PLAYER) {
        if (!isGameOver_[0] && !gameWon_[0]) {
            char char_ch = static_cast<char>(std::tolower(ch));
            if (char_ch == KeyCodes::P2_KEY_LEFT) p1Action = PlayerAction::MOVE_LEFT;
            else if (char_ch == KeyCodes::P2_KEY_RIGHT) p1Action = PlayerAction::MOVE_RIGHT;
            else if (char_ch == KeyCodes::P2_KEY_UP) p1Action = PlayerAction::ROTATE;
            else if (char_ch == KeyCodes::P2_KEY_DOWN) p1Action = PlayerAction::SOFT_DROP;
            else if (char_ch == KeyCodes::P2_KEY_TAB) p1Action = PlayerAction::HARD_DROP;
        }

        if (!isGameOver_[1] && !gameWon_[1]) {
            if (ch == KeyCodes::EXT_KEY || ch == 0) {
                ch = _getch();
                if (ch == KeyCodes::P1_KEY_LEFT_ARROW) p2Action = PlayerAction::MOVE_LEFT;
                else if (ch == KeyCodes::P1_KEY_RIGHT_ARROW) p2Action = PlayerAction::MOVE_RIGHT;
                else if (ch == KeyCodes::P1_KEY_UP_ARROW) p2Action = PlayerAction::ROTATE;
                else if (ch == KeyCodes::P1_KEY_DOWN_ARROW) p2Action = PlayerAction::SOFT_DROP;
            }
            else if (ch == KeyCodes::P1_KEY_SPACE) p2Action = PlayerAction::HARD_DROP;
        }
    }

    if (p1Action != PlayerAction::NONE) {
        if (currentBlock_[0]) renderer_.eraseBlock(*currentBlock_[0], 0);
        handlePlayerAction(0, p1Action);
    }
    if (gameMode_ == GameMode::TWO_PLAYER && p2Action != PlayerAction::NONE) {
        if (currentBlock_[1]) renderer_.eraseBlock(*currentBlock_[1], 1);
        handlePlayerAction(1, p2Action);
    }
}

void TetrisGame::handlePlayerAction(int playerIndex, PlayerAction action) {
    if (!currentBlock_[playerIndex] || isGameOver_[playerIndex] || gameWon_[playerIndex]) return;

    bool shouldRedrawBlock = true;

    switch (action) {
    case PlayerAction::MOVE_LEFT:  currentBlock_[playerIndex]->move(-1, 0, board_[playerIndex]); break;
    case PlayerAction::MOVE_RIGHT: currentBlock_[playerIndex]->move(1, 0, board_[playerIndex]);  break;
    case PlayerAction::ROTATE:     currentBlock_[playerIndex]->rotate(board_[playerIndex]);      break;
    case PlayerAction::SOFT_DROP:
        if (!currentBlock_[playerIndex]->move(0, 1, board_[playerIndex])) {
            updatePlayer(playerIndex);
            shouldRedrawBlock = false;
        }
        break;
    case PlayerAction::HARD_DROP:
        while (currentBlock_[playerIndex]->move(0, 1, board_[playerIndex])) {}
        updatePlayer(playerIndex);
        shouldRedrawBlock = false;
        break;
    case PlayerAction::NONE:
        shouldRedrawBlock = false;
        break;
    }

    if (shouldRedrawBlock && !isGameOver_[playerIndex] && !gameWon_[playerIndex] && currentBlock_[playerIndex]) {
        renderer_.drawBlock(*currentBlock_[playerIndex], playerIndex);
    }
}

void TetrisGame::updatePlayer(int playerIndex) {
    if (!currentBlock_[playerIndex] || isGameOver_[playerIndex] || gameWon_[playerIndex]) return;

    if (!currentBlock_[playerIndex]->move(0, 1, board_[playerIndex])) {
        bool mergedSuccessfully = board_[playerIndex].mergeBlock(
            currentBlock_[playerIndex]->getShape(), currentBlock_[playerIndex]->getAngle(),
            currentBlock_[playerIndex]->getX(), currentBlock_[playerIndex]->getY());

        if (!mergedSuccessfully || currentBlock_[playerIndex]->getY() < 0) {
            isGameOver_[playerIndex] = true;
            if (currentBlock_[playerIndex]) renderer_.drawBlock(*currentBlock_[playerIndex], playerIndex);
            renderer_.drawPlayerMessage(playerIndex, "GAME OVER!");
            return;
        }
        renderer_.drawBoard(board_[playerIndex], playerIndex);

        int cleared = board_[playerIndex].clearFullLines();
        if (cleared > 0) {
            lines_[playerIndex] += cleared;
            totalLinesCleared_[playerIndex] += cleared;
            int currentLevelForScore = std::min(level_[playerIndex], static_cast<int>(stages_.size()) - 1); if (currentLevelForScore < 0) currentLevelForScore = 0;
            int scoreMultiplier = (currentLevelForScore + 1);
            switch (cleared) {
            case 1: score_[playerIndex] += 40 * scoreMultiplier; break;
            case 2: score_[playerIndex] += 100 * scoreMultiplier; break;
            case 3: score_[playerIndex] += 300 * scoreMultiplier; break;
            case 4: score_[playerIndex] += 1200 * scoreMultiplier; break;
            default: score_[playerIndex] += (1200 + (cleared - 4) * 300) * scoreMultiplier; break;
            }
            renderer_.drawBoard(board_[playerIndex], playerIndex);
        }
        checkLevelUp(playerIndex);

        if (isGameOver_[playerIndex] || gameWon_[playerIndex]) {
            return;
        }
        if (!spawnBlock(playerIndex)) {
            isGameOver_[playerIndex] = true;
        }
    }
    else {
        if (currentBlock_[playerIndex]) {
            renderer_.drawBlock(*currentBlock_[playerIndex], playerIndex);
        }
    }

    if (!isGameOver_[playerIndex] && !gameWon_[playerIndex]) {
        int currentLevelForStats = std::min(level_[playerIndex], static_cast<int>(stages_.size()) - 1); if (currentLevelForStats < 0) currentLevelForStats = 0;
        int linesNeeded = stages_[currentLevelForStats].clearLine;
        int linesRemaining = std::max(0, linesNeeded - lines_[playerIndex]);
        renderer_.drawStats(level_[playerIndex], score_[playerIndex], linesRemaining, playerIndex);
    }
}

void TetrisGame::checkLevelUp(int playerIndex) {
    if (isGameOver_[playerIndex] || gameWon_[playerIndex]) return;

    int currentStageIdx = level_[playerIndex];
    if (currentStageIdx < 0 || currentStageIdx >= static_cast<int>(stages_.size())) return;

    int linesNeededForStageClear = stages_[currentStageIdx].clearLine;

    if (lines_[playerIndex] >= linesNeededForStageClear) {
        if (gameMode_ == GameMode::ONE_PLAYER) {
            level_[playerIndex]++;
            lines_[playerIndex] = 0;

            if (level_[playerIndex] < static_cast<int>(stages_.size())) {
                board_[playerIndex].init();
                renderer_.drawBoard(board_[playerIndex], playerIndex);

                std::string stageClearMsg = "Stage " + std::to_string(currentStageIdx + 1) + " Cleared!";
                std::string nextStageMsg = "Stage " + std::to_string(level_[playerIndex] + 1) + " Start!";
                int msgX = renderer_.getPlayerBoardOffsetX(playerIndex) + Board::COLS - (stageClearMsg.length() / 2);
                int msgY = renderer_.getPlayerBoardOffsetY(playerIndex) + Board::ROWS / 2 - 1;

                renderer_.gotoXY(msgX, msgY); renderer_.setColor(GREEN); std::cout << stageClearMsg;
                renderer_.gotoXY(msgX - (nextStageMsg.length() - stageClearMsg.length()) / 2, msgY + 1); renderer_.setColor(YELLOW); std::cout << nextStageMsg;
                Sleep(2500);

                renderer_.gotoXY(msgX - (nextStageMsg.length() - stageClearMsg.length()) / 2 - 2, msgY);     std::cout << std::string(std::max(stageClearMsg.length(), nextStageMsg.length()) + 5, ' ');
                renderer_.gotoXY(msgX - (nextStageMsg.length() - stageClearMsg.length()) / 2 - 2, msgY + 1); std::cout << std::string(std::max(stageClearMsg.length(), nextStageMsg.length()) + 5, ' ');
            }
            else {
                gameWon_[playerIndex] = true;
                isGameOver_[playerIndex] = true;
            }
        }
    }
    if (!isGameOver_[playerIndex] && !gameWon_[playerIndex]) {
        int currentLevelForDisplay = std::min(level_[playerIndex], static_cast<int>(stages_.size()) - 1); if (currentLevelForDisplay < 0) currentLevelForDisplay = 0;
        int linesNeeded = stages_[currentLevelForDisplay].clearLine;
        int linesRemaining = std::max(0, linesNeeded - lines_[playerIndex]);
        renderer_.drawStats(level_[playerIndex], score_[playerIndex], linesRemaining, playerIndex);
    }
}