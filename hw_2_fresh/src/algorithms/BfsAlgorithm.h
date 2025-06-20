#ifndef PATHFINDINGALGORITHM_H
#define PATHFINDINGALGORITHM_H

#include "Direction.h"
#include "MyTankAlgorithm.h"

class PathfindingAlgorithm final : public MyTankAlgorithm {
public:
    explicit PathfindingAlgorithm(int player_id, int tank_index) : MyTankAlgorithm(player_id, tank_index) {
        initLatestEnemyPosition();
    }

    std::string getName() const override { return "PathfindingAlgorithm"; }

    void calculateAction(ActionRequest *request, std::string *request_title) override;

private:
    bool was_threatened{false};
    std::vector<Position> last_enemy_positions = {};

    void initLatestEnemyPosition();

    void updateLatestEnemyPosition();

    void handleAttack(ActionRequest *request, std::string *request_title);

    void tryShootEnemy(ActionRequest *request, std::string *request_title);

    bool rotateToEnemy(ActionRequest *request, std::string *request_title) const;

    void updatePathIfNeeded();

    void handleEmptyPath(ActionRequest *request, std::string *request_title) const;

    void followPathOrRotate(ActionRequest *request, std::string *request_title);

    std::vector<Direction::DirectionType> computeBFS();

    bool hasEnemyMoved() const;
};


#endif //PATHFINDINGALGORITHM_H
