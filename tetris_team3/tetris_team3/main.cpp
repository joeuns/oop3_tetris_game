#include "Game.h"

int main() {
    TetrisGame game;
    game.init();  // 스테이지/레벨 초기화, 콘솔 세팅 등
    game.run();   // 메인 게임 루프 시작
    return 0;
}