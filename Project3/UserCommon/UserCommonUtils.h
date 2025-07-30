#ifndef USER_COMMON_UTILS_H
#define USER_COMMON_UTILS_H

#include "UserCommonTypes.h"
#include <vector>
#include <string>

namespace UserCommon_123456789_987654321 {

/**
 * Utility functions that can be shared between algorithms and game managers
 */
class GameUtils {
public:
    // Direction utilities
    static Direction rotateLeft45(Direction dir);
    static Direction rotateRight45(Direction dir);
    static Direction rotateLeft90(Direction dir);
    static Direction rotateRight90(Direction dir);
    
    // Position utilities
    static std::pair<int, int> getNextPosition(int x, int y, Direction dir);
    static bool isValidPosition(int x, int y, int width, int height);
    static double calculateDistance(int x1, int y1, int x2, int y2);
    
    // Game state utilities
    static bool isWinConditionMet(const std::vector<int>& remaining_tanks);
    static std::string directionToString(Direction dir);
    static std::string actionToString(ActionRequest action);
};

/**
 * Simple pathfinding utilities
 */
class PathfindingUtils {
public:
    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    };
    
    static std::vector<Point> findSimplePath(Point start, Point goal, 
                                           const std::vector<std::vector<char>>& map);
    static Direction getDirectionTo(Point from, Point to);
};

} // namespace UserCommon_123456789_987654321

#endif // USER_COMMON_UTILS_H
