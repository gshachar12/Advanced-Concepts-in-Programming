#include "MyTankAlgorithm.h"
#include "Logger.h"
#include "MyBattleInfo.h"

// Constructor initializes the algorithm with player and tank identifiers
// and creates a battle status tracker specific to this tank
MyTankAlgorithm::MyTankAlgorithm(const int player_id, const int tank_index) 
    : player_id(player_id),
      tank_index(tank_index),
      battle_status(MyBattleStatus(player_id, tank_index)) {
    // No additional initialization needed
}

// Updates the internal battle status with new information from the game
void MyTankAlgorithm::updateBattleInfo(BattleInfo &info) {
    // Try to cast to our specific battle info type
    auto* battleInfoPtr = dynamic_cast<MyBattleInfo*>(&info);
    
    // Update our status if the cast was successful
    if (battleInfoPtr) {
        // Update the board representation
        battle_status.updateBoard(battleInfoPtr->getBoard());
        
        // Update game parameters
        battle_status.num_shells = battleInfoPtr->getNumShells();
        battle_status.max_steps = battleInfoPtr->getMaxSteps();
    }
}

// Helper method to log tank actions with proper identification
void MyTankAlgorithm::printLogs(const std::string &msg) const {
    // Format: "Player X - Tank Index Y: Message"
    std::string logMessage = "Player " + std::to_string(player_id) + 
                           " - Tank Index " + std::to_string(tank_index) + 
                           " " + msg;
    
    // Send to the logging system
    Logger::getInstance().log(logMessage);
}

// Primary method to determine the next action for this tank
ActionRequest MyTankAlgorithm::getAction() {
    // Default to doing nothing unless the strategy determines otherwise
    ActionRequest requestedAction = ActionRequest::DoNothing;
    std::string actionDescription = "Doing nothing";
    
    // Determine the best action based on current battle conditions
    calculateAction(&requestedAction, &actionDescription);
    
    // Increment turn counter to track battle progression
    battle_status.turn_number++;
    
    // Log the chosen action for debugging
    printLogs(actionDescription);
    
    // Update internal state based on the action we're about to take
    battle_status.updateBattleStatusBaseAction(requestedAction);
    
    // Return the selected action to the game engine
    return requestedAction;
}

// Determines if the tank is in immediate danger
bool MyTankAlgorithm::isTankThreatened() const {
    // Check for nearby enemy shells first (highest priority threat)
    if (battle_status.isShellClose()) {
        // Log the threat type
        printLogs("Threatened by shells");
        return true;
    }
    
    // Check for nearby enemy tanks second
    if (battle_status.isEnemyClose()) {
        // Log the threat type
        printLogs("Threatened by enemy");
        return true;
    }
    
    // No immediate threats detected
    return false;
}


// Determine the best evasive action when threatened
ActionRequest MyTankAlgorithm::moveIfThreatened() const {
    // STRATEGY 1: Try moving forward if that position is safe
    // Calculate the position directly ahead of us
    Position positionAhead = battle_status.wrapPosition(
        battle_status.tank_position + battle_status.tank_direction
    );

    // Check if moving forward is safe
    if (battle_status.checkPosition(positionAhead)) {
        // Safe to move forward
        return ActionRequest::MoveForward;
    }

    // STRATEGY 2: Look in all possible directions for a safe position
    const int directionCount = Direction::getDirectionSize();
    for (int dirIdx = 0; dirIdx < directionCount; ++dirIdx) {
        // Get direction type from index
        Direction::DirectionType candidateDirection = Direction::getDirectionFromIndex(dirIdx);
        
        // Calculate position in this direction
        Position candidatePosition = battle_status.wrapPosition(
            battle_status.tank_position + candidateDirection
        );
        
        // Check if this position would be safe
        if (battle_status.checkPosition(candidatePosition)) {
            // Found a safe direction - rotate toward it
            return battle_status.rotateTowards(candidateDirection);
        }
    }
    
    // STRATEGY 3: Less strict safety check for forward movement
    // If no completely safe position found, try moving forward with relaxed safety requirements
    if (battle_status.checkPosition(positionAhead, true)) {
        return ActionRequest::MoveForward;
    }
    
    // STRATEGY 4: No safe moves found
    // Stay in place as a last resort
    return ActionRequest::DoNothing;
}
