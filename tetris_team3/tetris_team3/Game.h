#pragma once

#include <vector>
#include <cstdlib>  // rand, srand
#include <ctime>
#include "Board.h"
#include "Block.h"
#include "Renderer.h"

// �������� ���� ����ü
struct Stage {
    int speed; // �������� �ӵ� (�������� ����)
    int stickRate; // ���� �� ���� Ȯ�� (%)
    int clearLine; // �ش� �������� Ŭ���� ���� (�� ��)
};

// ��ü ������ �帧�� ���¸� �����ϴ� Ŭ����
class TetrisGame {
public:
    TetrisGame();
    void init();  // ���� ��ü �ʱ�ȭ (���� �õ�, ���� ����, �ܼ� �ʱ�ȭ ��)
    void run();

private:
    Board board_; // ������
    Renderer renderer_; // ȭ�� ���
    Block* currentBlock_; // ���� �� (���� �Ҵ�)
    Block* nextBlock_; // ���� �� (���� �Ҵ�)

    std::vector<Stage> stages_;  // �� �������� ����
    int level_;
    int score_;
    int lines_;

    void setupStages(); // �������� ����
    void inputData(); // ����� �Է� ó�� (���� ���� ��)
    void spawnBlock(); // �� �� ����
    void processInput(); // Ű �Է� ó��
    void update(); // ���� ���� (�浹, ����, ���� ��)
};