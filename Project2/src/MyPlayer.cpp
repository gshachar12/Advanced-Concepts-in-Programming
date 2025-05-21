#include "MyPlayer.h"
#include "Tank.h" // We need this for dynamic_cast
#include "TankBattleInfo.h"

void MyPlayer::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    // This method is called when a tank requests battle info
    // We need to identify which tank is making the request (through dynamic_cast or other means)
    // Then populate battle info for the tank
    
    // Try to identify the tank by checking if it's a custom algorithm
    // Note: In a real implementation, we would need a way to map the TankAlgorithm to its Tank
    // For now, we'll create a new battle info if one doesn't exist for this tank
    
    // Get or create a battle info object for this tank
    // In a proper implementation, we'd have the tankID passed to us
    // For now, we'll generate a hash from the address of the algorithm for the map key
    int tankKey = reinterpret_cast<std::intptr_t>(&tank);
    
    if (tankBattleInfos.find(tankKey) == tankBattleInfos.end()) {
        tankBattleInfos[tankKey] = std::make_unique<TankBattleInfo>(playerIndex);
    }
    
    // Clear previous data
    tankBattleInfos[tankKey]->clear();
    
    // We don't have direct access to the tank's information from this method
    // In a real implementation, we'd store these or pass them in
    // For now, we'll populate with basic scan data from the satellite view
    
    // Scan the board around the tank to populate battle info
    // This is a simplified example; in a real implementation, we would
    // know the tank's position and other details
    
    // Sample positions from the satellite view to gather information
    // In a real game, this would be more targeted and efficient
    for (size_t y = 0; y < boardHeight; ++y) {
        for (size_t x = 0; x < boardWidth; ++x) {
            char object = satellite_view.getObjectAt(x, y);
            
            // Process the object based on the satellite view char values
            switch (object) {
                case '%': // This is the requesting tank
                {
                    Position pos{static_cast<int>(x), static_cast<int>(y)};
                    tankBattleInfos[tankKey]->setPosition(pos);
                    // We would also set direction and other tank-specific details here
                    // but we don't have this information in this method
                    break;
                }
                case '1': // Tank owned by player 1
                {
                    TankInfo info;
                    info.position = {static_cast<int>(x), static_cast<int>(y)};
                    info.playerIndex = 1;
                    info.isMyTank = (playerIndex == 1);
                    tankBattleInfos[tankKey]->addTankInfo(info);
                    break;
                }
                case '2': // Tank owned by player 2
                {
                    TankInfo info;
                    info.position = {static_cast<int>(x), static_cast<int>(y)};
                    info.playerIndex = 2;
                    info.isMyTank = (playerIndex == 2);
                    tankBattleInfos[tankKey]->addTankInfo(info);
                    break;
                }
                case '#': // Wall
                {
                    Position wallPos{static_cast<int>(x), static_cast<int>(y)};
                    tankBattleInfos[tankKey]->addWall(wallPos);
                    break;
                }
                case '@': // Mine
                {
                    Position minePos{static_cast<int>(x), static_cast<int>(y)};
                    tankBattleInfos[tankKey]->addMine(minePos);
                    break;
                }
                // Other cases can be handled as needed
            }
        }
    }
    
    // Update the tank algorithm with the battle info
    // This gives the tank algorithm access to the BattleInfo object
    tank.updateBattleInfo(*tankBattleInfos[tankKey]);
}