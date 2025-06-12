#include "AdvancedPathfinder.h"

#include <queue>
#include <set>
#include <limits>
#include "Logger.h"
#include "Direction.h"
#include "StrategicPlanner.h"

// Navigation system state variables
// Flag indicating whether the current navigation route requires recalculation
bool route_validity_flag = false;
// The sequence of directional movements to follow for reaching the target
std::vector<Direction::DirectionType> tactical_movement_route;

// Initialize the system for tracking hostile unit positions
void AdvancedPathfinder::initTargetPositionMonitoring() {
    target_position_history = std::vector<Position>(battle_status.getOpponentVehicleCount(), {-1, -1});
}

// Update the database of tracked hostile unit positions
void AdvancedPathfinder::updateTargetPositions() {
    target_position_history = battle_status.getOpponentLocations();
}

void AdvancedPathfinder::executeThreatResponse(ActionRequest *request, std::string *request_title) {
    // Set the alert flag to request updated battlefield information next turn
    threat_alert = true;
    
    // If there's a clear shot at a threatening target, prioritize neutralizing it
    if (battle_status.canHitTarget(true)) {
        *request = ActionRequest::Shoot;
        *request_title = "Counter-Attack Response";
        return;
    }
    
    // Otherwise execute evasive maneuvers to avoid the threat
    *request = moveIfThreatened();
    *request_title = "Tactical Evasion";
}

void AdvancedPathfinder::attemptTargetAcquisition(ActionRequest *request, std::string *request_title) {
    // If a target is in firing range, engage immediately
    if (battle_status.canHitTarget()) {
        route_validity_flag = false; // Route planning no longer needed as action is taken
        *request = ActionRequest::Shoot;
        *request_title = "Target Engagement";
        return;
    }
    
    // Try to adjust the vehicle's orientation for a firing solution
    adjustAimingVector(request, request_title);
}

bool AdvancedPathfinder::adjustAimingVector(ActionRequest *request, std::string *request_title) const {
    // Check all possible orientations for a firing solution
    for (int dir_index = 0; dir_index < 8; ++dir_index) {
        // Get directional vector from index
        Direction::DirectionType candidate_orientation = Direction::getDirectionFromIndex(dir_index);
        
        // Skip current orientation - no adjustment needed
        if (battle_status.vehicle_orientation == candidate_orientation) {
            continue;
        }
        
        // Check if this orientation provides a firing solution
        if (battle_status.canHitTarget(candidate_orientation)) {
            // Clear any existing navigation plan
            tactical_movement_route.clear();
            route_validity_flag = false;
            
            // Request rotation to the target orientation
            *request = battle_status.calculateOptimalRotation(candidate_orientation);
            *request_title = "Adjusting aim vector to position " + std::to_string(dir_index);
            return true;
        }
    }
    
    // No viable firing orientation found
    return false;
}

bool AdvancedPathfinder::haveTargetsMoved() const {
    // Get current positions of hostile units
    auto current_target_positions = battle_status.getOpponentLocations();
    
    // Check if the number of targets has changed
    if (current_target_positions.size() != target_position_history.size()) {
        return true;
    }
    
    // Check if any target positions have changed
    return current_target_positions != target_position_history;
}

void AdvancedPathfinder::reassessNavigationRoute() {
    // Determine if route recalculation is necessary
    bool targets_repositioned = haveTargetsMoved();
    bool recalculation_required = tactical_movement_route.empty() || targets_repositioned || route_validity_flag;
    
    if (recalculation_required) {
        // Generate an appropriate reason for the log
        std::string recalculation_reason;
        if (tactical_movement_route.empty()) {
            recalculation_reason = "initial route planning";
        } else if (targets_repositioned) {
            recalculation_reason = "target position changes";
        } else {
            recalculation_reason = "previous route blocked";
        }
        
        std::string log_message = "Strategic route recalculation initiated: " + recalculation_reason;
        printLogs(log_message);
        
        // Calculate new optimal route
        tactical_movement_route = calculateOptimalRoute();
        route_validity_flag = tactical_movement_route.empty();
        updateTargetPositions();
    }
}

void AdvancedPathfinder::handleNavigationFailure(ActionRequest *request, std::string *request_title) const {
    // If no route is available but weapon is ready, attempt to clear obstacles or engage
    if (battle_status.isWeaponReady()) {
        *request = ActionRequest::Shoot;
        
        // Determine if we're shooting at a target or obstacle
        bool target_acquisition = battle_status.canHitTarget();
        route_validity_flag = !target_acquisition;
        
        // Set appropriate action description
        *request_title = target_acquisition 
                       ? "Target engaged - continuing mission" 
                       : "Tactical barrier removal";
        return;
    }
    
    // If no viable actions available, reorient to seek better options
    *request_title = "Strategic reorientation";
    *request = ActionRequest::RotateLeft45;
    route_validity_flag = true;
}

void AdvancedPathfinder::executeNavigationStep(ActionRequest *request, std::string *request_title) {
    // Get the next planned movement direction
    Direction::DirectionType planned_direction = tactical_movement_route.front();
    
    // If already oriented correctly, attempt forward movement
    if (battle_status.vehicle_orientation == planned_direction) {
        // Calculate destination coordinates
        Position destination = battle_status.normalizeCoordinates(battle_status.vehicle_location + planned_direction);

        // Safety check for mines
        if (battle_status.getTerrainAt(destination) == '@') {
            *request = ActionRequest::DoNothing;
            *request_title = "Mine detected - maintaining position";
            tactical_movement_route.clear();
            route_validity_flag = true;
            return;
        }

        // Handle walls and barriers
        if (battle_status.getTerrainAt(destination) == '#') {
            if (battle_status.isWeaponReady()) {
                *request = ActionRequest::Shoot;
                *request_title = "Removing barrier";
                route_validity_flag = false;
                return;
            }
            // Cannot clear barrier, hold position
            *request_title = "Barrier ahead - awaiting clearance";
            *request = ActionRequest::DoNothing;
            return;
        }

        // Path is clear, advance and update route plan
        tactical_movement_route.erase(tactical_movement_route.begin());
        route_validity_flag = false;
        *request_title = "Advancing to grid (" + std::to_string(destination.x) + "," + std::to_string(destination.y) + ")";
        *request = ActionRequest::MoveForward;
        return;
    }

    // Need to adjust orientation to match planned direction
    route_validity_flag = false;
    *request_title = "Adjusting orientation from " + Direction::directionToString(battle_status.vehicle_orientation) + 
                     " toward " + Direction::directionToString(planned_direction);
    *request = battle_status.calculateOptimalRotation(planned_direction);
}

std::vector<Direction::DirectionType> AdvancedPathfinder::calculateOptimalRoute() {
    // Define the traversal node structure for pathfinding
    struct TraversalNode {
        // Current grid coordinates
        Position grid_position;
        // Sequence of movements to reach this position
        std::vector<Direction::DirectionType> movement_sequence;

        // Constructor for convenient node creation
        TraversalNode(Position pos, const std::vector<Direction::DirectionType>& seq)
            : grid_position(pos), movement_sequence(seq) {}
    };
    
    // Log the start of the route calculation
    std::string log_entry = "Beginning tactical route analysis from " + battle_status.vehicle_location.toString();
    printLogs(log_entry);

    // Initialize search structures
    std::queue<TraversalNode> expansion_frontier;
    std::set<Position> visited_positions;
    
    // Start pathfinding from current position
    expansion_frontier.push(TraversalNode(battle_status.vehicle_location, {}));
    
    // Track the best route found
    std::vector<Direction::DirectionType> best_route;
    size_t best_route_length = std::numeric_limits<size_t>::max();

    // Process nodes until the frontier is exhausted
    while (!expansion_frontier.empty()) {
        // Get the next position to evaluate
        TraversalNode current = expansion_frontier.front();
        expansion_frontier.pop();
        
        // Skip already visited positions
        if (visited_positions.count(current.grid_position) > 0)
            continue;
            
        // Mark as visited
        visited_positions.insert(current.grid_position);
        
        // Check if this position offers a firing solution
        if (battle_status.canHitTarget(current.grid_position)) {
            // If this route is shorter than the best found so far
            if (current.movement_sequence.size() < best_route_length) {
                best_route = current.movement_sequence;
                best_route_length = current.movement_sequence.size();
            }
            // Continue searching for potentially better routes
            continue;
        }
        
        // Explore all viable directions from current position
        for (const auto& direction : battle_status.getSafeDirections(current.grid_position)) {
            // Calculate the resulting position
            Position next_position = battle_status.normalizeCoordinates(current.grid_position + direction);
            
            // Skip if already visited
            if (visited_positions.count(next_position) > 0)
                continue;
                
            // Create the new movement sequence
            std::vector<Direction::DirectionType> extended_sequence = current.movement_sequence;
            extended_sequence.push_back(direction);
            
            // Add to frontier for further exploration
            expansion_frontier.push(TraversalNode(next_position, extended_sequence));
        }
    }
    
    // Return the shortest route found (may be empty if no route exists)
    return best_route;
}

void AdvancedPathfinder::calculateAction(ActionRequest *request, std::string *request_title) {
    // First turn or recovering from threat - gather battlefield intelligence
    if (battle_status.current_turn == 0 || threat_alert) {
        // Reset threat status after intelligence gathering
        threat_alert = false;
        *request = ActionRequest::GetBattleInfo;
        *request_title = "Battlefield surveillance";
        return;
    }
    
    // Priority 1: Respond to immediate threats using the Strategic Planner
    static StrategicPlanner crisis_planner({
        "ThreatResponse"
    });
    
    if (isTankThreatened()) {
        if (crisis_planner.executeStrategies(battle_status, request, request_title)) {
            return;
        }
        // Fall back to legacy response if the strategy didn't handle it
        executeThreatResponse(request, request_title);
        return;
    }
    
    // Priority 2: Update battlefield information when necessary
    if (!battle_status.hasAmmunition() || (battle_status.last_requested_info_turn < battle_status.current_turn)) {
        battle_status.last_requested_info_turn = battle_status.current_turn + 1;
        *request = ActionRequest::GetBattleInfo;
        *request_title = "Tactical intelligence update";
        return;
    }
    
    // Priority 3: Engage targets of opportunity using the Strategic Planner
    static StrategicPlanner engagement_planner({
        "DirectEngagement", 
        "TargetAcquisition"
    });
    
    if (engagement_planner.executeStrategies(battle_status, request, request_title)) {
        return;
    }
    
    // Priority 4: Execute movement plan if no immediate action taken
    if (*request == ActionRequest::DoNothing) {
        // Evaluate and update movement plan
        reassessNavigationRoute();
        
        // Execute navigation or handle navigation failures
        if (tactical_movement_route.empty()) {
            // Try the obstacle breach strategy first
            static StrategicPlanner obstacle_planner({
                "ObstacleBreach"
            });
            
            if (!obstacle_planner.executeStrategies(battle_status, request, request_title)) {
                // Fall back to legacy handling if strategy didn't handle it
                handleNavigationFailure(request, request_title);
            }
        } else {
            executeNavigationStep(request, request_title);
        }
    }
}
