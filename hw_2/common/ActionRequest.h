/**
 * @file ActionRequest.h
 * @brief Defines the tank action commands available in the game.
 * @author Advanced Programming Team
 */
#pragma once

/**
 * @enum ActionRequest
 * @brief Represents the different actions a tank can perform during the battle.
 * 
 * This enumeration defines all possible commands that can be issued to a tank
 * including movement, rotation, attack, and information gathering.
 */
enum class ActionRequest {
    // Movement commands
    MoveForward,     ///< Move tank in the forward direction
    MoveBackward,    ///< Move tank in the backward direction
    
    // Rotation commands
    RotateLeft90,    ///< Rotate tank 90 degrees counter-clockwise
    RotateRight90,   ///< Rotate tank 90 degrees clockwise
    RotateLeft45,    ///< Rotate tank 45 degrees counter-clockwise
    RotateRight45,   ///< Rotate tank 45 degrees clockwise
    
    // Combat command
    Shoot,           ///< Fire tank's main gun
    
    // Information commands
    GetBattleInfo,   ///< Request battlefield intelligence
    
    // Default command
    DoNothing        ///< Take no action this turn
};
