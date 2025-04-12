#ifndef TANK_GAME_H
#define TANK_GAME_H

#include <vector>

// Enum for the direction of the tank's cannon
enum Cannon_Direction {
    Up,
    Up_Right,
    Right,
    Down_Right, 
    Down,
    Down_Left,
    Left,
    Up_Left
}; 

// Enum for possible tank actions
enum Tank_Action {
    move_forward,
    move_backward,
    rotate_eighth_left,
    rotate_eighth_right,
    rotate_quarter_left,
    rotate_quarter_right, 
    shoot,
    move_shell
};

// Function declaration for displaying the map
void displayMap(const std::vector<std::vector<char>>& map);

#endif // TANK_GAME_H
