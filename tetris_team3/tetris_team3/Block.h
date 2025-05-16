#pragma once

#include "Board.h"
#include <array>

/* �ϳ��� ���� ǥ���ϰ� ȸ��, �̵� ���� ������ ����ϴ� Ŭ���� */
class Block {
public:
    Block(int shape); // �� ������: ��� ��ȣ�� �ʱ�ȭ
    void reset(int shape, int startX, int startY); // ���� ���� �����ϰ� ���� ��ġ�� �ʱ�ȭ
    bool move(int dx, int dy, const Board& board); // ���� (dx, dy)��ŭ �̵��õ�. �̵� �����ϸ� true
    void rotate(const Board& board); // ���� ȸ���õ�. �����ϸ� ����

    // �� ���� ��ȸ (getter��)
    int getShape() const;
    int getAngle() const;
    int getX() const;
    int getY() const;

private:
    int shape_; // �� ��� ���� (0~6)
    int angle_; // ȸ�� ���� (0~3)
    int x_, y_; // ���� ��ġ

    // 7�� ���� ȸ�� ���¸� ������ ���� ������
    static const std::array<std::array<std::array<char, 4>, 4>, 7> shapes_;
};
