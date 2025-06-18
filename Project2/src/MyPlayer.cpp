#include "MyPlayer.h"
#include "Tank.h" 
#include "TankBattleInfo.h"
#include <iostream>

void MyPlayer::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    // This method is called when a tank requests battle info
    // We need to identify which tank is making the request and provide relevant battle info
    
    // Get or create a battle info object for this tank
    int tankKey = reinterpret_cast<std::intptr_t>(&tank);
    
    if (tankBattleInfos.find(tankKey) == tankBattleInfos.end()) {
        tankBattleInfos[tankKey] = std::make_unique<TankBattleInfo>(playerIndex);
    }
    
    // Clear previous data
    tankBattleInfos[tankKey]->clear();
    
    // Find the requesting tank position based on the % character in the satellite view
    Position tankPosition{-1, -1};
    
    // Track positions of all objects for potential coordination
    std::vector<Position> enemyTankPositions;
    std::vector<Position> friendlyTankPositions;
    std::vector<Position> minePositions;
    std::vector<Position> wallPositions;
    std::vector<Position> shellPositions;

    
    // First scan: find the requesting tank's position
    for (size_t y = 0; y < boardHeight; ++y) {
        for (size_t x = 0; x < boardWidth; ++x) {
            char object = satellite_view.getObjectAt(x, y);
            if (object == '%') {
                tankPosition = {static_cast<int>(x), static_cast<int>(y)};
                break;
            }
        }
        if (tankPosition.x != -1) break; // Found the tank
    }
    
    // Set tank position in battle info
    tankBattleInfos[tankKey]->setPosition(tankPosition);
    
    // Set remaining shells and shoot ability (defaults since we don't have the real data)
    tankBattleInfos[tankKey]->setRemainingShells(numShells); 
    tankBattleInfos[tankKey]->setCanShoot(true);           
    
    // Second scan: collect all battlefield information
    for (size_t y = 0; y < boardHeight; ++y) {
        for (size_t x = 0; x < boardWidth; ++x) {
            char object = satellite_view.getObjectAt(x, y);
            
            // Process the object based on the satellite view char values
            switch (object) {
                case '%': // Already handled above (requesting tank)
                    break;
                    
                case '1': // Tank owned by player 1
                {
                    TankInfo info;
                    info.position = {static_cast<int>(x), static_cast<int>(y)};
                    info.playerIndex = 1;
                    info.isMyTank = (playerIndex == 1);
                    
                    if (info.isMyTank) {
                        friendlyTankPositions.push_back(info.position);
                    } else {
                        enemyTankPositions.push_back(info.position);
                    }
                    
                    tankBattleInfos[tankKey]->addTankInfo(info);
                    break;
                }
                
                case '2': // Tank owned by player 2
                {
                    TankInfo info;
                    info.position = {static_cast<int>(x), static_cast<int>(y)};
                    info.playerIndex = 2;
                    info.isMyTank = (playerIndex == 2);
                    
                    if (info.isMyTank) {
                        friendlyTankPositions.push_back(info.position);
                    } else {
                        enemyTankPositions.push_back(info.position);
                    }
                    
                    tankBattleInfos[tankKey]->addTankInfo(info);
                    break;
                }
                
                case '#': // Wall
                {
                    Position wallPos{static_cast<int>(x), static_cast<int>(y)};
                    tankBattleInfos[tankKey]->addWall(wallPos);
                    wallPositions.push_back(wallPos);
                    break;
                }
                
                case '@': // Mine
                {
                    Position minePos{static_cast<int>(x), static_cast<int>(y)};
                    tankBattleInfos[tankKey]->addMine(minePos);
                    minePositions.push_back(minePos);
                    break;
                }
                
                case '*': // Artillery shell
                {
                    // Could track shells if needed for advanced strategies
                    Position shellPos{static_cast<int>(x), static_cast<int>(y)};
                    tankBattleInfos[tankKey]->addShell(shellPos);
                    shellPositions.push_back(shellPos);
                    break;
                }
                
                // Other cases can be handled as needed
            }
        }
    }
    
    // Update the tank algorithm with the battle info
    tank.updateBattleInfo(*tankBattleInfos[tankKey]);
}

void MyPlayer::setDirection(TankAlgorithm& tank, Direction direction) {
    // Get tank's battle info
    int tankKey = reinterpret_cast<std::intptr_t>(&tank);
    
    if (tankBattleInfos.find(tankKey) != tankBattleInfos.end()) {
        tankBattleInfos[tankKey]->setDirection(direction);
    }
}