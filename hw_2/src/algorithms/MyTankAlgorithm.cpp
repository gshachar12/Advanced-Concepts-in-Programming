#include "MyTankAlgorithm.h"

#include "Logger.h"
#include "MyBattleInfo.h"

MyTankAlgorithm::MyTankAlgorithm(const int player_id, const int tank_index): player_id{player_id},
                                                                             tank_index(tank_index),
                                                                             battle_status(
                                                                                 MyBattleStatus(
                                                                                     player_id, tank_index)) {
}


void MyTankAlgorithm::updateBattleInfo(BattleInfo &info) {
    if (const MyBattleInfo *my_battle_info = dynamic_cast<MyBattleInfo *>(&info)) {
        battle_status.updateBoard(my_battle_info->getBoard());
        battle_status.ammo_count = my_battle_info->getNumShells();
        battle_status.max_steps = my_battle_info->getMaxSteps();
    }
}

void MyTankAlgorithm::printLogs(const std::string &msg) const {
    Logger::getInstance().log("Player " + std::to_string(player_id) + " - Tank Index " +
                              std::to_string(tank_index) + " " + msg);
}

ActionRequest MyTankAlgorithm::getAction() {
    ActionRequest request = ActionRequest::DoNothing;
    std::string request_title = "Doing nothing";
    calculateAction(&request, &request_title);
    battle_status.current_turn++;
    printLogs(request_title);
    battle_status.updateBattleStatusBaseAction(request);
    return request;
}

bool MyTankAlgorithm::isTankThreatened() const {
    if (battle_status.isProjectileInVicinity()) {
        printLogs("Threatened by shells");
        return true;
    }
    if (battle_status.isOpponentNearby()) {
        printLogs("Threatened by enemy");
        return true;
    }
    return false;
}


ActionRequest MyTankAlgorithm::moveIfThreatened() const {
    // we'll try first moving forward in the current direction
    Position forward_pos = battle_status.normalizeCoordinates(battle_status.vehicle_location + battle_status.vehicle_orientation);

    if (battle_status.isSafePosition(forward_pos)) {
        return ActionRequest::MoveForward;
    }

    // If we can't move forward in the current direction, we'll find a safe cell around us and rotate towards it
    for (int i = 0; i < Direction::getDirectionSize(); ++i) {
        Direction::DirectionType possible_dir = Direction::getDirectionFromIndex(i);
        Position possible_pos = battle_status.normalizeCoordinates(battle_status.vehicle_location + possible_dir);
        if (battle_status.isSafePosition(possible_pos)) {
            return battle_status.calculateOptimalRotation(possible_dir);
        }
    }

    if (battle_status.isSafePosition(forward_pos, true)) {
        return ActionRequest::MoveForward;
    }

    return ActionRequest::DoNothing;
}
