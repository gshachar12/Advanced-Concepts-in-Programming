#include "Shell.h"
#include "Globals.h"
#include <iostream>
#include <utility>
#include <cmath>

// Checks if the shell is targeting the given obj.
bool Shell::isTargeting(const GameObject& obj) const{

    int shellX = getX(); 
    int shellY = getY(); 

    // Get the obj position
    int objX = obj.getX();
    int objY = obj.getY();

    // Get direction offset based on the shell's direction
    auto [dx, dy] = Directions::directionToOffset(direction);

    // Check if the obj is in the same line of movement (same x or y axis)
    if (dx != 0) {
        // If moving horizontally
        if (shellY == objY) {   // <<== FIX: use shellY
            if ((dx > 0 && shellX < objX) || (dx < 0 && shellX > objX)) {
                return true;  // Shell is heading toward obj horizontally
            }
        }
    }

    if (dy != 0) {
        // If moving vertically
        if (shellX == objX) {
            if ((dy > 0 && shellY < objY) || (dy < 0 && shellY > objY)) {
                return true;  // Shell is heading toward obj vertically
            }
        }
    }

    // Check diagonal movements
    if (dx != 0 && dy != 0) {
        if (std::abs(shellX - objX) == std::abs(shellY - objY)) {
            if ((dx > 0 && dy > 0 && shellX < objX && shellY < objY) ||
                (dx < 0 && dy < 0 && shellX > objX && shellY > objY) ||
                (dx > 0 && dy < 0 && shellX < objX && shellY > objY) ||
                (dx < 0 && dy > 0 && shellX > objX && shellY < objY)) {
                return true;  // Shell is heading toward the obj diagonally
            }
        }
    }

    return false;  // Not targeting
}


// Advance the shell (move it one step in the direction of travel)
std::pair<int, int> Shell::advance() {
    // Get movement offset based on the current direction.
    auto [dx, dy] = Directions::directionToOffset(getDirection());
    // Move 1 cell using the base class move(int, int).
    // Update the total distance traveled.
    distanceTraveled += 1;

    // If the shell has traveled its maximum range, deactivate it.
    if (distanceTraveled >= maxRange)
        deactivate();

    return {dx, dy};
}

void Shell::printStatus() const {
    std::cout << "Shell at (" << getX() << ", " << getY() << ")"
              << ", direction=" << static_cast<int>(getDirection())  //fix later
              << ", active=" << (active ? "true" : "false")
              << ", damage=" << damage
              << ", traveled=" << distanceTraveled << "/" << maxRange
              << ", ownerID=" << ownerID
              << std::endl;
}
