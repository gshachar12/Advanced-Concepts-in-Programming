#ifndef TANK_H
#define TANK_H
#include <memory>
#include "CellType.h"
#include "GameObject.h"
#include "Directions.h"
#include "Board.h"
#include "common/TankAlgorithm.h"
#include "common/ActionRequest.h"
#include <algorithm>
#include <iostream>
#include <vector>

class Board;

// For backward movement state:
enum class BackwardState {
    NOT_REQUESTED,
    WAITING_1,
    WAITING_2,
    MOVING_BACKWARD
};

// Constants for tank properties
extern const int DEFAULT_SHELL_COUNT;
extern const int SHOOT_COOLDOWN;

class Tank : public GameObject {
private:
    bool alive;
    size_t shellCount;      // Now based on the board's NumShells parameter
    int shootCooldown;     // Cooldown steps after shooting
    int tankID;            // Unique ID for the tank
    int playerIndex;       // 1 or 2 for which player owns this tank
    int tankIndex;         // Index within the player's tanks (0-based)
    std::unique_ptr<TankAlgorithm> algorithm; // Algorithm that controls this tank
    BackwardState backwardState;
    bool actionWasIgnored;  // Track if last action was ignored
    bool killedThisRound;   // Track if tank was killed in current round
    std::string lastAction; // Last action performed by the tank (for output file)

public:
    // Constructor with TankAlgorithm
    Tank(int x, int y, Direction dir, CellType objectType, int id, int player, int index, size_t initialShells, std::unique_ptr<TankAlgorithm> algo)
            : GameObject(x, y, dir, objectType),
              alive(true),
              shellCount(initialShells),
              shootCooldown(0),
              tankID(id),
              playerIndex(player),
              tankIndex(index),
              algorithm(std::move(algo)),
              backwardState(BackwardState::NOT_REQUESTED),
              actionWasIgnored(false),
              killedThisRound(false),
              lastAction("")
    {}

    void update();
    virtual ~Tank() = default;
    
    // Getters
    bool isAlive() const { return alive; }
    void destroy() { alive = false; killedThisRound = true; }
    int getTankID() const { return tankID; }
    int getPlayerIndex() const { return playerIndex; }
    int getTankIndex() const { return tankIndex; }
    size_t getShellCount() const { return shellCount; }
    int getShootCooldown() const { return shootCooldown; }
    bool canShoot() const { return (shootCooldown == 0 && shellCount > 0 && alive); }
    BackwardState getBackwardState() const { return backwardState; }
    bool wasActionIgnored() const { return actionWasIgnored; }
    bool wasKilledThisRound() const { return killedThisRound; }
    
    // Reset state for next round
    void resetRoundState() { 
        actionWasIgnored = false;
        killedThisRound = false;
    }
    
    // Get the formatted action string for output file
    std::string getActionString() const;

    // Get next action from the algorithm
    ActionRequest getAction() { 
        if (!algorithm || !isAlive()) return ActionRequest::DoNothing;
        return algorithm->getAction();
    }
    
    // Update the algorithm with battle info
    void updateAlgorithm(BattleInfo& info) {
        if (algorithm && isAlive()) {
            algorithm->updateBattleInfo(info);
        }
    }

    // Actions
    void shoot();                             // Shoot
    std::pair<int,int> moveForward(Board board);    // Move 1 step in current direction.
    std::pair<int,int> moveBackward(Board board);   // Move 1 step in opposite direction.

    void requestBackward();
    void cancelBackward();
    
    // Set the last action performed
    void setLastAction(const std::string& action, bool ignored = false) {
        lastAction = action;
        actionWasIgnored = ignored;
    }

    void printPosition();
    void printStatus() const;

    // Rotation helpers
    void rotateLeft45();   // Rotate 45 degrees counter-clockwise
    void rotateRight45();  // Rotate 45 degrees clockwise
    void rotateLeft90();   // Rotate 90 degrees counter-clockwise
    void rotateRight90();  // Rotate 90 degrees clockwise

    // Get a reference to the algorithm for the Player to access
    TankAlgorithm* getAlgorithm() const { 
        return algorithm.get();
    }

private:
    // Helpers for rotation
    int findDirectionIndex(const Direction &d) const;
    void setDirectionByIndex(int idx);
};

#endif // TANK_H