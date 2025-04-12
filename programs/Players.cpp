#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "header_files/utilities.h"
using namespace std;

#define INITIAL_SHELL_COUNT 16 
#define BACKWARD_MOVEMENT_STALL 3

class GameObject {
    protected:
        std::vector<int> position; // [x, y]
        std::string direction;     // "U", "D", "L", etc.
    
    public:
        // Constructor
        GameObject(int x, int y, const std::string& dir)
            : position({x, y}), direction(dir) {}
    
        // Getters
        std::vector<int> getPosition() const {
            return position;
        }
    
        std::string getDirection() const {
            return direction;
        }
    
        
        virtual void move() = 0;
    
        // virtual destructor
        virtual ~GameObject() {}
    };
    



class Shell: public GameObject {
    std::vector<int> position; // position[0] = x, position[1] = y
    std::string direction;
    public:
        Shell(int x, int y,  )
        : GameObject(x, y, dir) {}


        void hit_wall()
        {

            
        }

        // Print
        void printStatus() const {
            std::cout << "Shell at (" << position[0] << ", " << position[1] 
                    << ") moving " << direction << std::endl;
        }
};

class Tank: public GameObject {
    private:
        std::vector<int> position; // position[0] = x, position[1] = y

    public:

        // Attributes
        Cannon_Direction direction;
        vector<int> artilery_shells; 
        int shell_count; 

        // Constructor
        Tank(Cannon_Direction d, int startX, int startY) {
            direction = d;
            shell_count = INITIAL_SHELL_COUNT; 
            position.push_back(startX);
            position.push_back(startY);
        }
        
        // Print current position
        void printPosition() const {
            std::cout << "Tank is at (" << position[0] << ", " << position[1] << ")" << std::endl;
        }

        // Move to a new position
        void moveTo(int newX, int newY) {
            position[0] = newX;
            position[1] = newY;
        }

        void move() override {
            switch (direction) {
                case Cannon_Direction::U:  position[1] -= 1; break;
                case Cannon_Direction::D:  position[1] += 1; break;
                case Cannon_Direction::L:  position[0] -= 1; break;
                case Cannon_Direction::R:  position[0] += 1; break;
                case Cannon_Direction::UR: position[0] += 1; position[1] -= 1; break;
                case Cannon_Direction::UL: position[0] -= 1; position[1] -= 1; break;
                case Cannon_Direction::DR: position[0] += 1; position[1] += 1; break;
                case Cannon_Direction::DL: position[0] -= 1; position[1] += 1; break;
            }
        }

        // Get the position vector
        std::vector<int> getPosition() const {
            return position;
        }

        void move(){

        }
        void shoot() {
        
        }
        
        // Print
        void printStatus() const {
            std::cout << "Tank at (" << position[0] << ", " << position[1] 
                    << ") facing " << direction << std::endl;
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


    void chase(){
        
    }

    void MovePlayer()
    {

    }
};
