#ifndef ACTION_REQUEST_H
#define ACTION_REQUEST_H

/**
 * Enum representing the different actions a tank can perform
 */
enum class ActionRequest { 
    MoveForward, 
    MoveBackward,
    RotateLeft90, 
    RotateRight90, 
    RotateLeft45, 
    RotateRight45,
    Shoot, 
    GetBattleInfo, 
    DoNothing
};

#endif // ACTION_REQUEST_H
