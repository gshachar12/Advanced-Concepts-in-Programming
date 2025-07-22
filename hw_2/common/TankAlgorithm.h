/**
 * @file TankAlgorithm.h
 * @brief Defines the interface for tank decision-making algorithms.
 * @author Advanced Programming Team
 */
#pragma once

#include <memory>
#include "ActionRequest.h"
#include "BattleInfo.h"

// Use std namespace explicitly for clarity
using std::unique_ptr;

/**
 * @class TankAlgorithm
 * @brief Interface for tank AI decision-making logic
 *
 * This abstract class defines how a tank's AI should make decisions
 * based on battlefield information and produce action commands.
 */
class TankAlgorithm {
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     */
    virtual ~TankAlgorithm() = default;

    /**
     * @brief Determines the next action for the tank to perform
     * 
     * @return ActionRequest The command to execute next
     */
    virtual ActionRequest getAction() = 0;

    /**
     * @brief Updates the algorithm with current battlefield information
     * 
     * @param battleInfo Current state of the battlefield
     */
    virtual void updateBattleInfo(BattleInfo &battleInfo) = 0;
};
