#include "Renderer.h"
#include "Color.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <iomanip>

Renderer::Renderer() {}

int Renderer::getPlayerBoardOffsetX(int playerIndex) const {
    return (playerIndex == 0) ? P1_BOARD_OFFSET_X : P2_BOARD_OFFSET_X;
}
int Renderer::getPlayerBoardOffsetY(int playerIndex) const {
    return (playerIndex == 0) ? P1_BOARD_OFFSET_Y : P2_BOARD_OFFSET_Y;
}
int Renderer::getPlayerNextAreaBaseX(int playerIndex) const {
    return getPlayerBoardOffsetX(playerIndex) + ((playerIndex == 0) ? P1_NEXT_X_REL : P2_NEXT_X_REL);
}
int Renderer::getPlayerNextAreaBaseY(int playerIndex) const {
    return (playerIndex == 0) ? P1_NEXT_Y_ABS : P2_NEXT_Y_ABS;
}
int Renderer::getPlayerStatsAreaBaseX(int playerIndex) const {
    return getPlayerBoardOffsetX(playerIndex) + ((playerIndex == 0) ? P1_STATS_X_REL : P2_STATS_X_REL);
}
int Renderer::getPlayerStatsAreaBaseY(int playerIndex) const {
    return (playerIndex == 0) ? P1_STATS_Y_ABS : P2_STATS_Y_ABS;
}

void Renderer::initConsole(bool isTwoPlayerMode) {
    if (isTwoPlayerMode) {
        system("mode con: cols=120 lines=30");
    }
    else {
        system("mode con: cols=80 lines=25");
    }
    system("cls");
    hideCursor();
}

void Renderer::hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void Renderer::gotoXY(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hConsole, pos);
}

void Renderer::setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Renderer::drawBoard(const Board& board, int playerIndex) {
    const auto& grid = board.getGrid();
    int currentOffsetX = getPlayerBoardOffsetX(playerIndex);
    int currentOffsetY = getPlayerBoardOffsetY(playerIndex);

    for (int i = 0; i < Board::ROWS; ++i) {
        for (int j = 0; j < Board::COLS; ++j) {
            gotoXY(j * 2 + currentOffsetX, i + currentOffsetY);
            char cell = grid[i][j];
            if (cell == Board::WALL_CELL) {
                setColor((j == 0 || j == Board::COLS - 1 || i == Board::ROWS - 1) ? GRAY : DARK_GRAY);
                std::cout << "■";
            }
            else {
                setColor(BLACK);
                std::cout << "  ";
            }
        }
    }
    setColor(BLACK);
}

void Renderer::drawBlock(const Block& block, int playerIndex, int customOffsetX, int customOffsetY) {
    int shape = block.getShape();
    int angle = block.getAngle();
    int blockBoardX = block.getX();
    int blockBoardY = block.getY();

    int baseBoardRenderX = getPlayerBoardOffsetX(playerIndex);
    int baseBoardRenderY = getPlayerBoardOffsetY(playerIndex);

    int renderPosX, renderPosY;

    if (customOffsetX != -1 && customOffsetY != -1) {
        renderPosX = customOffsetX;
        renderPosY = customOffsetY;
    }
    else {
        renderPosX = blockBoardX * 2 + baseBoardRenderX;
        renderPosY = blockBoardY + baseBoardRenderY;
    }

    int blockColor;
    switch (shape) {
    case 0:  blockColor = WHITE; break;
    case 1:  blockColor = GRAY; break;
    case 2:  blockColor = DARK_GREEN; break;
    case 3:  blockColor = DARK_SKY_BLUE; break;
    case 4:  blockColor = RED; break;
    case 5:  blockColor = YELLOW; break;
    case 6:  blockColor = VIOLET; break;
    case 7:  blockColor = BLUE; break;
    case 8:  blockColor = DARK_YELLOW; break;
    case 9:  blockColor = GREEN; break;
    case 10: blockColor = SKY_BLUE; break;
    case 11: blockColor = DARK_RED; break;
    case 12: blockColor = DARK_BLUE; break;
    case 13: blockColor = DARK_VIOLET; break;
    case 14: blockColor = GREEN; break;
    case 15: blockColor = YELLOW; break;
    case 16: blockColor = RED; break;
    case 17: blockColor = BLUE; break;
    case 18: blockColor = WHITE; break;
    case 19: blockColor = VIOLET; break;
    case 20: blockColor = SKY_BLUE; break;
    default: blockColor = GRAY; break;
    }
    setColor(blockColor);

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                int screenX = renderPosX + c * 2;
                int screenY = renderPosY + r;

                if (customOffsetX != -1 || (blockBoardY + r >= 0)) {
                    if (customOffsetX != -1 || screenY >= baseBoardRenderY) {
                        gotoXY(screenX, screenY);
                        std::cout << "■";
                    }
                }
            }
        }
    }
    setColor(BLACK);
}

void Renderer::eraseBlock(const Block& block, int playerIndex, int customOffsetX, int customOffsetY) {
    int shape = block.getShape();
    int angle = block.getAngle();
    int blockBoardX = block.getX();
    int blockBoardY = block.getY();

    int baseBoardRenderX = getPlayerBoardOffsetX(playerIndex);
    int baseBoardRenderY = getPlayerBoardOffsetY(playerIndex);

    int renderPosX, renderPosY;

    if (customOffsetX != -1 && customOffsetY != -1) {
        renderPosX = customOffsetX;
        renderPosY = customOffsetY;
    }
    else {
        renderPosX = blockBoardX * 2 + baseBoardRenderX;
        renderPosY = blockBoardY + baseBoardRenderY;
    }

    setColor(BLACK);

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                int screenX = renderPosX + c * 2;
                int screenY = renderPosY + r;
                if (customOffsetX != -1 || (blockBoardY + r >= 0)) {
                    if (customOffsetX != -1 || screenY >= baseBoardRenderY) {
                        gotoXY(screenX, screenY);
                        std::cout << "  ";
                    }
                }
            }
        }
    }
    setColor(BLACK);
}

void Renderer::drawNextBlockArea(const Block& nextBlock, int playerIndex) {
    setColor(GRAY);
    int boxScreenX = getPlayerNextAreaBaseX(playerIndex);
    int boxScreenY = getPlayerNextAreaBaseY(playerIndex);

    for (int i = 0; i < NEXT_BLOCK_BOX_HEIGHT; ++i) {
        gotoXY(boxScreenX, boxScreenY + i);
        for (int j = 0; j < NEXT_BLOCK_BOX_WIDTH; ++j) {
            if (i == 0 || i == NEXT_BLOCK_BOX_HEIGHT - 1 || j == 0 || j == NEXT_BLOCK_BOX_WIDTH - 1) {
                std::cout << "□ ";
            }
            else {
                std::cout << "  ";
            }
        }
    }
    gotoXY(boxScreenX + (NEXT_BLOCK_BOX_WIDTH * 2 - 4) / 2 - 1, boxScreenY - 1);
    std::cout << "NEXT";

    int blockDisplayAreaX = boxScreenX + 2;
    int blockDisplayAreaY = boxScreenY + 1;
    setColor(BLACK);
    for (int r = 0; r < 4; ++r) {
        gotoXY(blockDisplayAreaX, blockDisplayAreaY + r);
        for (int c = 0; c < 4 * 2; ++c) {
            std::cout << " ";
        }
    }

    int blockRenderScreenX = boxScreenX + (NEXT_BLOCK_BOX_WIDTH * 2 - 4 * 2) / 2 + 2;
    int blockRenderScreenY = boxScreenY + (NEXT_BLOCK_BOX_HEIGHT - 4) / 2 + 1;

    int currentShape = nextBlock.getShape();
    if (currentShape == 4 || currentShape == 2) {
        drawBlock(nextBlock, playerIndex, blockRenderScreenX - 2, blockRenderScreenY);
    }
    else if (currentShape == 1) {
        drawBlock(nextBlock, playerIndex, blockRenderScreenX - 1, blockRenderScreenY);
    }
    else if (currentShape == 0) {
        drawBlock(nextBlock, playerIndex, blockRenderScreenX, blockRenderScreenY + 1);
    }
    else if (currentShape == 19) {
        drawBlock(nextBlock, playerIndex, blockRenderScreenX - 1, blockRenderScreenY - 1);
    }
    else {
        drawBlock(nextBlock, playerIndex, blockRenderScreenX - 1, blockRenderScreenY);
    }
}

void Renderer::drawStats(int level, int score, int linesToClear, int playerIndex) {
    setColor(GRAY);
    int baseStatsScreenX = getPlayerStatsAreaBaseX(playerIndex);
    int baseStatsScreenY = getPlayerStatsAreaBaseY(playerIndex);

    gotoXY(baseStatsScreenX + STATS_LABEL_X_LEVEL, baseStatsScreenY);
    std::cout << "STAGE: ";
    std::cout << std::setw(3) << std::left << (level + 1) << "   ";

    gotoXY(baseStatsScreenX + STATS_LABEL_X_LEVEL, baseStatsScreenY + STATS_Y_OFFSET_SCORE);
    std::cout << "SCORE: ";
    std::cout << std::setw(7) << std::left << score << "   ";

    gotoXY(baseStatsScreenX + STATS_LABEL_X_LEVEL, baseStatsScreenY + STATS_Y_OFFSET_LINES);
    std::cout << "LINES: ";
    std::cout << std::setw(3) << std::left << linesToClear << "   ";

    setColor(BLACK);
}

void Renderer::drawLogo() {
    setColor(WHITE);
    gotoXY(13, 3);  std::cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    gotoXY(13, 4);  std::cout << "┃◆ ◆ ◆  ◆◆◆  ◆ ◆ ◆   ◆◆     ◆   ◆ ◆ ◆         ┃";
    gotoXY(13, 5);  std::cout << "┃  ◆    ◆      ◆     ◆ ◆    ◆   ◆             ┃";
    gotoXY(13, 6);  std::cout << "┃  ◆    ◆◆◆    ◆     ◆◆     ◆     ◆           ┃";
    gotoXY(13, 7);  std::cout << "┃  ◆    ◆      ◆     ◆ ◆    ◆       ◆         ┃";
    gotoXY(13, 8);  std::cout << "┃  ◆    ◆◆◆    ◆     ◆  ◆   ◆   ◆ ◆ ◆         ┃";
    gotoXY(13, 9);  std::cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
    gotoXY(28, 20); std::cout << "Please Press Any Key~!";

    int logoBlocksBaseX = 20;
    int logoBlocksBaseY = 14;

    const int totalShapesForLogo = 21;

    for (int i = 0; ; ++i) {
        if (i % 40 == 0) {
            setColor(BLACK);
            for (int r = 0; r < 5; ++r) {
                gotoXY(logoBlocksBaseX - 10, logoBlocksBaseY + r - 1);
                for (int c = 0; c < 50; ++c) {
                    std::cout << " ";
                }
            }

            Block b1(rand() % totalShapesForLogo); Block b2(rand() % totalShapesForLogo);
            Block b3(rand() % totalShapesForLogo); Block b4(rand() % totalShapesForLogo);

            b1.reset(b1.getShape(), 0, 0); b2.reset(b2.getShape(), 0, 0);
            b3.reset(b3.getShape(), 0, 0); b4.reset(b4.getShape(), 0, 0);

            for (int k = 0; k < rand() % 4; k++) b1.rotate();
            for (int k = 0; k < rand() % 4; k++) b2.rotate();
            for (int k = 0; k < rand() % 4; k++) b3.rotate();
            for (int k = 0; k < rand() % 4; k++) b4.rotate();

            drawBlock(b1, 0, logoBlocksBaseX + 0 * 10, logoBlocksBaseY);
            drawBlock(b2, 0, logoBlocksBaseX + 1 * 10, logoBlocksBaseY);
            drawBlock(b3, 0, logoBlocksBaseX + 2 * 10, logoBlocksBaseY);
            drawBlock(b4, 0, logoBlocksBaseX + 3 * 10, logoBlocksBaseY);
        }
        if (_kbhit()) break;
        Sleep(30);
    }
    _getche();
}

void Renderer::drawGameOver() {
    int consoleWidth = 80;
    int centerX = consoleWidth / 2;
    int centerY = 12;
    std::vector<std::string> gameOverMsg = {
        "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
        "┃      G A M E  O V E R      ┃",
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
    };
    std::string pressKeyMsg = "Handled by Game loop. Press key...";

    setColor(RED);
    for (size_t i = 0; i < gameOverMsg.size(); ++i) {
        gotoXY(centerX - gameOverMsg[i].length() / 2, centerY + i - 1);
        std::cout << gameOverMsg[i];
    }
    setColor(YELLOW);
    gotoXY(centerX - pressKeyMsg.length() / 2, centerY + gameOverMsg.size() + 1);
    std::cout << pressKeyMsg;
}

void Renderer::drawPlayerSelectionScreen(int& selection) {
    int currentSelection = 1;
    const int numChoices = 2;
    int inputKey;

    std::string title = "SELECT GAME MODE";
    std::string choice1 = "1 PLAYER";
    std::string choice2 = "2 PLAYERS";

    int titleX = (80 - title.length()) / 2;
    int choiceX = titleX + 1;

    while (true) {
        gotoXY(titleX, 10); setColor(WHITE); std::cout << title;

        gotoXY(choiceX, 12);
        setColor(currentSelection == 1 ? YELLOW : GRAY);
        std::cout << (currentSelection == 1 ? ">> " : "   ") << choice1 << (currentSelection == 1 ? " <<" : "   ");

        gotoXY(choiceX, 14);
        setColor(currentSelection == 2 ? YELLOW : GRAY);
        std::cout << (currentSelection == 2 ? ">> " : "   ") << choice2 << (currentSelection == 2 ? " <<" : "   ");

        setColor(GRAY);

        inputKey = _getch();
        if (inputKey == 0xE0 || inputKey == 0) {
            inputKey = _getch();
            if (inputKey == 72) {
                currentSelection = (currentSelection == 1) ? numChoices : currentSelection - 1;
            }
            else if (inputKey == 80) {
                currentSelection = (currentSelection == numChoices) ? 1 : currentSelection + 1;
            }
        }
        else if (inputKey == 13) {
            selection = currentSelection;
            break;
        }
    }
    setColor(BLACK);
}

void Renderer::drawPlayerMessage(int playerIndex, const std::string& message) {
    int boardCenterX = getPlayerBoardOffsetX(playerIndex) + Board::COLS;
    int msgX = boardCenterX - message.length() / 2;
    int msgY = getPlayerBoardOffsetY(playerIndex) + 2;

    gotoXY(msgX, msgY);
    setColor(YELLOW);
    std::cout << message;
    setColor(BLACK);
}

void Renderer::drawOptionSelectionScreen(int& selection) {
    hideCursor();
    do {
        system("cls");
        int currentSelection = 1;
        const int numChoices = 3;
        int inputKey;

        std::string title = "SELECT OPTION";
        std::string choice1 = "STORY";
        std::string choice2 = "HOW TO PLAY";
        std::string choice3 = "SELECT GAME MODE";

        int titleX = (80 - title.length()) / 2 - 1;
        int choiceX1 = (80 - choice1.length()) / 2 - 4;
        int choiceX2 = (80 - choice2.length()) / 2 - 4;
        int choiceX3 = (80 - choice3.length()) / 2 - 4;

        while (true) {
            gotoXY(titleX, 9); setColor(WHITE); std::cout << title;

            gotoXY(choiceX1, 12);
            setColor(currentSelection == 1 ? YELLOW : GRAY);
            std::cout << (currentSelection == 1 ? ">> " : "   ") << choice1 << (currentSelection == 1 ? " <<" : "   ");

            gotoXY(choiceX2, 14);
            setColor(currentSelection == 2 ? YELLOW : GRAY);
            std::cout << (currentSelection == 2 ? ">> " : "   ") << choice2 << (currentSelection == 2 ? " <<" : "   ");

            gotoXY(choiceX3, 16);
            setColor(currentSelection == 3 ? YELLOW : GRAY);
            std::cout << (currentSelection == 3 ? ">> " : "   ") << choice3 << (currentSelection == 3 ? " <<" : "   ");

            setColor(GRAY);

            inputKey = _getch();
            if (inputKey == 0xE0 || inputKey == 0) {
                inputKey = _getch();
                if (inputKey == 72) {
                    currentSelection = (currentSelection == 1) ? numChoices : currentSelection - 1;
                }
                else if (inputKey == 80) {
                    currentSelection = (currentSelection == numChoices) ? 1 : currentSelection + 1;
                }
            }
            else if (inputKey == 13) {
                selection = currentSelection;
                break;
            }
        }
        switch (selection)
        {
        case 1: drawStory(); break;
        case 2: drawHowToPlay(); break;
        case 3: break;
        default: break;
        }
    } while (selection != 3);
    setColor(BLACK);
}

void Renderer::drawStory() {
    system("cls");
    setColor(WHITE);
    std::string temp = "이곳에 스토리 추가 예정";

    int tempX = (80 - temp.length()) / 2;
    gotoXY(tempX, 10); std::cout << temp;
    gotoXY(28, 20); std::cout << "Press Any Key to Exit!";
    _getch();
}

void Renderer::drawHowToPlay() {
    system("cls");
    setColor(WHITE);
    gotoXY(10, 3); std::cout << "            Player 1                             Player 2";
    gotoXY(10, 6); std::cout << "        ← → : 블럭 좌우 이동                 a d : 블럭 좌우 이동";
    gotoXY(10, 8); std::cout << "          ↑ : 블럭 회전                        w : 블럭 회전";
    gotoXY(10, 10); std::cout << "          ↓ : 블럭 내리기                      s : 블럭 내리기";
    gotoXY(10, 12); std::cout << "스페이스 바 : 블럭 바로 내리기            TAB 키 : 블럭 바로 내리기";

    gotoXY(5, 18); std::cout << "                                ※  주의 ※";
    gotoXY(5, 20); std::cout << "회색 상자는 취급 주의 물품이 들어가 있어, 조심히 다뤄야 하기에 회전이 불가능합니다.";
    gotoXY(5, 22); std::cout << "      무거운 상자들은 더 빨리 떨어집니다. 크기가 큰 상자들을 조심하세요!";
    
    gotoXY(32, 27); std::cout << "Press Any Key to Exit!";
    _getch();
}