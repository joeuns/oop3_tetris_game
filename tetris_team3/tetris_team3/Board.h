#pragma once
#include <array>
#include <vector>
enum class GameMode;

class Board {
public:
    static constexpr int ROWS = 21;
    static constexpr int COLS = 14;

    static constexpr char EMPTY_CELL = 0;
    static constexpr char WALL_CELL = 1;

    Board();
    void init();
    bool strikeCheck(int shape, int angle, int x, int y) const;
    bool mergeBlock(int shape, int angle, int x, int y);
    int clearFullLines(GameMode mode, int stage, int playerIndex);

    const std::array<std::array<char, COLS>, ROWS>& getGrid() const;

private:
    std::array<std::array<char, COLS>, ROWS> grid_;
};