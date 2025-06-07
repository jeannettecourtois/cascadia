class Position {
public:
    int x;
    int y;
    Position(int a = 0, const int b = 0) : x(a), y(b) {}
    Position(const Position& pos) : x(pos.x), y(pos.y) {}
    ~Position() = default;

    void SetPosition(int a, int b) { x = a; y = b; }
    void SetPosition(const Position& pos) { x = pos.x; y = pos.y; }

    Position getE() const { return Position(x + 1, y); }
    Position getSE() const { return Position(x, y + 1); }
    Position getSW() const { return Position(x - 1, y + 1); }
    Position getW() const { return Position(x - 1, y); }
    Position getNW() const { return Position(x, y - 1); }
    Position getNE() const { return Position(x + 1, y - 1); }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Position& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
}; #pragma once
