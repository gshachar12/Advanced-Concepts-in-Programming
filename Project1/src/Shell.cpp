#include "Shell.h"
#include <iostream>
#include <utility>
#include <cmath>

// Advance the shell (move it one step in the direction of travel)
std::pair<int, int> Shell::advance() {
    // Get movement offset based on the current direction
    auto [dx, dy] = Directions::directionToOffset(getDirection());
    
    // Update the total distance traveled
    distanceTraveled += 1;

    // If the shell has traveled its maximum range, deactivate it
    if (distanceTraveled >= maxRange)
        deactivate();

    return {dx, dy};
}

void Shell::printStatus()  {
    std::cout << "Shell at (" << getX() << ", " << getY() << ")"
              << ", direction=" << Directions::directionToString(getDirection()) 
              << ", active=" << (active ? "true" : "false")
              << ", damage=" << damage
              << ", traveled=" << distanceTraveled << "/" << maxRange
              << ", ownerID=" << ownerID
              << std::endl;
}
