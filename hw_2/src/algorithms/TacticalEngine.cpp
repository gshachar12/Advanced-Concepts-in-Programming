#include "TacticalEngine.h"

#include <queue>
#include <set>
#include <limits>
#include "Logger.h"
#include "Direction.h"

// Initialize the strategic planning components
void TacticalEngine::initializeStrategies() {
    // Configure the primary planner with standard strategies
    primary_planner = StrategicPlanner({
        "ThreatResponse",
        "DirectEngagement",
        "TargetAcquisition"
    });
    
    // Configure the navigation planner for path execution
    navigation_planner = StrategicPlanner({
        "ObstacleBreach"
    });
}

// Initialize the target position tracking system
void TacticalEngine::initializeTargetTracking() {
    target_position_log = std::vector<Position>(battle_status.getOpponentVehicleCount(), {-1, -1});
}

// Update the database of tracked target positions
void TacticalEngine::updateTargetLog() {
    target_position_log = battle_status.getOpponentLocations();
}

// Check if targets have moved since last update
bool TacticalEngine::haveTargetsRepositioned() const {
    // Get current target positions
    auto current_positions = battle_status.getOpponentLocations();
    
    // Check if the count has changed
    if (current_positions.size() != target_position_log.size()) {
        return true;
    }
    
    // Compare current and logged positions
    return current_positions != target_position_log;
}

// Update the navigation plan based on current battlefield state
void TacticalEngine::updateNavigationPlan() {
    // Determine if recalculation is necessary
    bool targets_moved = haveTargetsRepositioned();
    bool needs_recalculation = movement_plan.empty() || targets_moved || recalculate_route;
    
    if (needs_recalculation) {
        // Determine reason for recalculation
        std::string reason = movement_plan.empty() ? "initial planning" : 
                            (targets_moved ? "target movement" : "route obstruction");
        
        std::string log_message = "Recalculating tactical route: " + reason;
        printLogs(log_message);
        
        // Calculate a new movement plan
        movement_plan = calculateEfficientPath();
        recalculate_route = movement_plan.empty(); // Mark as failed if no path found
        updateTargetLog();
    }
}

// Evaluate movement options and execute navigation
void TacticalEngine::evaluateMovementOptions(ActionRequest *request, std::string *request_title) {
    // Update the navigation plan if needed
    updateNavigationPlan();
    
    // If no valid path exists
    if (movement_plan.empty()) {
        // Try using the obstacle breach strategy first
        if (!navigation_planner.executeStrategies(battle_status, request, request_title)) {
            // If no strategy applies, take a random action
            if (battle_status.isWeaponReady()) {
                *request = ActionRequest::Shoot;
                *request_title = "Tactical area denial";
            } else {
                *request = ActionRequest::RotateLeft45;
                *request_title = "Tactical reorientation";
            }
            recalculate_route = true;
        }
        return;
    }
    
    // Execute the next step in the movement plan
    Direction::DirectionType next_direction = movement_plan.front();
    
    // If already facing the right direction, try to move forward
    if (battle_status.vehicle_orientation == next_direction) {
        // Calculate the destination
        Position destination = battle_status.normalizeCoordinates(
            battle_status.vehicle_location + next_direction);
        
        // Check for mines
        if (battle_status.getTerrainAt(destination) == '@') {
            *request = ActionRequest::DoNothing;
            *request_title = "Mine detected - holding position";
            movement_plan.clear();
            recalculate_route = true;
            return;
        }
        
        // Check for walls
        if (battle_status.getTerrainAt(destination) == '#') {
            if (battle_status.isWeaponReady()) {
                *request = ActionRequest::Shoot;
                *request_title = "Clearing obstacle from path";
                return;
            }
            *request_title = "Obstacle ahead - maintaining position";
            *request = ActionRequest::DoNothing;
            return;
        }
        
        // Path is clear, move forward
        movement_plan.erase(movement_plan.begin());
        recalculate_route = false;
        *request_title = "Advancing to coordinates (" + 
                       std::to_string(destination.x) + "," + 
                       std::to_string(destination.y) + ")";
        *request = ActionRequest::MoveForward;
        return;
    }
    
    // Need to rotate to face the right direction
    recalculate_route = false;
    *request_title = "Aligning from " + 
                   Direction::directionToString(battle_status.vehicle_orientation) + 
                   " toward " + Direction::directionToString(next_direction);
    *request = battle_status.calculateOptimalRotation(next_direction);
}

// Calculate the most efficient path to a firing position
std::vector<Direction::DirectionType> TacticalEngine::calculateEfficientPath() {
    // Define the search node structure
    struct RouteNode {
        // Grid position
        Position location;
        // Sequence of directions to reach this position
        std::vector<Direction::DirectionType> path;
        
        // Constructor
        RouteNode(Position pos, const std::vector<Direction::DirectionType>& p)
            : location(pos), path(p) {}
    };
    
    // Log the start of path calculation
    std::string log_entry = "Beginning route analysis from " + battle_status.vehicle_location.toString();
    printLogs(log_entry);
    
    // Initialize search structures
    std::queue<RouteNode> expansion_queue;
    std::set<Position> visited_locations;
    
    // Start from current position
    expansion_queue.push(RouteNode(battle_status.vehicle_location, {}));
    
    // Track the best path found
    std::vector<Direction::DirectionType> best_path;
    size_t best_path_length = std::numeric_limits<size_t>::max();
    
    // Process the frontier until exhausted
    while (!expansion_queue.empty()) {
        // Get the next node to process
        RouteNode current = expansion_queue.front();
        expansion_queue.pop();
        
        // Skip already visited locations
        if (visited_locations.count(current.location) > 0)
            continue;
            
        // Mark as visited
        visited_locations.insert(current.location);
        
        // Check if this position offers a firing solution
        if (battle_status.canHitTarget(current.location)) {
            // If this is a shorter path than previously found
            if (current.path.size() < best_path_length) {
                best_path = current.path;
                best_path_length = current.path.size();
            }
            // Continue searching for potentially better paths
            continue;
        }
        
        // Explore all possible directions from current position
        for (const auto& direction : battle_status.getSafeDirections(current.location)) {
            // Calculate the resulting position
            Position next_position = battle_status.normalizeCoordinates(current.location + direction);
            
            // Skip if already visited
            if (visited_locations.count(next_position) > 0)
                continue;
                
            // Create the new path
            std::vector<Direction::DirectionType> extended_path = current.path;
            extended_path.push_back(direction);
            
            // Add to search queue
            expansion_queue.push(RouteNode(next_position, extended_path));
        }
    }
    
    // Return the best path found
    return best_path;
}

// Main decision-making method
void TacticalEngine::calculateAction(ActionRequest *request, std::string *request_title) {
    // Initialize with a null action
    *request = ActionRequest::DoNothing;
    
    // Priority 0: First turn or recovering from alert - gather intelligence
    if (battle_status.current_turn == 0 || alert_status) {
        alert_status = false;
        *request = ActionRequest::GetBattleInfo;
        *request_title = "Strategic reconnaissance";
        return;
    }
    
    // Priority 1: Check for immediate threats
    if (isTankThreatened()) {
        alert_status = true;
        
        // Use the primary planner first to handle threats
        if (!primary_planner.executeStrategies(battle_status, request, request_title)) {
            // Fall back to evasive action if no strategy applies
            *request = moveIfThreatened();
            *request_title = "Emergency evasion maneuver";
        }
        return;
    }
    
    // Priority 2: Gather intelligence if needed
    if (!battle_status.hasAmmunition() || battle_status.last_requested_info_turn < battle_status.current_turn) {
        battle_status.last_requested_info_turn = battle_status.current_turn + 1;
        *request = ActionRequest::GetBattleInfo;
        *request_title = "Battlefield analysis";
        return;
    }
    
    // Priority 3: Try direct engagement using strategic planner
    if (primary_planner.executeStrategies(battle_status, request, request_title)) {
        return;
    }
    
    // Priority 4: Navigate to acquire firing position
    evaluateMovementOptions(request, request_title);
}
