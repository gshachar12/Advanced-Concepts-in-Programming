#ifndef TANK_BATTLE_INFO_H
#define TANK_BATTLE_INFO_H

#include "common/BattleInfo.h"
#include "Position.h"
#include "Directions.h"
#include <vector>
#include <utility>

struct TankInfo {
    Position position;
    int playerIndex;   // 1 or 2
    bool isMyTank;
};

class TankBattleInfo : public BattleInfo {
private:
    int playerIndex;
    Position position;
    Direction direction;
    size_t remainingShells;
    bool canShoot;
    std::vector<TankInfo> tanks;
    std::vector<Position> mines;
    std::vector<Position> walls;
    std::vector<Position> shells; 
    // New fields
    int boardWidth = 0;
    int boardHeight = 0;

public:
    TankBattleInfo(int playerIdx) 
        : playerIndex(playerIdx), position({0, 0}), direction(Direction::UP),
          remainingShells(0), canShoot(false) {}

    // Setters
    void setPosition(const Position& pos) { position = pos; }
    void setDirection(Direction dir) { direction = dir; }
    void setRemainingShells(size_t shells) { remainingShells = shells; }
    void setCanShoot(bool can) { canShoot = can; }
    void addTankInfo(const TankInfo& info) { tanks.push_back(info); }
    void addMine(const Position& minePos) { mines.push_back(minePos); }
    void addWall(const Position& wallPos) { walls.push_back(wallPos); }
    void addShell(const Position& shellPos) { shells.push_back(shellPos); }

    // Set board dimensions
    void setBoardSize(int width, int height) {
        boardWidth = width;
        boardHeight = height;
    }

    // Get board dimensions
    int getWidth() const { return boardWidth; }
    int getHeight() const { return boardHeight; }

    // Getters for battle info
    int getPlayerIndex() const { return playerIndex; }
    Position getPosition() const { return position; }
    Direction getDirection() const { return direction; }
    const std::vector<Position>& getShells() const { return shells; }
    size_t getRemainingShells() const { return remainingShells; }
    bool getCanShoot() const { return canShoot; }
    const std::vector<TankInfo>& getTanks() const { return tanks; }
    const std::vector<Position>& getMines() const { return mines; }
    const std::vector<Position>& getWalls() const { return walls; }

    // Clear all collected data
    void clear() {
        tanks.clear();
        mines.clear();
        walls.clear();
    }
};

#endif // TANK_BATTLE_INFO_H
