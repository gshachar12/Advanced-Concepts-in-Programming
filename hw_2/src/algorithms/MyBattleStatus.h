#ifndef BATTLEUTILS_H
#define BATTLEUTILS_H
#include <vector>
#include "Direction.h"

class MyBattleStatus {
public:
    size_t arena_width{0};
    size_t arena_height{0};
    size_t max_steps{0};
    size_t ammo_count{0};
    size_t current_turn{0};
    size_t weapon_cooldown{0};
    size_t last_requested_info_turn{0};

    Direction::DirectionType vehicle_orientation;
    Position vehicle_location = {-1, -1};

    enum class TerrainObject {
        WALL,
        MINE,
        SHELL,
        TANK_PLAYER_1,
        TANK_PLAYER_2,
        CURRENT_TANK,
        EMPTY,
        OUTSIDE
    };

    explicit MyBattleStatus(int player_id, int tank_index);

    void updateBoard(const std::vector<std::vector<char> > &updated_board);

    Position normalizeCoordinates(Position p) const;

    void updateBattleStatusBaseAction(ActionRequest action);

    char getTerrainAt(int x, int y) const;

    char getTerrainAt(Position p) const;

    size_t calculateDistance(const Position &pos1, const Position &pos2) const;

    bool isProjectileInVicinity(Position position, size_t thresh = 6) const;

    bool isProjectileInVicinity(size_t thresh = 6) const;

    bool isOpponentNearby(Position position) const;

    bool isOpponentNearby() const;

    size_t getOpponentVehicleCount() const;

    std::vector<Position> getOpponentLocations() const;

    bool hasAmmunition() const;

    bool isWeaponReady() const;

    bool canHitTarget(bool include_projectiles = false) const;

    bool canHitTarget(Position target) const;

    bool canHitTarget(Direction::DirectionType dir, bool include_projectiles = false) const;

    bool isSafePosition(Position p, bool immediate_safe = false) const;

    std::vector<Direction::DirectionType> getSafeDirections(Position position) const;

    ActionRequest calculateOptimalRotation(Direction::DirectionType to_direction) const;

    static char terrainObjectToChar(const TerrainObject obj) {
        switch (obj) {
            case TerrainObject::WALL: return '#';
            case TerrainObject::SHELL: return '*';
            case TerrainObject::MINE: return '@';
            case TerrainObject::TANK_PLAYER_1: return '1';
            case TerrainObject::TANK_PLAYER_2: return '2';
            case TerrainObject::CURRENT_TANK: return '%';
            case TerrainObject::EMPTY: return ' ';
            case TerrainObject::OUTSIDE: return '&';
            default: return '?';
        }
    }
    
    // Compatibility methods to ensure consistent behavior with original code
    Position wrapPosition(Position p) const { return normalizeCoordinates(p); }
    Position getTankPosition() const { return vehicle_location; }
    char getBoardItem(int x, int y) const { return getTerrainAt(x, y); }
    char getBoardItem(Position p) const { return getTerrainAt(p); }
    size_t getDistance(const Position &pos1, const Position &pos2) const { return calculateDistance(pos1, pos2); }
    bool isShellClose(Position position, size_t thresh = 6) const { return isProjectileInVicinity(position, thresh); }
    bool isShellClose(size_t thresh = 6) const { return isProjectileInVicinity(thresh); }
    bool isEnemyClose(Position position) const { return isOpponentNearby(position); }
    bool isEnemyClose() const { return isOpponentNearby(); }
    size_t getEnemyTankCounts() const { return getOpponentVehicleCount(); }
    std::vector<Position> getEnemyPositions() const { return getOpponentLocations(); }
    bool hasTankAmmo() const { return hasAmmunition(); }
    bool canTankShoot() const { return isWeaponReady(); }
    bool canTankShootEnemy(bool include_shells = false) const { return canHitTarget(include_shells); }
    bool canTankShootEnemy(Position enemy) const { return canHitTarget(enemy); }
    bool canTankShootEnemy(Direction::DirectionType dir, bool include_shells = false) const { return canHitTarget(dir, include_shells); }
    ActionRequest rotateTowards(Direction::DirectionType dir) const { return calculateOptimalRotation(dir); }
    static char boardItemToChar(const TerrainObject b) { return terrainObjectToChar(b); }
    
    // Test helper methods
    void setTestCondition_Threatened(bool is_threatened) {
        test_is_threatened = is_threatened;
    }
    
    void setTestCondition_CanShoot(bool can_shoot) {
        test_can_shoot = can_shoot;
    }
    
    void setTestCondition_CanShootFromDirection(Direction::DirectionType dir) {
        test_direction_can_shoot = dir;
        test_direction_shooting_enabled = true;
    }
    
    void configureTestMap(size_t width, size_t height) {
        arena_width = width;
        arena_height = height;
        terrain_grid.resize(height, std::vector<char>(width, ' '));
    }
    
    void setWallAt(Position pos) {
        if (pos.x >= 0 && pos.x < static_cast<int>(arena_width) && 
            pos.y >= 0 && pos.y < static_cast<int>(arena_height)) {
            terrain_grid[pos.y][pos.x] = '#';
        }
    }
    
    void addTestEnemy(Position pos) {
        opponent_positions.push_back(pos);
    }
    
    void setupTestRoute(const std::vector<Direction::DirectionType>& route) {
        test_route = route;
    }

private:
    // terrain data based on last satellite update
    std::vector<std::vector<char> > terrain_grid = {};
    std::vector<Position> opponent_positions = {};
    std::vector<Position> friendly_positions = {};
    std::vector<Position> projectile_positions = {};

    int player_id{0};
    int tank_index{0};
    
    // Test control flags
    bool test_is_threatened{false};
    bool test_can_shoot{false};
    bool test_direction_shooting_enabled{false};
    Direction::DirectionType test_direction_can_shoot{Direction::UP};
    std::vector<Direction::DirectionType> test_route;

    char getOpponentIdentifier() const;

    char getFriendlyIdentifier() const;

    void updateVehiclePositions();

    Position getVehicleLocation() const;

    void updateVehicleOrientationAfterAction(ActionRequest action);

    bool isTargetInSightline(Direction::DirectionType dir, Position target) const;
};


#endif //BATTLEUTILS_H
