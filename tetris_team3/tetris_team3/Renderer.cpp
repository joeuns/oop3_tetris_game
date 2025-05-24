#include "Renderer.h"
#include "Color.h"    // 색상 enum
#include <Windows.h>  // Windows API (콘솔 제어)
#include <iostream>   // std::cout
#include <vector>     // 로고 등에서 사용될 수 있음
#include <string>     // drawStats에서 숫자->문자열 변환 및 길이 계산에 사용
#include <conio.h>
#include <time.h>

// Block.h는 Renderer.h에 이미 포함되어 있으므로 중복 포함 불필요
// #include "Block.h"

Renderer::Renderer(int offsetX, int offsetY) : offsetX_(offsetX), offsetY_(offsetY), statsLabelsPrinted_(false) {}

void Renderer::initConsole() {
    system("mode con: cols=80 lines=25"); // 콘솔 크기 설정 (예시)
    system("cls"); // 화면 지우기
    hideCursor();
}

void Renderer::hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // 커서 숨김
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
    // static HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE); // 멤버 변수로 하거나 매번 호출
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Renderer::drawBoard(const Board& board) {
    const auto& grid = board.getGrid();
    for (int i = 0; i < Board::ROWS; ++i) {
        for (int j = 0; j < Board::COLS; ++j) {
            gotoXY(j * 2 + offsetX_, i + offsetY_); // 가로 2칸씩 사용
            char cell = grid[i][j];
            if (cell == Board::WALL_CELL) { // 벽 또는 쌓인 블록
                // 벽 색상 (예: 회색 또는 다채롭게)
                setColor((j == 0 || j == Board::COLS - 1 || i == Board::ROWS - 1) ? GRAY : DARK_GRAY); // 테두리 다른색
                // setColor(GRAY); // 단순한 벽 색상
                std::cout << "■"; // 채워진 칸 (꽉찬 네모 문자)
            }
            else { // 빈 공간 (Board::EMPTY_CELL)
                setColor(BLACK); // 배경색 (또는 DARK_GRAY 등)
                std::cout << "  "; // 빈 칸 (공백 2개)
            }
        }
    }
    setColor(BLACK); // 기본 색상으로 복원
    gotoXY(0, Board::ROWS + offsetY_ + 1); // 커서 게임 영역 바깥으로 이동
}


void Renderer::drawBlock(const Block& block, int customOffsetX, int customOffsetY) {
    int shape = block.getShape();
    int angle = block.getAngle();
    // 블록의 좌상단 기준 좌표 (Board 좌표계)
    int blockBoardX = block.getX();
    int blockBoardY = block.getY();

    // 실제 화면에 그릴 때 사용할 오프셋 결정
    // customOffsetX/Y가 -1 (기본값)이면 멤버 변수 offsetX_/offsetY_ 사용
    int renderPosX = (customOffsetX == -1) ? blockBoardX * 2 + offsetX_ : customOffsetX;
    int renderPosY = (customOffsetY == -1) ? blockBoardY + offsetY_ : customOffsetY;


    // 블록 종류에 따른 색상 설정
    int blockColor = WHITE; // 기본색
    switch (shape) {
    case 0: blockColor = RED; break;        // I
    case 1: blockColor = YELLOW; break;     // O
    case 2: blockColor = VIOLET; break;    // T  (원래 보라색 계열)
    case 3: blockColor = BLUE; break;       // L
    case 4: blockColor = DARK_YELLOW; break;       // J (원래 주황색 계열, DARK_YELLOW로 대체)
    case 5: blockColor = GREEN; break;      // S
    case 6: blockColor = SKY_BLUE; break;   // Z  (원래 하늘색/청록색 계열)
    default: blockColor = GRAY; break;
    }
    setColor(blockColor);

    for (int r = 0; r < 4; ++r) { // 블록 4x4 내부 행
        for (int c = 0; c < 4; ++c) { // 블록 4x4 내부 열
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                // 블록의 (0,0)이 renderPosX, renderPosY에 그려지도록 계산
                // 블록 내부 셀 (c,r)의 화면상 위치:
                // X: renderPosX + c * 2 (가로 2칸)
                // Y: renderPosY + r
                int screenX = renderPosX + c * 2;
                int screenY = renderPosY + r;

                // 블록이 게임 보드 상단(y < 0)에 있을 경우, 화면에 그리지 않음
                // 단, customOffsetY가 사용된 경우는 (예: Next 블록) y 좌표를 그대로 사용
                if (customOffsetY != -1 || (blockBoardY + r >= 0)) {
                    gotoXY(screenX, screenY);
                    std::cout << "■";
                }
            }
        }
    }
    setColor(BLACK); // 기본 색상 복원
    // gotoXY(0, Board::ROWS + offsetY_ + 2); // 커서 게임 영역 바깥으로 이동
}

void Renderer::eraseBlock(const Block& block, int customOffsetX, int customOffsetY) {
    int shape = block.getShape();
    int angle = block.getAngle();
    int blockBoardX = block.getX();
    int blockBoardY = block.getY();

    int renderPosX = (customOffsetX == -1) ? blockBoardX * 2 + offsetX_ : customOffsetX;
    int renderPosY = (customOffsetY == -1) ? blockBoardY + offsetY_ : customOffsetY;

    setColor(BLACK); // 배경색으로 지움

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (Block::getBlockShapeData(shape, angle, r, c) == 1) {
                int screenX = renderPosX + c * 2;
                int screenY = renderPosY + r;
                if (customOffsetY != -1 || (blockBoardY + r >= 0)) {
                    gotoXY(screenX, screenY);
                    std::cout << "  "; // 공백 2칸으로 지움
                }
            }
        }
    }
    // gotoXY(0, Board::ROWS + offsetY_ + 2);
}

void Renderer::clearArea(int x, int y, int widthChars, int heightChars) {
    setColor(BLACK); // 배경색으로 설정
    for (int r = 0; r < heightChars; ++r) {
        gotoXY(x, y + r);
        for (int c = 0; c < widthChars; ++c) {
            std::cout << " "; // 공백으로 채움
        }
    }
}

void Renderer::drawNextBlockArea(const Block& nextBlock) {
    // 1. Next 블록 영역 테두리 그리기 및 내부 클리어
    // 테두리 색상 설정 (예: GRAY)
    setColor(GRAY);
    int boxScreenX = NEXT_BLOCK_AREA_X; // Renderer 멤버 오프셋은 여기서 사용 안 함 (절대 좌표 기준)
    int boxScreenY = NEXT_BLOCK_AREA_Y;

    for (int i = 0; i < NEXT_BLOCK_BOX_HEIGHT; ++i) {
        gotoXY(boxScreenX, boxScreenY + i);
        for (int j = 0; j < NEXT_BLOCK_BOX_WIDTH; ++j) {
            if (i == 0 || i == NEXT_BLOCK_BOX_HEIGHT - 1 || j == 0 || j == NEXT_BLOCK_BOX_WIDTH - 1) {
                std::cout << "□ "; // 테두리 문자 (또는 "■ ")
            }
            else {
                std::cout << "  "; // 내부 공백 (가로 2칸씩 가정)
            }
        }
    }
    // "NEXT" 텍스트 추가 (선택 사항)
    gotoXY(boxScreenX + (NEXT_BLOCK_BOX_WIDTH * 2 - 4) / 2 - 1, boxScreenY - 1); // 테두리 위에 중앙 정렬 시도
    std::cout << "NEXT";


    // 2. Next 블록 그리기
    // Next 블록은 4x4 영역 내에 그려지며, 이 영역의 좌상단 위치를 계산합니다.
    // 테두리 내부 중앙에 블록을 위치시키기 위한 오프셋 계산
    // (NEXT_BLOCK_BOX_WIDTH * 2는 문자단위 너비, 4*2는 블록 최대 문자단위 너비)
    int blockRenderScreenX = boxScreenX + (NEXT_BLOCK_BOX_WIDTH * 2 - 4 * 2) / 2 + 2; // +2는 테두리 두께고려
    // (NEXT_BLOCK_BOX_HEIGHT는 문자단위 높이, 4는 블록 최대 문자단위 높이)
    int blockRenderScreenY = boxScreenY + (NEXT_BLOCK_BOX_HEIGHT - 4) / 2;

    // Block 객체를 직접 그리는 drawBlock 함수 사용, custom 오프셋 전달
    // nextBlock 자체의 x, y는 게임보드 기준이므로 사용하지 않고, 계산된 화면 좌표를 전달
    drawBlock(nextBlock, blockRenderScreenX - 2, blockRenderScreenY + 1);
}


void Renderer::drawStats(int level, int score, int lines) {
    setColor(GRAY);

    // 통계 영역의 기본 X 좌표 (보드의 오른쪽)
    // 이 값은 Renderer 생성자의 offsetX_ 와는 별개로, 화면 전체 레이아웃상의 위치.
    // 만약 STATS_AREA_X가 보드 기준 상대 위치라면 offsetX_ + STATS_AREA_X 로 계산.
    // 현재는 STATS_AREA_X를 절대 화면 좌표로 사용.
    // 만약 offsetX_를 활용하려면:
    // int baseStatsScreenX = offsetX_ + Board::COLS * 2 + 3; // 보드 바로 옆
    int baseStatsScreenX = STATS_AREA_X; // Renderer.h에 정의된 절대값 사용

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
    
    gotoXY(13, 3);  std::cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    gotoXY(13, 4);  std::cout << "┃◆ ◆ ◆  ◆◆◆  ◆ ◆ ◆   ◆◆     ◆   ◆ ◆ ◆         ┃";
    gotoXY(13, 5);  std::cout << "┃  ◆    ◆      ◆     ◆ ◆    ◆   ◆             ┃";
    gotoXY(13, 6);  std::cout << "┃  ◆    ◆◆◆    ◆     ◆◆     ◆     ◆           ┃";
    gotoXY(13, 7);  std::cout << "┃  ◆    ◆      ◆     ◆ ◆    ◆       ◆         ┃";
    gotoXY(13, 8);  std::cout << "┃  ◆    ◆◆◆    ◆     ◆  ◆   ◆   ◆ ◆ ◆         ┃";
    gotoXY(13, 9);  std::cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    gotoXY(28, 20);
    std::cout << "Please Press Any Key~!";

    for (int i = 0; ; ++i) {
        if (i % 40 == 0) {
            for (int j = 0; j < 5; ++j) {
                gotoXY(10, 14 + j);
                std::cout << "                                                  ";
            }
            // 4개의 블럭 생성 후 reset
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
    // 화면 중앙에 게임 오버 메시지 표시
    int centerX = 40; // 대략적인 화면 중앙 X (80컬럼 기준)
    int centerY = 12; // 대략적인 화면 중앙 Y (25라인 기준)

    std::vector<std::string> gameOverMsg = {
        "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓",
        "┃      G A M E  O V E R      ┃",
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
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

    // 기존 _getche(), system("cls")는 Game 클래스나 main에서 처리하도록 함
    // 여기서는 그리기만 담당
    while (!_kbhit()) { // 키 입력 대기
        Sleep(100);
    }
    _getche(); // 키 입력 소모
    system("cls"); // 종료 전 화면 정리
}