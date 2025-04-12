#include <iostream>
#include <vector>
#include "header_files/players.h"
using namespace std;
#define INITIAL_REMAINING_STEPS 40
/*

Each tank has 16 artillery shells when the game starts. There is no recharge option.
If both tanks used all their artillery the game continues for an additional 40 steps, if no one wins by then, the game ends with a tie.

The boundaries of the game board are considered as an invisible tunnel to the other side 
of the board (from top to bottom and vice versa and from left to right and vice versa).
This applies for both the tanks and the shells (i.e. both tanks and shells may continue their movement to the other side through the invisible tunnel).

When a shell hits a wall, the wall is weakened, so after two hits the wall will fall (disappear from the game board). 
Shells cannot hit mines. If a shell hits another shell, both explode.
If a shell hits a tank, the tank is destroyed. If a tank steps on a mine both are destroyed.
If a tank hits another tank   (both or one drive on the other) they are both destroyed. 
In the first assignment, as there is only one tank per player, if a tank is destroyed the other player wins. 
If both tanks are destroyed at the same time, it’s a tie.
See more on the above in moodle: here (move order) and here (multiple objects collision).

*/ 
class GameManager {
    // mine
    // players
    int remaining_steps; 
    Player player1; 
    Player player2;
    
    GameManager (Direction starting_direction, int player_number) 
    {
        remaining_steps = INITIAL_REMAINING_STEPS; 
        player1 = Player(Left, 1);
        player2 = Player(Right, 2 ); 


    }

    void check_if_legal()
    {
        

    }
    void start_game()
    {
    

    }

    void update_game()
    {

    }

    void display_game()
    {

    }
    Player declare_winner()
    {

    }

}

class action {




}