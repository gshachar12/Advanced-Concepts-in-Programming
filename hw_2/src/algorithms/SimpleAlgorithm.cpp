#include "SimpleAlgorithm.h"
#include "Logger.h"

void SimpleAlgorithm::calculateAction(ActionRequest *request, std::string *request_title) {
    if (battle_status.current_turn == 0 || was_threatened) {
        was_threatened = false;
        *request = ActionRequest::GetBattleInfo;
        *request_title = "Requesting Battle Info (first turn or previously threatened)";
        return;
    }
    if (isTankThreatened()) {
        was_threatened = true;
        *request = moveIfThreatened();
        *request_title = "Moving because currently threatened";
        return;
    }
    if (battle_status.canHitTarget()) {
        *request = ActionRequest::Shoot;
        *request_title = "Shooting opponent in direct line of fire";
    }

    battle_status.last_requested_info_turn = battle_status.current_turn + 1;
    *request = ActionRequest::GetBattleInfo;
    *request_title = "Requesting updated Battle Info";
}
