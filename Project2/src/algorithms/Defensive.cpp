#include "Defensive.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>



#include "Defensive.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <set>
#include <vector>

const int MIN_SAFE_DISTANCE = 5;


Defensive::Defensive(int player_index, int tank_index)
    : playerIndex(player_index),
      tankIndex(tank_index),
      currentActionIndex(0),
      position({0, 0}),
      direction(Direction::UP),
      needsBattleInfo(true),
      enemyDetected(false),
      consecutiveShots(0)
{
    initActionSequence();
}

void Defensive::initActionSequence() {
    // Clear any existing actions
    actionSequence.clear();
    
    // First action is always to get battle info

    actionSequence.push_back(ActionRequest::GetBattleInfo);
    
    // Create a defensive sequence based on player and tank index
    // This algorithm prioritizes safety and strategic positioning
    
    // Begin with a scanning rotation to get a view of surroundings
    actionSequence.push_back(ActionRequest::RotateRight45);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    actionSequence.push_back(ActionRequest::RotateRight45);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    actionSequence.push_back(ActionRequest::RotateRight45);
    actionSequence.push_back(ActionRequest::GetBattleInfo);
    actionSequence.push_back(ActionRequest::RotateRight45);
    
    // Reset action index
    currentActionIndex = 0;
}

ActionRequest Defensive::getAction() {

    if (needsBattleInfo) {
        needsBattleInfo = false;
        return ActionRequest::GetBattleInfo;
    }

    if (currentActionIndex >= actionSequence.size()) {
        currentActionIndex = 0;
        needsBattleInfo = true;
        return ActionRequest::GetBattleInfo;
    }

    ActionRequest action = actionSequence[currentActionIndex++];

    if (action == ActionRequest::Shoot) {
        if (consecutiveShots >= 3) {
            consecutiveShots = 0;
            return ActionRequest::RotateRight45;
        }
        consecutiveShots++;
    } else {
        consecutiveShots = 0;
    }

    return action;
}


void Defensive::ChaseBFS(const TankBattleInfo& info) {
    // This method would implement a BFS algorithm to chase enemies
    // For now, we'll keep it simple and just return
    // Initialize a visited array to track explored positions
    std::vector<std::vector<bool>> visited(info.getWidth(), 
        std::vector<bool>(info.getHeight(), false));

    // Initialize queue with starting position
    std::queue<Position> queue;
    queue.push(position);
    visited[position.x][position.y] = true;

    // BFS traversal
    while (!queue.empty()) {
        Position current = queue.front();
        queue.pop();

        // Check for enemy tanks
        for (const auto& tank : info.getTanks()) {
            if (!tank.isMyTank) {
                // Found enemy - update action sequence to move toward it
                actionSequence.clear();
                actionSequence.push_back(ActionRequest::GetBattleInfo);
                actionSequence.push_back(ActionRequest::Shoot);
                return;
            }
        }

        // Check adjacent positions
        const int dx[] = {0, 0, 1, -1};
        const int dy[] = {1, -1, 0, 0};
        
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];
            
            // Check bounds and if not visited
            if (newX >= 0 && newX < info.getWidth() && 
                newY >= 0 && newY < info.getHeight() && 
                !visited[newX][newY]) {
                
                queue.push({newX, newY});
                visited[newX][newY] = true;
            }
        }
    }


}


void Defensive::updateBattleInfo(BattleInfo& info) {


    TankBattleInfo* battleInfo = dynamic_cast<TankBattleInfo*>(&info);

    if (!battleInfo) { // invalid battle info
        std::cerr << "Defensive algorithm received invalid battle info!" << std::endl;
        return;
    }
    position = battleInfo->getPosition();
    direction = battleInfo->getDirection();

    // Start with getting new info every time
    actionSequence.clear();
    actionSequence.push_back(ActionRequest::GetBattleInfo);

    // Run away from nearby shells
    const auto& shells = battleInfo->getShells();  
    for (const auto& shell : shells) {

        if (shell.manhattanDistance(position) <= MIN_SAFE_DISTANCE) {  // shell is too close
            actionSequence.push_back(ActionRequest::RotateRight90);
        }

    }

        // Check for a mine in the next tile in front of the tank
    auto [dx, dy] = Directions::directionToOffset(direction);
    Position nextPos = position.offset(dx, dy);

    for (const auto& mine : battleInfo->getMines()) {
        if (mine == nextPos) {
            // Mine is directly ahead — avoid it by rotating
            actionSequence.push_back(ActionRequest::RotateRight45);
            actionSequence.push_back(ActionRequest::GetBattleInfo); // re-check after rotating
            return;
        }
    }


    // Default fallback if no danger
    actionSequence.push_back(ActionRequest::MoveForward);
}



bool Defensive::shouldRetreat(const TankBattleInfo& info) const {
    // Count nearby enemy tanks
    int nearbyEnemies = 0;
    for (const auto& tank : info.getTanks()) {
        if (!tank.isMyTank) {
            int dx = tank.position.x - position.x;
            int dy = tank.position.y - position.y;
            int distance = std::abs(dx) + std::abs(dy);
            
            if (distance < 4) {
                nearbyEnemies++;
            }
        }
    }
    
    // Retreat if outnumbered or low on shells
    return (nearbyEnemies > 1) || (info.getRemainingShells() <= 2);
}

ActionRequest Defensive::planDefensiveAction(const TankBattleInfo& /* info */) {
    // This method would implement more sophisticated defensive logic
    // For now, we'll keep it simple
    return ActionRequest::MoveBackward;
}