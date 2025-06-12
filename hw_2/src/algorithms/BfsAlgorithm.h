#ifndef BFSALGORITHM_H
#define BFSALGORITHM_H

#include "Direction.h"
#include "MyTankAlgorithm.h"

class BfsAlgorithm final : public MyTankAlgorithm {
public:
    explicit BfsAlgorithm(int player_id, int tank_index) : MyTankAlgorithm(player_id, tank_index) {
        initLatestEnemyPosition();
    }

    std::string getName() const override { return "BfsAlgorithm"; }

    void calculateAction(ActionRequest *request, std::string *request_title) override;

private:
    bool was_threatened{false};
    std::vector<Position> last_enemy_positions = {};

    void initLatestEnemyPosition();

    void updateLatestEnemyPosition();

    void handleTankThreatened(ActionRequest *request, std::string *request_title);

    void tryShootEnemy(ActionRequest *request, std::string *request_title);

    bool rotateToEnemy(ActionRequest *request, std::string *request_title) const;

    void updatePathIfNeeded();

    void handleEmptyPath(ActionRequest *request, std::string *request_title) const;

    void followPathOrRotate(ActionRequest *request, std::string *request_title);

    std::vector<Direction::DirectionType> computeBFS();

    bool hasEnemyMoved() const;
};


#endif //BFSALGORITHM_H
