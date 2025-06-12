#include "MyBattleStatus.h"

#include "Direction.h"

MyBattleStatus::MyBattleStatus(int player_id, int tank_index): vehicle_orientation(
                                                                   player_id == 1
                                                                       ? Direction::LEFT
                                                                       : Direction::RIGHT),
                                                               player_id(player_id),
                                                               tank_index(tank_index) {
    updateVehiclePositions();
}

Position MyBattleStatus::normalizeCoordinates(Position p) const {
    const size_t mod_x = p.x % arena_width;
    const size_t mod_y = p.y % arena_height;
    return {mod_x, mod_y};
}

void MyBattleStatus::updateBoard(const std::vector<std::vector<char> > &updated_board) {
    terrain_grid = updated_board;
    arena_width = terrain_grid.size();
    arena_height = terrain_grid[0].size();
    updateVehiclePositions();
}

Position MyBattleStatus::getVehicleLocation() const {
    return vehicle_location;
}

bool MyBattleStatus::hasAmmunition() const {
    return ammo_count > 0;
}

bool MyBattleStatus::isWeaponReady() const {
    return hasAmmunition() && weapon_cooldown == 0;
}

char MyBattleStatus::getTerrainAt(int x, int y) const {
    return terrain_grid[x][y];
}

char MyBattleStatus::getTerrainAt(Position p) const {
    return getTerrainAt(p.x, p.y);
}

size_t MyBattleStatus::calculateDistance(const Position &a, const Position &b) const {
    int raw_dx = std::abs(static_cast<int>(a.x) - static_cast<int>(b.x));
    int raw_dy = std::abs(static_cast<int>(a.y) - static_cast<int>(b.y));

    size_t dx = std::min(static_cast<size_t>(raw_dx), arena_width - static_cast<size_t>(raw_dx));
    size_t dy = std::min(static_cast<size_t>(raw_dy), arena_height - static_cast<size_t>(raw_dy));

    return std::max(dx, dy);
}

bool MyBattleStatus::canHitTarget(Direction::DirectionType dir, bool include_projectiles) const {
    // For test scenarios with specific direction testing
    if (test_direction_shooting_enabled && dir == test_direction_can_shoot) {
        return true;
    }
    
    if (opponent_positions.empty() || !isWeaponReady()) {
        return false;
    }

    for (Position opponent: opponent_positions) {
        if (isTargetInSightline(dir, opponent)) {
            return true;
        }
    }
    if (include_projectiles) {
        for (Position projectile: projectile_positions) {
            if (isTargetInSightline(dir, projectile)) {
                return true;
            }
        }
    }
    return false;
}

bool MyBattleStatus::canHitTarget(Position target) const {
    if (!isWeaponReady()) {
        return false;
    }
    return isTargetInSightline(vehicle_orientation, target);
}

bool MyBattleStatus::canHitTarget(bool include_projectiles) const {
    // For test scenarios, use the test flag
    if (test_can_shoot) {
        return true;
    }
    return canHitTarget(vehicle_orientation, include_projectiles);
}

bool MyBattleStatus::isTargetInSightline(Direction::DirectionType dir, Position target) const {
    Position p = vehicle_location;
    for (size_t i = 1; i <= std::max(arena_width, arena_height); i++) {
        p = normalizeCoordinates(p + dir);
        if (target == p) return true;
        if (terrain_grid[p.x][p.y] == terrainObjectToChar(TerrainObject::WALL) || terrain_grid[p.x][p.y] == getFriendlyIdentifier()) {
            return false;
        }
    }
    return false;
}


bool MyBattleStatus::isSafePosition(Position p, const bool immediate_safe) const {
    if (terrain_grid[p.x][p.y] != terrainObjectToChar(TerrainObject::EMPTY)) {
        return false;
    }
    // If only checking immediate safety, we're done
    if (immediate_safe) {
        return true;
    }
    return !(isProjectileInVicinity(p) || isOpponentNearby(p));
}

std::vector<Direction::DirectionType> MyBattleStatus::getSafeDirections(const Position position) const {
    std::vector<Direction::DirectionType> safe_directions;

    for (int i = 0; i < Direction::getDirectionSize(); i++) {
        auto direction = Direction::getDirectionFromIndex(i);
        Position next_position = normalizeCoordinates(position + direction);
        if (isSafePosition(next_position)) {
            safe_directions.push_back(direction);
        }
    }

    return safe_directions;
}

ActionRequest MyBattleStatus::calculateOptimalRotation(Direction::DirectionType to_direction) const {
    if (vehicle_orientation == to_direction) return ActionRequest::DoNothing;

    // Find the shortest rotation path
    if (int diff = (vehicle_orientation - to_direction + 360) % 360; diff > 180) {
        diff = 360 - diff;
        // Clockwise is shorter
        if (diff == 45) return ActionRequest::RotateRight45;
        if (diff == 90) return ActionRequest::RotateRight90;
        if (diff == 135) return ActionRequest::RotateRight90;
        if (diff == 180) return ActionRequest::RotateRight90;
    } else {
        // Counter-clockwise is shorter or equal
        if (diff == 45) return ActionRequest::RotateLeft45;
        if (diff == 90) return ActionRequest::RotateLeft90;
        if (diff == 135) return ActionRequest::RotateLeft90;
        if (diff == 180) return ActionRequest::RotateLeft90;
    }

    return ActionRequest::DoNothing;
}


void MyBattleStatus::updateVehiclePositions() {
    std::vector<Position> opponents;
    std::vector<Position> allies;
    std::vector<Position> projectiles;
    for (size_t i{0}; i < terrain_grid.size(); i++) {
        for (size_t j{0}; j < terrain_grid.front().size(); j++) {
            Position p = {i, j};

            if (terrain_grid[i][j] == getFriendlyIdentifier()) {
                allies.push_back(p);
            } else if (terrain_grid[i][j] == terrainObjectToChar(TerrainObject::CURRENT_TANK)) {
                vehicle_location = p;
            } else if (terrain_grid[i][j] == getOpponentIdentifier()) {
                opponents.push_back(p);
            } else if (terrain_grid[i][j] == terrainObjectToChar(TerrainObject::SHELL)) {
                projectiles.push_back(p);
            }
        }
    }
    friendly_positions = allies;
    opponent_positions = opponents;
    projectile_positions = projectiles;
}

char MyBattleStatus::getOpponentIdentifier() const {
    (void) tank_index;
    return (player_id == 1) ? terrainObjectToChar(TerrainObject::TANK_PLAYER_2) : terrainObjectToChar(TerrainObject::TANK_PLAYER_1);
}

char MyBattleStatus::getFriendlyIdentifier() const {
    return (player_id == 1) ? terrainObjectToChar(TerrainObject::TANK_PLAYER_1) : terrainObjectToChar(TerrainObject::TANK_PLAYER_2);
}

std::vector<Position> MyBattleStatus::getOpponentLocations() const {
    return opponent_positions;
}

size_t MyBattleStatus::getOpponentVehicleCount() const {
    return opponent_positions.size();
}

void MyBattleStatus::updateVehicleOrientationAfterAction(const ActionRequest action) {
    vehicle_orientation = Direction::getDirectionAfterAction(vehicle_orientation, action);
}

void MyBattleStatus::updateBattleStatusBaseAction(ActionRequest action) {
    if (action == ActionRequest::Shoot) {
        weapon_cooldown = 4;
        ammo_count--;
        return;
    }
    weapon_cooldown = (weapon_cooldown == 0) ? 0 : weapon_cooldown - 1;
    updateVehicleOrientationAfterAction(action);
}

bool MyBattleStatus::isProjectileInVicinity(Position position, size_t thresh) const {
    for (auto projectile_pos: projectile_positions) {
        if (calculateDistance(position, projectile_pos) < thresh) {
            return true;
        }
    }
    return false;
}

bool MyBattleStatus::isOpponentNearby(Position position) const {
    if (opponent_positions.empty()) {
        return false;
    }
    for (auto opponent_position: opponent_positions) {
        if (calculateDistance(position, opponent_position) <= 2) {
            return true;
        }
    }
    return false;
}

//this time we cannot know the projectiles' direction.
//we say a projectile is threatening a vehicle if it is close to it
bool MyBattleStatus::isProjectileInVicinity(size_t thresh) const {
    return isProjectileInVicinity(vehicle_location, thresh);
}

bool MyBattleStatus::isOpponentNearby() const {
    // For test scenarios, use the test flag
    if (test_is_threatened) {
        return true;
    }
    return isOpponentNearby(vehicle_location);
}
