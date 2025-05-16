#pragma once

#include "Board.h"
#include "Block.h"

/* 콘솔에 출력(게임판, 블럭, 통계 등)을 담당하는 클래스
   커서 이동, 색상 설정 등 콘솔 관련 함수도 포함 */
class Renderer {
public:
    // 생성자: 콘솔 출력 오프셋 설정
    Renderer(int offsetX, int offsetY);

    // 콘솔 커서/화면 초기화 함수
    void initConsole();
    void gotoXY(int x, int y);
    void setColor(int color);

    // 그리기 함수들
    void drawBoard(const Board& board);
    void drawBlock(const Block& block);
    void eraseBlock(const Block& block);
    void drawNext(const Block& nextBlock);
    void drawStats(int level, int score, int lines);
    void drawLogo();
    void drawGameOver();

private:
    int offsetX_, offsetY_;  // 콘솔 위치 조정용 오프셋
};