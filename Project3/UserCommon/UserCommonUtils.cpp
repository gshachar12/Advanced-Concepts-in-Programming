#include "UserCommonUtils.h"
#include <cmath>
#include <queue>
#include <set>

namespace UserCommon_123456789_987654321 {

// Direction utilities
Direction GameUtils::rotateLeft45(Direction dir) {
    return static_cast<Direction>((dir + 7) % 8);
}

Direction GameUtils::rotateRight45(Direction dir) {
    return static_cast<Direction>((dir + 1) % 8);
}

Direction GameUtils::rotateLeft90(Direction dir) {
    return static_cast<Direction>((dir + 6) % 8);
}

Direction GameUtils::rotateRight90(Direction dir) {
    return static_cast<Direction>((dir + 2) % 8);
}

// Position utilities
std::pair<int, int> GameUtils::getNextPosition(int x, int y, Direction dir) {
    const int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    
    return {x + dx[dir], y + dy[dir]};
}

bool GameUtils::isValidPosition(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

double GameUtils::calculateDistance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

bool GameUtils::isWinConditionMet(const std::vector<int>& remaining_tanks) {
    int alive_players = 0;
    for (int tanks : remaining_tanks) {
        if (tanks > 0) alive_players++;
    }
    return alive_players <= 1;
}

std::string GameUtils::directionToString(Direction dir) {
    const std::string names[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    return (dir < 8) ? names[dir] : "?";
}

std::string GameUtils::actionToString(ActionRequest action) {
    switch (action) {
        case ActionRequest::MoveForward: return "Move Forward";
        case ActionRequest::MoveBackward: return "Move Backward";
        case ActionRequest::RotateLeft45: return "Rotate Left 45°";
        case ActionRequest::RotateRight45: return "Rotate Right 45°";
        case ActionRequest::RotateLeft90: return "Rotate Left 90°";
        case ActionRequest::RotateRight90: return "Rotate Right 90°";
        case ActionRequest::Shoot: return "Shoot";
        case ActionRequest::GetBattleInfo: return "Get Battle Info";
        case ActionRequest::DoNothing: return "Do Nothing";
        default: return "Unknown Action";
    }
}

// Pathfinding utilities
std::vector<PathfindingUtils::Point> PathfindingUtils::findSimplePath(
    Point start, Point goal, const std::vector<std::vector<char>>& map) {
    
    std::vector<Point> path;
    
    // Simple direct path for now
    if (start == goal) {
        path.push_back(start);
        return path;
    }
    
    // Basic straight-line pathfinding
    int dx = (goal.x > start.x) ? 1 : (goal.x < start.x) ? -1 : 0;
    int dy = (goal.y > start.y) ? 1 : (goal.y < start.y) ? -1 : 0;
    
    Point current = start;
    path.push_back(current);
    
    while (current.x != goal.x || current.y != goal.y) {
        if (current.x != goal.x) current.x += dx;
        if (current.y != goal.y) current.y += dy;
        path.push_back(current);
        
        // Safety check to prevent infinite loops
        if (path.size() > 100) break;
    }
    
    return path;
}

Direction PathfindingUtils::getDirectionTo(Point from, Point to) {
    int dx = to.x - from.x;
    int dy = to.y - from.y;
    
    if (dx == 0 && dy < 0) return NORTH;
    if (dx > 0 && dy < 0) return NORTHEAST;
    if (dx > 0 && dy == 0) return EAST;
    if (dx > 0 && dy > 0) return SOUTHEAST;
    if (dx == 0 && dy > 0) return SOUTH;
    if (dx < 0 && dy > 0) return SOUTHWEST;
    if (dx < 0 && dy == 0) return WEST;
    if (dx < 0 && dy < 0) return NORTHWEST;
    
    return NORTH; // Default
}

} // namespace UserCommon_123456789_987654321
