#include "BfsAlgorithm.h"

#include <queue>
#include <set>
#include <limits>
#include "Logger.h"
#include "Direction.h"

// Navigation state variables
// Flag to track if the current path is viable or needs recalculation
bool path_viability_flag = false;
// Store the sequence of directions for the tank to follow
std::vector<Direction::DirectionType> navigation_path;

// Initialize tracking of enemy positions with default values
void BfsAlgorithm::initEnemyPositionTracking() {
    enemy_position_history = std::vector<Position>(battle_status.getOpponentVehicleCount(), {-1, -1});
}

// Update the stored enemy positions with current battlefield data
void BfsAlgorithm::refreshEnemyPositions() {
    enemy_position_history = battle_status.getOpponentLocations();
}


void BfsAlgorithm::respondToThreat(ActionRequest *request, std::string *request_title) {
    // Set the danger flag to request battlefield info in next turn
    danger_detected = true;
    
    // If we have a direct shot at an enemy, prioritize offensive action
    if (battle_status.canHitTarget(true)) {
        *request = ActionRequest::Shoot;
        *request_title = "Defensive Fire";
        return;
    }
    
    // Otherwise, take evasive action
    *request = moveIfThreatened();
    *request_title = "Evasive Maneuver";
}

void BfsAlgorithm::attemptToEngageEnemy(ActionRequest *request, std::string *request_title) {
    // If we have a clean shot, take it immediately
    if (battle_status.canHitTarget()) {
        path_viability_flag = false; // Reset path flag as we're taking action
        *request = ActionRequest::Shoot;
        *request_title = "Engaging Target";
        return;
    }
    
    // Otherwise try to rotate to get a shot
    turnTowardsEnemy(request, request_title);
}

bool BfsAlgorithm::turnTowardsEnemy(ActionRequest *request, std::string *request_title) const {
    // Check all 8 possible directions for a shot opportunity
    for (int direction_id = 0; direction_id < 8; ++direction_id) {
        // Get the direction from the index (0-7)
        Direction::DirectionType target_direction = Direction::getDirectionFromIndex(direction_id);
        
        // Skip our current direction - no need to rotate
        if (battle_status.vehicle_orientation == target_direction) {
            continue;
        }
        
        // Check if we can shoot an enemy from this direction
        if (battle_status.canHitTarget(target_direction)) {
            // Clear any existing path as we're changing strategy
            navigation_path.clear();
            path_viability_flag = false;
            
            // Request rotation toward the target direction
            *request = battle_status.calculateOptimalRotation(target_direction);
            *request_title = "Rotating to firing position " + std::to_string(direction_id);
            return true;
        }
    }
    
    // No suitable rotation found
    return false;
}

bool BfsAlgorithm::haveEnemiesMoved() const {
    // Get the current positions of all enemies
    auto current_enemy_positions = battle_status.getOpponentLocations();
    
    // If the count of enemies has changed, consider it a movement
    if (current_enemy_positions.size() != enemy_position_history.size()) {
        return true;
    }
    
    // Otherwise check if any positions have changed
    return current_enemy_positions != enemy_position_history;
}

void BfsAlgorithm::updateNavigationPath() {
    // Check if we need to recalculate our path
    bool enemies_repositioned = haveEnemiesMoved();
    bool recalculation_needed = navigation_path.empty() || enemies_repositioned || path_viability_flag;
    
    if (recalculation_needed) {
        // Determine and log the reason for recalculation
        std::string recalculation_reason;
        if (navigation_path.empty()) {
            recalculation_reason = "no existing path";
        } else if (enemies_repositioned) {
            recalculation_reason = "battlefield changes detected";
        } else {
            recalculation_reason = "previous path navigation failed";
        }
        
        std::string log_message = "Performing tactical recalculation: " + recalculation_reason;
        printLogs(log_message);
        
        // Calculate new path and update state
        navigation_path = findPathWithBFS();
        path_viability_flag = navigation_path.empty();
        refreshEnemyPositions();
    }
}

void BfsAlgorithm::handleNoValidPath(ActionRequest *request, std::string *request_title) const {
    // If no path is available but we can shoot, take the shot
    if (battle_status.isWeaponReady()) {
        *request = ActionRequest::Shoot;
        
        // Track whether this is targeted or random fire
        bool can_hit_enemy = battle_status.canHitTarget();
        path_viability_flag = !can_hit_enemy;
        
        // Set appropriate message
        *request_title = can_hit_enemy 
                       ? "Direct fire - no navigation needed" 
                       : "Suppressive fire - clearing obstacles";
        return;
    }
    
    // If we can't shoot, try rotating to find a better position
    *request_title = "Tactical reorientation";
    *request = ActionRequest::RotateLeft45;
    path_viability_flag = true;
}


void BfsAlgorithm::followCurrentPath(ActionRequest *request, std::string *request_title) {
    // Get the next direction from our path
    Direction::DirectionType desired_direction = navigation_path.front();
    
    // If we're already facing the right way, try to move forward
    if (battle_status.vehicle_orientation == desired_direction) {
        // Calculate the next position
        Position destination = battle_status.normalizeCoordinates(battle_status.vehicle_location + desired_direction);

        // Check for mines - don't move if there's a mine
        if (battle_status.getTerrainAt(destination) == '@') {
            *request = ActionRequest::DoNothing;
            *request_title = "Mine detected - holding position";
            navigation_path.clear();
            path_viability_flag = true;
            return;
        }

        // Check for walls - shoot if possible
        if (battle_status.getTerrainAt(destination) == '#') {
            if (battle_status.isWeaponReady()) {
                *request = ActionRequest::Shoot;
                *request_title = "Clearing obstacle";
                path_viability_flag = false;
                return;
            }
            // Can't shoot the wall, so wait
            *request_title = "Obstacle ahead - awaiting clearance";
            *request = ActionRequest::DoNothing;
            return;
        }

        // Path is clear, move forward and update path
        navigation_path.erase(navigation_path.begin());
        path_viability_flag = false;
        *request_title = "Advancing to coordinates (" + std::to_string(destination.x) + "," + std::to_string(destination.y) + ")";
        *request = ActionRequest::MoveForward;
        return;
    }

    // We need to rotate to face the right direction
    path_viability_flag = false;
    *request_title = "Aligning from " + Direction::directionToString(battle_status.vehicle_orientation) + 
                     " toward " + Direction::directionToString(desired_direction);
    *request = battle_status.calculateOptimalRotation(desired_direction);
}


std::vector<Direction::DirectionType> BfsAlgorithm::findPathWithBFS() {
    // Define the search node structure for the BFS algorithm
    struct SearchNode {
        // Current position in the battlefield
        Position location;
        // Sequence of directions to reach this position
        std::vector<Direction::DirectionType> route;

        // Constructor for easy node creation
        SearchNode(Position loc, const std::vector<Direction::DirectionType>& r)
            : location(loc), route(r) {}
    };
    
    // Log the start of path calculation
    std::string log_entry = "Tactical navigation analysis from " + battle_status.vehicle_location.toString();
    printLogs(log_entry);

    // Initialize the search structures
    std::queue<SearchNode> frontier;
    std::set<Position> explored_positions;
    
    // Add starting position to the search frontier
    frontier.push(SearchNode(battle_status.vehicle_location, {}));
    
    // Track the optimal path found so far
    std::vector<Direction::DirectionType> optimal_route;
    size_t optimal_distance = std::numeric_limits<size_t>::max();

    // Process the search frontier until exhausted
    while (!frontier.empty()) {
        // Get the next position to explore
        SearchNode current = frontier.front();
        frontier.pop();
        
        // Skip already explored positions
        if (explored_positions.count(current.location) > 0)
            continue;
            
        // Mark position as explored
        explored_positions.insert(current.location);
        
        // Check if this position gives us a firing solution
        if (battle_status.canHitTarget(current.location)) {
            // If this is a shorter path than previously found
            if (current.route.size() < optimal_distance) {
                optimal_route = current.route;
                optimal_distance = current.route.size();
            }
            // Continue searching for potentially better paths
            continue;
        }
        
        // Explore safe directions from current position
        for (const auto& direction : battle_status.getSafeDirections(current.location)) {
            // Calculate the next position
            Position next_location = battle_status.normalizeCoordinates(current.location + direction);
            
            // Skip if already explored
            if (explored_positions.count(next_location) > 0)
                continue;
                
            // Create the new path by extending current path
            std::vector<Direction::DirectionType> extended_route = current.route;
            extended_route.push_back(direction);
            
            // Add to search frontier
            frontier.push(SearchNode(next_location, extended_route));
        }
    }
    
    // Return the optimal path found (may be empty if no path exists)
    return optimal_route;
}

void BfsAlgorithm::calculateAction(ActionRequest *request, std::string *request_title) {
    // Initial turn or following a danger state - gather intelligence
    if (battle_status.current_turn == 0 || danger_detected) {
        // Reset danger flag after gathering intel
        danger_detected = false;
        *request = ActionRequest::GetBattleInfo;
        *request_title = "Intelligence gathering";
        return;
    }
    
    // Priority 1: Check for immediate threats and respond
    if (isTankThreatened()) {
        respondToThreat(request, request_title);
        return;
    }
    
    // Priority 2: Gather intelligence if ammunition is low or info is outdated
    if (!battle_status.hasAmmunition() || (battle_status.last_requested_info_turn < battle_status.current_turn)) {
        battle_status.last_requested_info_turn = battle_status.current_turn + 1;
        *request = ActionRequest::GetBattleInfo;
        *request_title = "Battlefield reconnaissance";
        return;
    }
    
    // Priority 3: Attack if enemy is in sight
    attemptToEngageEnemy(request, request_title);
    
    // Priority 4: Navigate towards enemy if no immediate action was taken
    if (*request == ActionRequest::DoNothing) {
        // Update navigation path if needed
        updateNavigationPath();
        
        // Either follow the path or handle situation with no path
        if (navigation_path.empty()) {
            handleNoValidPath(request, request_title);
        } else {
            followCurrentPath(request, request_title);
        }
    }
}
