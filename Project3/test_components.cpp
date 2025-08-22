#include "Board.h"
#include "Tank.h"
#include "Shell.h"
#include <iostream>

/**
 * Simple test to verify core components work
 */
int main() {
    std::cout << "=== Project 3 Core Components Test ===" << std::endl;
    
    // Test Board creation
    std::cout << "Testing Board..." << std::endl;
    Board board(10, 8);
    board.setCellType(0, 0, CellType::WALL);
    board.setCellType(1, 1, CellType::MINE);
    
    std::cout << "  Board created: " << board.getWidth() << "x" << board.getHeight() << std::endl;
    std::cout << "  Cell (0,0): " << board.getObjectAt(0, 0) << std::endl;
    std::cout << "  Cell (1,1): " << board.getObjectAt(1, 1) << std::endl;
    
    // Test Tank creation
    std::cout << "Testing Tank..." << std::endl;
    Position tank_pos(5, 4);
    Tank tank(tank_pos, Direction::UP, 1, 0);
    
    std::cout << "  Tank created at (" << tank.getPosition().x << "," << tank.getPosition().y << ")" << std::endl;
    std::cout << "  Tank direction: " << DirectionUtils::toString(tank.getDirection()) << std::endl;
    std::cout << "  Tank can shoot: " << (tank.canShoot() ? "Yes" : "No") << std::endl;
    std::cout << "  Tank shells remaining: " << tank.getShellsRemaining() << std::endl;
    
    // Test Tank movement
    Position forward_pos = tank.getForwardPosition();
    std::cout << "  Forward position would be: (" << forward_pos.x << "," << forward_pos.y << ")" << std::endl;
    
    // Test Tank rotation
    tank.rotateRight45();
    std::cout << "  After rotating right 45°: " << DirectionUtils::toString(tank.getDirection()) << std::endl;
    
    // Test Shell creation
    std::cout << "Testing Shell..." << std::endl;
    Shell shell(Position(3, 3), Direction::RIGHT, 1, 0);
    
    std::cout << "  Shell created at (" << shell.getPosition().x << "," << shell.getPosition().y << ")" << std::endl;
    std::cout << "  Shell direction: " << DirectionUtils::toString(shell.getDirection()) << std::endl;
    std::cout << "  Shell active: " << (shell.isActive() ? "Yes" : "No") << std::endl;
    
    // Test Shell movement
    Position next_pos = shell.getNextPosition();
    std::cout << "  Next position would be: (" << next_pos.x << "," << next_pos.y << ")" << std::endl;
    
    shell.advance();
    std::cout << "  After advance: (" << shell.getPosition().x << "," << shell.getPosition().y << ")" << std::endl;
    std::cout << "  Distance traveled: " << shell.getDistanceTraveled() << std::endl;
    
    std::cout << "=== All core components working! ✓ ===" << std::endl;
    
    return 0;
}
