#ifndef GAME_RESULT_H
#define GAME_RESULT_H

#include <vector>
#include <cstddef>

/**
 * Structure containing the results of a completed game
 */
struct GameResult {
    /**
     * Winner of the game
     * 0 = tie, 1 = player 1, 2 = player 2
     */
    int winner;
    
    /**
     * Reason the game ended
     */
    enum Reason { 
        ALL_TANKS_DEAD,  // All tanks of one or both players destroyed
        MAX_STEPS,       // Maximum number of steps reached
        ZERO_SHELLS      // All players out of ammunition
    };
    Reason reason;
    
    /**
     * Number of remaining tanks per player
     * Index 0 = player 1, index 1 = player 2
     */
    std::vector<size_t> remaining_tanks;
};

#endif // GAME_RESULT_H
