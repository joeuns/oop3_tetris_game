#include "Renderer.h"
#include "Color.h"    // ���� enum
#include <Windows.h>  // Windows API (�ܼ� ����)
#include <iostream>   // std::cout
#include <vector>     // �ΰ� ��� ���� �� ����
#include <string>     // drawStats���� ����->���ڿ� ��ȯ �� ���� ��꿡 ���
#include <conio.h>
#include <time.h>

// Block.h�� Renderer.h�� �̹� ���ԵǾ� �����Ƿ� �ߺ� ���� ���ʿ�
// #include "Block.h"

Renderer::Renderer(int offsetX, int offsetY) : offsetX_(offsetX), offsetY_(offsetY), statsLabelsPrinted_(false) {}

void Renderer::initConsole() {
    system("mode con: cols=80 lines=25"); // �ܼ� ũ�� ���� (����)
    system("cls"); // ȭ�� �����
    hideCursor();
}

void Renderer::hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Ŀ�� ����
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
    // static HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE); // ��� ������ �ϰų� �Ź� ȣ��
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Renderer::drawBoard(const Board& board) {
    const auto& grid = board.getGrid();
    for (int i = 0; i < Board::ROWS; ++i) {
        for (int j = 0; j < Board::COLS; ++j) {
            gotoXY(j * 2 + offsetX_, i + offsetY_); // ���� 2ĭ�� ���
            char cell = grid[i][j];
            if (cell == Board::WALL_CELL) { // �� �Ǵ� ���� ���
                // �� ���� (��: ȸ�� �Ǵ� ��ä�Ӱ�)
                setColor((j == 0 || j == Board::COLS - 1 || i == Board::ROWS - 1) ? GRAY : DARK_GRAY); // �׵θ� �ٸ���
                // setColor(GRAY); // �ܼ��� �� ����
                std::cout << "��"; // ä���� ĭ (���� �׸� ����)
            }
            else { // �� ���� (Board::EMPTY_CELL)
                setColor(BLACK); // ���� (�Ǵ� DARK_GRAY ��)
                std::cout << "  "; // �� ĭ (���� 2��)
            }
        }
    }
    setColor(BLACK); // �⺻ �������� ����
    gotoXY(0, Board::ROWS + offsetY_ + 1); // Ŀ�� ���� ���� �ٱ����� �̵�
}


void Renderer::drawBlock(const Block& block, int customOffsetX, int customOffsetY) {
    int shape = block.getShape();
    int angle = block.getAngle();
    // ����� �»�� ���� ��ǥ (Board ��ǥ��)
    int blockBoardX = block.getX();
    int blockBoardY = block.getY();

    // ���� ȭ�鿡 �׸� �� ����� ������ ����
    // customOffsetX/Y�� -1 (�⺻��)�̸� ��� ���� offsetX_/offsetY_ ���
    int renderPosX = (customOffsetX == -1) ? blockBoardX * 2 + offsetX_ : customOffsetX;
    int renderPosY = (customOffsetY == -1) ? blockBoardY + offsetY_ : customOffsetY;


    // ��� ������ ���� ���� ����
    int blockColor = WHITE; // �⺻��
    switch (shape) {
    case 0: blockColor = RED; break;        // I
    case 1: blockColor = YELLOW; break;     // O
    case 2: blockColor = VIOLET; break;    // T  (���� ����� �迭)
    case 3: blockColor = BLUE; break;       // L
    case 4: blockColor = DARK_YELLOW; break;       // J (���� ��Ȳ�� �迭, DARK_YELLOW�� ��ü)
    case 5: blockColor = GREEN; break;      // S
    case 6: blockColor = SKY_BLUE; break;   // Z  (���� �ϴû�/û�ϻ� �迭)
    default: blockColor = GRAY; break;
    }
    setColor(blockColor);

    for (int r = 0; r < 4; ++r) { // ��� 4x4 ���� ��
        for (int c = 0; c < 4; ++c) { // ��� 4x4 ���� ��
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                // ����� (0,0)�� renderPosX, renderPosY�� �׷������� ���
                // ��� ���� �� (c,r)�� ȭ��� ��ġ:
                // X: renderPosX + c * 2 (���� 2ĭ)
                // Y: renderPosY + r
                int screenX = renderPosX + c * 2;
                int screenY = renderPosY + r;

                // ����� ���� ���� ���(y < 0)�� ���� ���, ȭ�鿡 �׸��� ����
                // ��, customOffsetY�� ���� ���� (��: Next ���) y ��ǥ�� �״�� ���
                if (customOffsetY != -1 || (blockBoardY + r >= 0)) {
                    gotoXY(screenX, screenY);
                    std::cout << "��";
                }
            }
        }
    }
    setColor(BLACK); // �⺻ ���� ����
    // gotoXY(0, Board::ROWS + offsetY_ + 2); // Ŀ�� ���� ���� �ٱ����� �̵�
}

void Renderer::eraseBlock(const Block& block, int customOffsetX, int customOffsetY) {
    int shape = block.getShape();
    int angle = block.getAngle();
    int blockBoardX = block.getX();
    int blockBoardY = block.getY();

    int renderPosX = (customOffsetX == -1) ? blockBoardX * 2 + offsetX_ : customOffsetX;
    int renderPosY = (customOffsetY == -1) ? blockBoardY + offsetY_ : customOffsetY;

    setColor(BLACK); // �������� ����

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                int screenX = renderPosX + c * 2;
                int screenY = renderPosY + r;
                if (customOffsetY != -1 || (blockBoardY + r >= 0)) {
                    gotoXY(screenX, screenY);
                    std::cout << "  "; // ���� 2ĭ���� ����
                }
            }
        }
    }
    // gotoXY(0, Board::ROWS + offsetY_ + 2);
}

void Renderer::clearArea(int x, int y, int widthChars, int heightChars) {
    setColor(BLACK); // �������� ����
    for (int r = 0; r < heightChars; ++r) {
        gotoXY(x, y + r);
        for (int c = 0; c < widthChars; ++c) {
            std::cout << " "; // �������� ä��
        }
    }
}

void Renderer::drawNextBlockArea(const Block& nextBlock) {
    // 1. Next ��� ���� �׵θ� �׸��� �� ���� Ŭ����
    // �׵θ� ���� ���� (��: GRAY)
    setColor(GRAY);
    int boxScreenX = NEXT_BLOCK_AREA_X; // Renderer ��� �������� ���⼭ ��� �� �� (���� ��ǥ ����)
    int boxScreenY = NEXT_BLOCK_AREA_Y;

    for (int i = 0; i < NEXT_BLOCK_BOX_HEIGHT; ++i) {
        gotoXY(boxScreenX, boxScreenY + i);
        for (int j = 0; j < NEXT_BLOCK_BOX_WIDTH; ++j) {
            if (i == 0 || i == NEXT_BLOCK_BOX_HEIGHT - 1 || j == 0 || j == NEXT_BLOCK_BOX_WIDTH - 1) {
                std::cout << "�� "; // �׵θ� ���� (�Ǵ� "�� ")
            }
            else {
                std::cout << "  "; // ���� ���� (���� 2ĭ�� ����)
            }
        }
    }
    // "NEXT" �ؽ�Ʈ �߰� (���� ����)
    gotoXY(boxScreenX + (NEXT_BLOCK_BOX_WIDTH * 2 - 4) / 2 - 1, boxScreenY - 1); // �׵θ� ���� �߾� ���� �õ�
    std::cout << "NEXT";


    // 2. Next ��� �׸���
    // Next ����� 4x4 ���� ���� �׷�����, �� ������ �»�� ��ġ�� ����մϴ�.
    // �׵θ� ���� �߾ӿ� ����� ��ġ��Ű�� ���� ������ ���
    // (NEXT_BLOCK_BOX_WIDTH * 2�� ���ڴ��� �ʺ�, 4*2�� ��� �ִ� ���ڴ��� �ʺ�)
    int blockRenderScreenX = boxScreenX + (NEXT_BLOCK_BOX_WIDTH * 2 - 4 * 2) / 2 + 2; // +2�� �׵θ� �β����
    // (NEXT_BLOCK_BOX_HEIGHT�� ���ڴ��� ����, 4�� ��� �ִ� ���ڴ��� ����)
    int blockRenderScreenY = boxScreenY + (NEXT_BLOCK_BOX_HEIGHT - 4) / 2;

    // Block ��ü�� ���� �׸��� drawBlock �Լ� ���, custom ������ ����
    // nextBlock ��ü�� x, y�� ���Ӻ��� �����̹Ƿ� ������� �ʰ�, ���� ȭ�� ��ǥ�� ����
    drawBlock(nextBlock, blockRenderScreenX - 2, blockRenderScreenY + 1);
}


void Renderer::drawStats(int level, int score, int lines) {
    setColor(GRAY);

    // ��� ������ �⺻ X ��ǥ (������ ������)
    // �� ���� Renderer �������� offsetX_ �ʹ� ������, ȭ�� ��ü ���̾ƿ����� ��ġ.
    // ���� STATS_AREA_X�� ���� ���� ��� ��ġ��� offsetX_ + STATS_AREA_X �� ���.
    // ����� STATS_AREA_X�� ���� ȭ�� ��ǥ�� ���.
    // ���� offsetX_�� Ȱ���Ϸ���:
    // int baseStatsScreenX = offsetX_ + Board::COLS * 2 + 3; // ���� �ٷ� ��
    int baseStatsScreenX = STATS_AREA_X; // Renderer.h�� ���ǵ� ���밪 ���

    gotoXY(baseStatsScreenX + STATS_LABEL_X_LEVEL, STATS_LABEL_Y_LEVEL);
    std::cout << "STAGE";

    gotoXY(baseStatsScreenX + STATS_LABEL_X_SCORE, STATS_LABEL_Y_SCORE);
    std::cout << "SCORE";

    gotoXY(baseStatsScreenX + STATS_LABEL_X_LINES, STATS_LABEL_Y_LINES);
    std::cout << "LINES";
    statsLabelsPrinted_ = true;

    std::string levelStr = std::to_string(level + 1);
    gotoXY(baseStatsScreenX + STATS_VALUE_X_LEVEL, STATS_VALUE_Y_LEVEL);
    std::cout << std::string(8, ' ');
    gotoXY(baseStatsScreenX + STATS_VALUE_X_LEVEL, STATS_VALUE_Y_LEVEL);
    std::cout << levelStr;

    std::string scoreStr = std::to_string(score);
    gotoXY(baseStatsScreenX + STATS_VALUE_X_SCORE, STATS_VALUE_Y_SCORE);
    std::cout << std::string(10, ' ');
    gotoXY(baseStatsScreenX + STATS_VALUE_X_SCORE, STATS_VALUE_Y_SCORE);
    std::cout << scoreStr;

    std::string linesStr = std::to_string(lines);
    gotoXY(baseStatsScreenX + STATS_VALUE_X_LINES, STATS_VALUE_Y_LINES);
    std::cout << std::string(8, ' ');
    gotoXY(baseStatsScreenX + STATS_VALUE_X_LINES, STATS_VALUE_Y_LINES);
    std::cout << linesStr;

    setColor(BLACK);
}


void Renderer::drawLogo() {
    
    setColor(WHITE);
    
    gotoXY(13, 3);  std::cout << "����������������������������������������������������������������������������������������������";
    gotoXY(13, 4);  std::cout << "���� �� ��  �ߡߡ�  �� �� ��   �ߡ�     ��   �� �� ��         ��";
    gotoXY(13, 5);  std::cout << "��  ��    ��      ��     �� ��    ��   ��             ��";
    gotoXY(13, 6);  std::cout << "��  ��    �ߡߡ�    ��     �ߡ�     ��     ��           ��";
    gotoXY(13, 7);  std::cout << "��  ��    ��      ��     �� ��    ��       ��         ��";
    gotoXY(13, 8);  std::cout << "��  ��    �ߡߡ�    ��     ��  ��   ��   �� �� ��         ��";
    gotoXY(13, 9);  std::cout << "����������������������������������������������������������������������������������������������";

    gotoXY(28, 20);
    std::cout << "Please Press Any Key~!";

    for (int i = 0; ; ++i) {
        if (i % 40 == 0) {
            for (int j = 0; j < 5; ++j) {
                gotoXY(10, 14 + j);
                std::cout << "                                                  ";
            }
            // 4���� �� ���� �� reset
            Block b1(rand() % 7);
            Block b2(rand() % 7);
            Block b3(rand() % 7);
            Block b4(rand() % 7);

            b1.reset(b1.getShape(), 6, 14);
            b2.reset(b2.getShape(), 12, 14);
            b3.reset(b3.getShape(), 18, 14);
            b4.reset(b4.getShape(), 24, 14);

            for (int i = 0; i < rand() % 4; i++) b1.rotate();
            for (int i = 0; i < rand() % 4; i++) b2.rotate();
            for (int i = 0; i < rand() % 4; i++) b3.rotate();
            for (int i = 0; i < rand() % 4; i++) b4.rotate();

            drawBlock(b1);
            drawBlock(b2);
            drawBlock(b3);
            drawBlock(b4);
        }
        if (_kbhit()) break;
        Sleep(30);
    }
    _getche();
    system("cls");
}


void Renderer::drawGameOver() {
    // ȭ�� �߾ӿ� ���� ���� �޽��� ǥ��
    int centerX = 40; // �뷫���� ȭ�� �߾� X (80�÷� ����)
    int centerY = 12; // �뷫���� ȭ�� �߾� Y (25���� ����)

    std::vector<std::string> gameOverMsg = {
        "��������������������������������������������������������",
        "��      G A M E  O V E R      ��",
        "��������������������������������������������������������"
    };
    std::string pressKeyMsg = "Press any key to exit...";

    setColor(RED);
    for (size_t i = 0; i < gameOverMsg.size(); ++i) {
        gotoXY(centerX - gameOverMsg[i].length() / 2, centerY + i - 1);
        std::cout << gameOverMsg[i];
    }

    setColor(YELLOW);
    gotoXY(centerX - pressKeyMsg.length() / 2, centerY + gameOverMsg.size() + 1);
    std::cout << pressKeyMsg;

    // ���� _getche(), system("cls")�� Game Ŭ������ main���� ó���ϵ��� ��
    // ���⼭�� �׸��⸸ ���
    while (!_kbhit()) { // Ű �Է� ���
        Sleep(100);
    }
    _getche(); // Ű �Է� �Ҹ�
    system("cls"); // ���� �� ȭ�� ����
}