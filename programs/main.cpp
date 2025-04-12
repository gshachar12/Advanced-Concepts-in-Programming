#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Tank {

    public:
        // Attributes
        int player_number;
        int cannon_direction;
    
        // Constructor
        Person(string n, int a) {
            name = n;
            age = a;
        }
    
        // Method (a.k.a. member function)
        void sayHello() {
            cout << "Hello, my name is " << name << " and I am " << age << " years old." << endl;
        }
    };


class Player {
public:
    // Attributes (a.k.a. data members)
    Tank tank;
    string name;
    int player_number;

    // Constructor
    Person(string n, int a) {
        name = n;
        age = a;
    }

    // Method (a.k.a. member function)
    void sayHello() {
        cout << "Hello, my name is " << name << " and I am " << age << " years old." << endl;
    }
};


int main(int argc, char* argv[]) {

    if (argc < 2) { // too few arguments
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1]; // get input file
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) { // read input
        cout << line << endl;
    }

    inputFile.close();
    return 0;
}
