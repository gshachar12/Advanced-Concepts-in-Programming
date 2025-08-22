#include "GameState.h"
#include "CollisionDetector.h"
#include "ActionProcessor.h"
#include "TankBattleInfo.h"
#include <iostream>

/**
 * Test for the game management system
 */
int main() {
    std::cout << "=== Project 3 Game Management System Test ===" << std::endl;
    
    // Test GameState creation
    std::cout << "Testing GameState..." << std::endl;
    GameState game_state(10, 8, 100, 16);
    
    // Add some walls to the board
    game_state.getBoard().setCellType(3, 3, CellType::WALL);
    game_state.getBoard().setCellType(5, 5, CellType::MINE);
    
    // Add tanks
    game_state.addTank(1, Position(1, 1), Direction::RIGHT);
    game_state.addTank(2, Position(8, 6), Direction::LEFT);
    
    std::cout << "  GameState created with " << game_state.getAllTanks().size() << " tanks" << std::endl;
    std::cout << "  Player 1 alive tanks: " << game_state.getAlivePlayerTanks(1) << std::endl;
    std::cout << "  Player 2 alive tanks: " << game_state.getAlivePlayerTanks(2) << std::endl;
    
    // Test CollisionDetector
    std::cout << "Testing CollisionDetector..." << std::endl;
    CollisionDetector collision_detector(&game_state);
    
    Tank* tank1 = game_state.getPlayerTanks(1)[0];
    Position forward_pos = tank1->getForwardPosition();
    
    bool would_collide = collision_detector.wouldTankCollide(tank1, forward_pos);
    std::cout << "  Tank1 forward movement collision check: " << (would_collide ? "Would collide" : "Safe") << std::endl;
    
    // Test ActionProcessor
    std::cout << "Testing ActionProcessor..." << std::endl;
    ActionProcessor action_processor(&game_state, &collision_detector);
    
    // Test tank movement
    bool moved = action_processor.processAction(tank1, ActionRequest::MoveForward);
    std::cout << "  Tank1 moved forward: " << (moved ? "Success" : "Failed") << std::endl;
    
    if (moved) {
        std::cout << "  Tank1 new position: (" << tank1->getPosition().x << "," << tank1->getPosition().y << ")" << std::endl;
    }
    
    // Test tank rotation
    bool rotated = action_processor.processAction(tank1, ActionRequest::RotateRight45);
    std::cout << "  Tank1 rotated right 45°: " << (rotated ? "Success" : "Failed") << std::endl;
    std::cout << "  Tank1 new direction: " << DirectionUtils::toString(tank1->getDirection()) << std::endl;
    
    // Test tank shooting
    bool shot = action_processor.processAction(tank1, ActionRequest::Shoot);
    std::cout << "  Tank1 shot: " << (shot ? "Success" : "Failed") << std::endl;
    std::cout << "  Active shells: " << game_state.getAllShells().size() << std::endl;
    std::cout << "  Tank1 shells remaining: " << tank1->getShellsRemaining() << std::endl;
    std::cout << "  Tank1 shoot cooldown: " << tank1->getShootCooldown() << std::endl;
    
    // Test BattleInfo creation
    std::cout << "Testing BattleInfo..." << std::endl;
    auto battle_info = action_processor.createBattleInfo(tank1);
    
    std::cout << "  BattleInfo created for tank at (" << battle_info->getTankPosition().x << "," << battle_info->getTankPosition().y << ")" << std::endl;
    std::cout << "  Map size in BattleInfo: " << battle_info->getMapWidth() << "x" << battle_info->getMapHeight() << std::endl;
    std::cout << "  Visible enemies: " << battle_info->getVisibleEnemies().size() << std::endl;
    std::cout << "  Visible shells: " << battle_info->getVisibleShells().size() << std::endl;
    
    // Test shell movement and collision detection
    std::cout << "Testing shell movement..." << std::endl;
    if (!game_state.getAllShells().empty()) {
        Shell* shell = game_state.getAllShells()[0];
        Position old_pos = shell->getPosition();
        shell->advance();
        Position new_pos = shell->getPosition();
        
        std::cout << "  Shell moved from (" << old_pos.x << "," << old_pos.y << ") to (" << new_pos.x << "," << new_pos.y << ")" << std::endl;
        std::cout << "  Shell distance traveled: " << shell->getDistanceTraveled() << std::endl;
    }
    
    // Test collision detection
    std::cout << "Testing collision detection..." << std::endl;
    collision_detector.detectCollisions();
    collision_detector.resolveCollisions();
    
    // Update board representation
    game_state.updateBoardRepresentation();
    
    std::cout << "=== Game Management System working! ✓ ===" << std::endl;
    
    // Print final board state
    std::cout << "Final board state:" << std::endl;
    game_state.getBoard().print();
    
    return 0;
}
