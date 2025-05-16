#pragma once

#include <array>

/* �������� ����(21x14 �׸���)�� �����ϰ� 
�� �浹, ����, ���� ���� ���� ������ ó���ϴ� Ŭ���� */

class Board {
public:
    static constexpr int ROWS = 21; // ���� �� ��
    static constexpr int COLS = 14; // ���� ĭ ��


    Board(); // �������� �ʱ�ȭ�Ѵ� (��/�ٴ� ���� ��)
    void init(); // �־��� �� ������ �浹 ���θ� �˻��Ѵ�
    bool strikeCheck(int shape, int angle, int x, int y) const; // ���� �����ǿ� �����Ѵ� (�浹 �� false ��ȯ)
    bool mergeBlock(int shape, int angle, int x, int y); // ���� �� ���� �����ϰ� �����ش�, ������ �� �� ��ȯ
    int clearFullLines(); // ���� �� ���� �����ϰ� �����ش�, ������ �� �� ��ȯ

    // �ܺο��� ������ �׸���(�б� ����)�� �����´�
    const std::array<std::array<char, COLS>, ROWS>& getGrid() const;

private:
    std::array<std::array<char, COLS>, ROWS> grid_; // ������ �׸��� ����
};