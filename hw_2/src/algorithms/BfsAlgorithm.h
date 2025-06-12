#ifndef BFSALGORITHM_H
#define BFSALGORITHM_H

#include "Direction.h"
#include "MyTankAlgorithm.h"

// Intelligent tank algorithm that uses breadth-first search for pathfinding
class BfsAlgorithm final : public MyTankAlgorithm {
public:
    // Constructor initializes the tank with player and tank identifiers
    explicit BfsAlgorithm(int player_id, int tank_index) : MyTankAlgorithm(player_id, tank_index) {
        initEnemyPositionTracking();
    }

    // Return algorithm identifier
    std::string getName() const override { return "BfsAlgorithm"; }

    // Main decision-making method for the tank's actions
    void calculateAction(ActionRequest *request, std::string *request_title) override;

private:
    // State tracking
    bool danger_detected{false};
    std::vector<Position> enemy_position_history = {};

    // Enemy position tracking methods
    void initEnemyPositionTracking();
    void refreshEnemyPositions();
    bool haveEnemiesMoved() const;

    // Threat response methods
    void respondToThreat(ActionRequest *request, std::string *request_title);
    void attemptToEngageEnemy(ActionRequest *request, std::string *request_title);
    bool turnTowardsEnemy(ActionRequest *request, std::string *request_title) const;

    // Path management methods
    void updateNavigationPath();
    void handleNoValidPath(ActionRequest *request, std::string *request_title) const;
    void followCurrentPath(ActionRequest *request, std::string *request_title);
    
    // Core pathfinding algorithm
    std::vector<Direction::DirectionType> findPathWithBFS();
};


#endif //BFSALGORITHM_H
