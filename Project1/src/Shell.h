#ifndef SHELL_H
#define SHELL_H

#include "GameObject.h"

/**
 * A simple projectile that inherits from GameObject.
 * It moves 2 cells each turn in its current direction.
 */
class Shell : public GameObject {
private:
    bool active;
    int damage;           // Damage inflicted by the shell
    int maxRange;         // Maximum distance the shell can travel
    int distanceTraveled; // Cumulative distance traveled by the shell
    int ownerID;          // ID of the tank that fired this shell

public:
    // Constructor using the Direction type from GameObject.
    Shell(int x, int y, Direction dir)
            : GameObject(x, y, dir, CellType::SHELL),
              active(true),
              damage(1),
              maxRange(9999),
              distanceTraveled(0),
              ownerID(-1)
    {}

    // Accessors
    bool isActive() const { return active; }
    void deactivate() { active = false; ObjectType = CellType::BOOM;}

    int getDamage() const { return damage; }
    void setDamage(int dmg) { damage = dmg; }

    int getMaxRange() const { return maxRange; }
    void setMaxRange(int rng) { maxRange = rng; }

    int getDistanceTraveled() const { return distanceTraveled; }

    int getOwnerID() const { return ownerID; }
    void setOwnerID(int id) { ownerID = id; }

    // Rename the movement function to 'advance' to avoid hiding the base class's move(int, int)
    std::pair<int,int> advance();

    // Override update() so that each turn the shell automatically advances.

    // Print the shell’s current status (for debugging)
    void printStatus() const;
};

#endif // SHELL_H
