#pragma once

#include "Board.h"
#include "Block.h"

/* �ֿܼ� ���(������, ��, ��� ��)�� ����ϴ� Ŭ����
   Ŀ�� �̵�, ���� ���� �� �ܼ� ���� �Լ��� ���� */
class Renderer {
public:
    // ������: �ܼ� ��� ������ ����
    Renderer(int offsetX, int offsetY);

    // �ܼ� Ŀ��/ȭ�� �ʱ�ȭ �Լ�
    void initConsole();
    void gotoXY(int x, int y);
    void setColor(int color);

    // �׸��� �Լ���
    void drawBoard(const Board& board);
    void drawBlock(const Block& block);
    void eraseBlock(const Block& block);
    void drawNext(const Block& nextBlock);
    void drawStats(int level, int score, int lines);
    void drawLogo();
    void drawGameOver();

private:
    int offsetX_, offsetY_;  // �ܼ� ��ġ ������ ������
};