#pragma once


/**
 * 
 * - WALL:     '#'
 * - MINE:     '@'
 * - TANK1:    '1'
 * - TANK2:    '2'
 * - EMPTY:    ' '
 * - UNKNOWN:  
 */


enum class CellType {
    EMPTY,
    WALL,
    MINE,
    TANK1,
    TANK2,
    SHELL, 
    UNKNOWN
};