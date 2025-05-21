#ifndef TANK_BATTLE_INFO_H
#define TANK_BATTLE_INFO_H

#include "common/BattleInfo.h"
#include "Position.h"
#include "Directions.h"
#include <vector>
#include <utility>

// Information about other tanks, shared with the algorithm
struct TankInfo {
    Position position;
    int playerIndex;   // 1 or 2
    bool isMyTank;     // true if belongs to the same player
};

class TankBattleInfo : public BattleInfo {
private:
    int playerIndex;                // 1 or 2
    Position position;              // Current tank position
    Direction direction;            // Current tank direction
    size_t remainingShells;         // Remaining shells for this tank
    bool canShoot;                  // Whether the tank can shoot now
    std::vector<TankInfo> tanks;    // Info about visible tanks
    std::vector<Position> mines;    // Positions of visible mines
    std::vector<Position> walls;    // Positions of visible walls

public:
    TankBattleInfo(int playerIdx) 
        : playerIndex(playerIdx), position({0, 0}), direction(Direction::UP),
          remainingShells(0), canShoot(false) {}
    
    // Setters for the player to initialize the battle info
    void setPosition(const Position& pos) { position = pos; }
    void setDirection(Direction dir) { direction = dir; }
    void setRemainingShells(size_t shells) { remainingShells = shells; }
    void setCanShoot(bool can) { canShoot = can; }
    void addTankInfo(const TankInfo& info) { tanks.push_back(info); }
    void addMine(const Position& minePos) { mines.push_back(minePos); }
    void addWall(const Position& wallPos) { walls.push_back(wallPos); }
    
    // Getters for the algorithm to access the battle info
    int getPlayerIndex() const { return playerIndex; }
    Position getPosition() const { return position; }
    Direction getDirection() const { return direction; }
    size_t getRemainingShells() const { return remainingShells; }
    bool getCanShoot() const { return canShoot; }
    const std::vector<TankInfo>& getTanks() const { return tanks; }
    const std::vector<Position>& getMines() const { return mines; }
    const std::vector<Position>& getWalls() const { return walls; }
    
    // Clear all collected battlefield data (to reset between uses)
    void clear() {
        tanks.clear();
        mines.clear();
        walls.clear();
    }
};

#endif // TANK_BATTLE_INFO_H