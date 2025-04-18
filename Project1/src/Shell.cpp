#include "Shell.h"
#include <iostream>
#include <utility>

// Helper function: convert a Direction value to an (dx, dy) pair.
static std::pair<int,int> shellDirectionToOffset(Direction dir) {
    switch (dir) {
        case Direction::U:  return {0, -1};
        case Direction::UR: return {1, -1};
        case Direction::R:  return {1,  0};
        case Direction::DR: return {1,  1};
        case Direction::D:  return {0,  1};
        case Direction::DL: return {-1, 1};
        case Direction::L:  return {-1, 0};
        case Direction::UL: return {-1, -1};
        default:            return {0, 0};
    }
}

void Shell::advance() {
    if (!active) return;

    // Get movement offset based on the current direction.
    auto [dx, dy] = shellDirectionToOffset(getDirection());

    // Move 2 cells using the base class move(int, int).
    GameObject::move(2 * dx, 2 * dy);

    // Update the total distance traveled.
    distanceTraveled += 2;

    // If the shell has traveled its maximum range, deactivate it.
    if (distanceTraveled >= maxRange)
        deactivate();
}

void Shell::update() {
    // Simply advance the shell each update.
    advance();
}

void Shell::printStatus() const {
    std::cout << "Shell at (" << getX() << ", " << getY() << ")"
              << ", direction=" << static_cast<int>(getDirection())
              << ", active=" << (active ? "true" : "false")
              << ", damage=" << damage
              << ", traveled=" << distanceTraveled << "/" << maxRange
              << ", ownerID=" << ownerID
              << std::endl;
}
