#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "header_files/utilities.h"
using namespace std;

#define INITIAL_SHELL_COUNT 16 
#define BACKWARD_MOVEMENT_STALL 3


class Shell {
 
    public:
        Shell()
        {

        }


        void hit_wall()
        {

            
        }
};

class Tank {
    public:

        // Attributes
        int cannon_direction;
        vector<int> artilery_shells; 
        int shell_count; 

        // Constructor
        Tank(Cannon_Direction d) {
            cannon_direction = d;
            shell_count = INITIAL_SHELL_COUNT; 
        }
       
        void move(){

        }
        void shoot() {
        }

    };


class Player {
public:
    // Attributes (a.k.a. data members)
    Tank tank;
    int score; 
    int player_number;
    int last_shooting_step; // the last step in which the tank shot 
    int last_backward_step; 
    // Constructor
    Player(Cannon_Direction starting_direction, int player_number) 
    {


    }

    void applyAction(Tank_Action action, int current_step){

        switch(action) {

            case move_forward:
                cout << "Moving Forward" << endl;
            case move_backward:
                cout << "Moving Backword" << endl;       
                if (current_step-last_shooting_step == BACKWARD_MOVEMENT_STALL ) // move only after 2 idle steps
                    tank.move();

            case rotate_eighth_left:
                cout << "rotate_eighth_left" << endl;           

            case rotate_eighth_right:
            cout << "rotate_eighth_left" << endl;           

            case rotate_quarter_right: 
            cout << "rotate_eighth_right" << endl;           

            case shoot:
                cout << "shoot" << endl;           

            case move_shell:
                cout << "move shell" << endl;           



        }

    }


    // Method (a.k.a. member function)
    void sayHello() {
    }


    void chase(){
        
    }

    void MovePlayer()
    {

    }
};
