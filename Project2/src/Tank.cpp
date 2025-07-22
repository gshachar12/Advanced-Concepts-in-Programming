#include "Tank.h"
#include <sstream>

// Constants for tank properties
const int DEFAULT_SHELL_COUNT = 20;  // This is now overridden by the board file
const int SHOOT_COOLDOWN = 2;       // Cooldown steps after shooting

void Tank::update() {
    if (!isAlive()) return;
    
    // Update cooldown counter if needed
    if (shootCooldown > 0) {
        shootCooldown--;
    }
    
    // Handle backward state machine
    if (backwardState == BackwardState::WAITING_1) {
        backwardState = BackwardState::WAITING_2;
    } else if (backwardState == BackwardState::WAITING_2) {
        backwardState = BackwardState::MOVING_BACKWARD;
    }
}

void Tank::shoot() {
    if (!canShoot()) return;
    
    // Decrement shell count and set cooldown
    shellCount--;
    shootCooldown = SHOOT_COOLDOWN;
}

std::pair<int, int> Tank::moveForward(Board board) {
    // Cancel any backward request
    cancelBackward();
    
    // Get direction offset
    auto [dx, dy] = Directions::directionToOffset(direction);
    return tryToMove(board, dx, dy);
}

std::pair<int, int> Tank::moveBackward(Board board) {
    if (backwardState != BackwardState::MOVING_BACKWARD) {
        // Can't move backward yet
        return {getX(), getY()};
    }
    
    // Reset state machine
    cancelBackward();
    
    // Move in opposite direction
    auto [dx, dy] = Directions::directionToOffset(direction);
    return tryToMove(board, -dx, -dy);
}

void Tank::requestBackward() {
    if (backwardState == BackwardState::NOT_REQUESTED) {
        backwardState = BackwardState::WAITING_1;
    }
}

void Tank::cancelBackward() {
    backwardState = BackwardState::NOT_REQUESTED;
}

void Tank::printPosition() {
    std::cout << "Tank " << tankID << " is at (" << getX() << ", " << getY() 
              << "), facing " << Directions::directionToString(direction) << std::endl;
}

void Tank::printStatus() const {
    std::cout << "Tank " << tankID << " Status:" << std::endl;
    std::cout << "  Alive: " << (alive ? "Yes" : "No") << std::endl;
    std::cout << "  Position: (" << getPosition().x << ", " << getPosition().y << ")" << std::endl;
    std::cout << "  Direction: " << Directions::directionToString(direction) << std::endl;
    std::cout << "  Shell Count: " << shellCount << std::endl;
    std::cout << "  Shoot Cooldown: " << shootCooldown << std::endl;
}

// New method for formatting the action string for the output file
std::string Tank::getActionString() const {
    if (!isAlive() && !killedThisRound) {
        return "killed";
    }
    
    std::stringstream ss;
    ss << lastAction;
    
    if (actionWasIgnored) {
        ss << " (ignored)";
    }
    
    if (killedThisRound) {
        ss << " (killed)";
    }
    
    return ss.str();
}

// Rotation methods
void Tank::rotateLeft45() {
    int idx = findDirectionIndex(direction);
    idx = (idx - 1 + 8) % 8;  // Subtract 1 and ensure it wraps around (8 directions)
    setDirectionByIndex(idx);
}

void Tank::rotateRight45() {
    int idx = findDirectionIndex(direction);
    idx = (idx + 1) % 8;  // Add 1 and wrap around if needed
    setDirectionByIndex(idx);
}

void Tank::rotateLeft90() {
    int idx = findDirectionIndex(direction);
    idx = (idx - 2 + 8) % 8;  // Subtract 2 and ensure it wraps around
    setDirectionByIndex(idx);
}

void Tank::rotateRight90() {
    int idx = findDirectionIndex(direction);
    idx = (idx + 2) % 8;  // Add 2 and wrap around if needed
    setDirectionByIndex(idx);
}

int Tank::findDirectionIndex(const Direction &d) const {
    // Map from Direction enum to array index
    // Using 8 directions clockwise
    static const Direction directions[8] = {
        Direction::UP,
        Direction::UP_RIGHT,
        Direction::RIGHT,
        Direction::DOWN_RIGHT,
        Direction::DOWN,
        Direction::DOWN_LEFT,
        Direction::LEFT,
        Direction::UP_LEFT
    };
    
    for (int i = 0; i < 8; ++i) {
        if (directions[i] == d) return i;
    }
    
    // Default to UP if not found (should not happen)
    return 0;
}

void Tank::setDirectionByIndex(int idx) {
    static const Direction directions[8] = {
        Direction::UP,
        Direction::UP_RIGHT,
        Direction::RIGHT,
        Direction::DOWN_RIGHT,
        Direction::DOWN,
        Direction::DOWN_LEFT,
        Direction::LEFT,
        Direction::UP_LEFT
    };
    
    if (idx >= 0 && idx < 8) {
        direction = directions[idx];
    }
}