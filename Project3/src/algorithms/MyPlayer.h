#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include "../../common/Player.h"
#include "../../common/TankAlgorithm.h"
#include "../../common/SatelliteView.h"
#include <string>
#include <memory>

/**
 * Concrete Player implementation for the plugin architecture
 */
class MyPlayer : public Player {
private:
    std::string name_;
    
public:
    explicit MyPlayer(const std::string& name);
    
    /**
     * Update a specific tank algorithm with battle information and satellite view
     */
    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
    
    /**
     * Get player name for identification
     */
    std::string getName() const;
};

#endif // MY_PLAYER_H
