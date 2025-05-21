#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x;
    int y;
    
    Position() : x(0), y(0) {}
    Position(int _x, int _y) : x(_x), y(_y) {}
    
    bool operator==(const Position& other) const {
        return (x == other.x && y == other.y);
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
    
    // New method declarations to match Position.cpp
    int manhattanDistance(const Position& other) const;
    double euclideanDistance(const Position& other) const;
    bool isInRange(const Position& center, int range) const;
    Position offset(int dx, int dy) const;
    bool isWithinBounds(int maxX, int maxY) const;
    void clamp(int maxX, int maxY);
    void wrap(int maxX, int maxY);
};

#endif // POSITION_H