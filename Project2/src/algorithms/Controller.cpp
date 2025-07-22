#include "Controller.h"
#include <iostream>

Controller::Controller(int player_index, int tank_index)
    : playerIndex(player_index),
      tankIndex(tank_index),
      currentActionIndex(0),
      position({0, 0}),
      direction(Direction::UP),
      needsBattleInfo(true)
{
    initActionSequence();
}

void Controller::initActionSequence() {
    // Create a deterministic action sequence based on player and tank index
    // The sequence is different for each player and tank, but always the same for the same player/tank
    
    // Clear any existing actions
    actionSequence.clear();
    
    // First action is always to get battle info
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    
    // Create a sequence based on player and tank index
    // This is a very simple algorithm for demonstration purposes
    if (playerIndex == 1) {
        // Player 1 tanks
        if (tankIndex % 2 == 0) {
            // Even-indexed tanks: move forward, rotate, shoot pattern
            actionSequence.push_back(ActionRequest::MoveForward);
            actionSequence.push_back(ActionRequest::RotateRight45);
            actionSequence.push_back(ActionRequest::MoveForward);
            actionSequence.push_back(ActionRequest::Shoot);
            actionSequence.push_back(ActionRequest::RotateLeft90);
            actionSequence.push_back(ActionRequest::MoveForward);
        } else {
            // Odd-indexed tanks: rotate, shoot, move backward pattern
            actionSequence.push_back(ActionRequest::RotateLeft45);
            actionSequence.push_back(ActionRequest::Shoot);
            actionSequence.push_back(ActionRequest::MoveBackward);
            actionSequence.push_back(ActionRequest::MoveBackward);
            actionSequence.push_back(ActionRequest::RotateRight90);
            actionSequence.push_back(ActionRequest::MoveForward);
        }
    } else {
        // Player 2 tanks
        if (tankIndex % 2 == 0) {
            // Even-indexed tanks: shoot, rotate, move pattern
            actionSequence.push_back(ActionRequest::Shoot);
            actionSequence.push_back(ActionRequest::RotateLeft45);
            actionSequence.push_back(ActionRequest::MoveForward);
            actionSequence.push_back(ActionRequest::MoveForward);
            actionSequence.push_back(ActionRequest::RotateRight90);
            actionSequence.push_back(ActionRequest::Shoot);
        } else {
            // Odd-indexed tanks: move, shoot, rotate pattern
            actionSequence.push_back(ActionRequest::MoveForward);
            actionSequence.push_back(ActionRequest::MoveForward);
            actionSequence.push_back(ActionRequest::Shoot);
            actionSequence.push_back(ActionRequest::RotateRight45);
            actionSequence.push_back(ActionRequest::MoveBackward);
            actionSequence.push_back(ActionRequest::RotateLeft90);
        }
    }
    
    // Add periodic battle info requests
    for (size_t i = 0; i < actionSequence.size() * 2; i += 3) {
        actionSequence.insert(actionSequence.begin() + i, ActionRequest::GetBattleInfo);
    }
    
    // Reset action index
    currentActionIndex = 0;
}

ActionRequest Controller::getAction() {
    // After some actions, request battle info
    return ActionRequest::Shoot;

    if (needsBattleInfo) {
        needsBattleInfo = false;
        return ActionRequest::GetBattleInfo;
    }
    
    // Get current action
    if (currentActionIndex >= actionSequence.size()) {
        // Loop back to beginning, but first ask for battle info
        currentActionIndex = 0;
        needsBattleInfo = true;
        return ActionRequest::GetBattleInfo;
    }
    
    ActionRequest action = actionSequence[currentActionIndex++];
    
    // Every few actions, request battle info again
    if (currentActionIndex % 5 == 0) {
        needsBattleInfo = true;
    }
    
    return action;
}

void Controller::updateBattleInfo(BattleInfo& info) {
    // Cast to our concrete TankBattleInfo type
    TankBattleInfo* battleInfo = dynamic_cast<TankBattleInfo*>(&info);
    if (!battleInfo) {
        std::cerr << "Error: BattleInfo is not a TankBattleInfo" << std::endl;
        return;
    }
    
    // Update our local state from the battle info
    position = battleInfo->getPosition();
    direction = battleInfo->getDirection();
    
    // Use battle info to make decisions
    // In this simple example, we'll just check for nearby tanks
    const auto& tanks = battleInfo->getTanks();
    if (!tanks.empty()) {
        // If we see enemy tanks, prioritize shooting
        for (const auto& tank : tanks) {
            if (!tank.isMyTank) {
                // Enemy tank spotted, prioritize shooting
                actionSequence.insert(actionSequence.begin() + currentActionIndex, ActionRequest::Shoot);
                break;
            }
        }
    }
}