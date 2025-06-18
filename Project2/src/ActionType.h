#ifndef ACTION_TYPE_H
#define ACTION_TYPE_H

#include <string>
#include "common/ActionRequest.h"

namespace ActionType {

// Convert ActionRequest enum to string representation for output
inline std::string toString(ActionRequest action) {
    switch (action) {
        case ActionRequest::MoveForward:  return "MoveForward";
        case ActionRequest::MoveBackward: return "MoveBackward";
        case ActionRequest::RotateLeft90: return "RotateLeft90";
        case ActionRequest::RotateRight90: return "RotateRight90";
        case ActionRequest::RotateLeft45: return "RotateLeft45";
        case ActionRequest::RotateRight45: return "RotateRight45";
        case ActionRequest::Shoot:        return "Shoot";
        case ActionRequest::GetBattleInfo: return "GetBattleInfo";
        case ActionRequest::DoNothing:    return "DoNothing";
        default:                          return "Unknown";
    }
}

// Convert string to ActionRequest enum
inline ActionRequest fromString(const std::string& actionStr) {
    if (actionStr == "MoveForward")  return ActionRequest::MoveForward;
    if (actionStr == "MoveBackward") return ActionRequest::MoveBackward;
    if (actionStr == "RotateLeft90") return ActionRequest::RotateLeft90;
    if (actionStr == "RotateRight90") return ActionRequest::RotateRight90;
    if (actionStr == "RotateLeft45") return ActionRequest::RotateLeft45;
    if (actionStr == "RotateRight45") return ActionRequest::RotateRight45;
    if (actionStr == "Shoot")        return ActionRequest::Shoot;
    if (actionStr == "GetBattleInfo") return ActionRequest::GetBattleInfo;
    if (actionStr == "DoNothing")    return ActionRequest::DoNothing;
    
    // Default to DoNothing for unrecognized actions
    return ActionRequest::DoNothing;
}

// Check if action is a movement action
inline bool isMovement(ActionRequest action) {
    return action == ActionRequest::MoveForward || 
           action == ActionRequest::MoveBackward;
}

// Check if action is a rotation action
inline bool isRotation(ActionRequest action) {
    return action == ActionRequest::RotateLeft90 || 
           action == ActionRequest::RotateRight90 ||
           action == ActionRequest::RotateLeft45 || 
           action == ActionRequest::RotateRight45;
}

} // namespace ActionType

#endif // ACTION_TYPE_H