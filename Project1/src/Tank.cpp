#include "Tank.h"
#include <iostream>
#include <algorithm>

// We removed the duplicate definition of directionToString here
// because it is already defined inline in Tank.h.

const std::vector<std::string> Tank::validDirections = {"U", "UR", "R", "DR", "D", "DL", "L", "UL"};

// Helper: Convert a string direction (e.g. "U", "UR", etc.) to (dx, dy).
std::pair<int,int> Tank::directionToOffset(const std::string &dirStr) const {
    if (dirStr == "U")  return {0, -1};
    if (dirStr == "UR") return {1, -1};
    if (dirStr == "R")  return {1, 0};
    if (dirStr == "DR") return {1, 1};
    if (dirStr == "D")  return {0, 1};
    if (dirStr == "DL") return {-1, 1};
    if (dirStr == "L")  return {-1, 0};
    if (dirStr == "UL") return {-1, -1};
    return {0, 0};
}

void Tank::update() {
    if (shootCooldown > 0) {
        shootCooldown--;
    }
    switch (backwardState) {
        case BackwardState::WAITING_1:
            backwardState = BackwardState::WAITING_2;
            break;
        case BackwardState::WAITING_2:
            backwardState = BackwardState::MOVING_BACKWARD;
            break;
        case BackwardState::MOVING_BACKWARD:
            backwardState = BackwardState::NOT_REQUESTED;
            break;
        default:
            break;
    }
}

void Tank::shoot() {
    if (canShoot()) {
        shellCount = std::max(0, shellCount - 1);
        shootCooldown = 4; 
    } else {
        std::cerr << "Cannot shoot: either on cooldown or out of shells." << std::endl;
    }
}

std::pair<int,int>  Tank::moveForward() {
    std::string dirStr = directionToString(getDirection());
    auto [dx, dy] = directionToOffset(dirStr);
    // Use the inherited move(int, int) to adjust the position.
    return {dx, dy};
}

std::pair<int,int> Tank::moveBackward() {
    std::string dirStr = directionToString(getDirection());
    auto [dx, dy] = directionToOffset(dirStr);
    return {-dx,-dy};
}

void Tank::requestBackward() {
    if (backwardState == BackwardState::NOT_REQUESTED) {
        backwardState = BackwardState::WAITING_1;
    }
}

void Tank::cancelBackward() {
    if (backwardState == BackwardState::WAITING_1 ||
        backwardState == BackwardState::WAITING_2) {
        backwardState = BackwardState::NOT_REQUESTED;
    }
}

void Tank::printPosition() {
    std::cout << "Tank " << tankID<< " is at (" << getX() << ", " << getY() << ")." << std::endl;
}

void Tank::printStatus() const {
    std::cout << "Tank "<< tankID<< " at (" << getX() << ", " << getY() << "), direction="
              << directionToString(getDirection())
              << ", shells=" << shellCount
              << ", cooldown=" << shootCooldown
              << ", alive=" << (alive ? "true" : "false") << std::endl;
}

// Optional rotation helpers
void Tank::rotateLeft1_8() {
    int idx = findDirectionIndex(directionToString(getDirection()));
    if (idx >= 0) {
        setDirectionByIndex(idx - 1);
    }
}

void Tank::rotateRight1_8() {
    int idx = findDirectionIndex(directionToString(getDirection()));
    if (idx >= 0) {
        setDirectionByIndex(idx + 1);
    }
}

void Tank::rotateLeft1_4() {
    int idx = findDirectionIndex(directionToString(getDirection()));
    if (idx >= 0) {
        setDirectionByIndex(idx - 2);
    }
}

void Tank::rotateRight1_4() {
    int idx = findDirectionIndex(directionToString(getDirection()));
    if (idx >= 0) {
        setDirectionByIndex(idx + 2);
    }
}

int Tank::findDirectionIndex(const std::string &d) const {
    // Use size_t to avoid the integer-sign mismatch warning.
    for (size_t i = 0; i < validDirections.size(); i++) {
        if (validDirections[i] == d)
            return static_cast<int>(i);
    }
    return -1;
}

void Tank::setDirectionByIndex(int idx) {
    int n = validDirections.size();
    idx = (idx % n + n) % n;
    setDirection(GameObject::stringToDirection(validDirections[idx]));
}

 Tank:: ~Tank(){}