#pragma once

#include "Board.h" // Board.h�� ���� �����ϴ� ���� ���� �� �ֽ��ϴ� (������ ����).
#include <array>

/* �ϳ��� ���� ǥ���ϰ� ȸ��, �̵� ���� ������ ����ϴ� Ŭ���� */
class Block {
public:
    Block(int shape); // �� ������: ��� ��ȣ�� �ʱ�ȭ
    void reset(int shape, int startX, int startY); // ���� ���� �����ϰ� ���� ��ġ�� �ʱ�ȭ
    bool move(int dx, int dy, const Board& board); // ���� (dx, dy)��ŭ �̵��õ�. �̵� �����ϸ� true
    void rotate(const Board& board); // ���� ȸ���õ�. �����ϸ� ����
    void rotate(); // �浹 �˻縦 ���� �ʰ�, ������ ȸ�� ����.

    // �� ���� ��ȸ (getter��)
    int getShape() const;
    int getAngle() const;
    int getX() const;
    int getY() const;

    // 4x4 �׸��� ���� ��� ��� ������ ����
    static char getBlockShapeData(int shape, int angle, int row, int col);

    // 7�� ���� ȸ�� ���¸� ������ ���� ������
    // ���� ���ӿ����� �� �����͸� �ùٸ��� �����ؾ� �մϴ�.
    // ���÷� Block.cpp�� ���ǵǾ� �ֽ��ϴ�.
    static const char shapes_[7][4][4][4]; // ���� �ϰ� ���Ǵ� cpp ���Ͽ���

private:
    int shape_; // �� ��� ���� (0~6)
    int angle_; // ȸ�� ���� (0~3)
    int x_, y_; // ���� ��ġ (��� 4x4 �迭�� �»�� ���� ���� ��ǥ)
};