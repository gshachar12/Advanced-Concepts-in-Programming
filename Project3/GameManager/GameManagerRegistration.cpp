#include "MyGameManager_Fixed.h"
#include "../common/AbstractGameManager.h"
#include <memory>

using namespace GameManager_123456789_987654321;

// Entry point for the shared library
extern "C" {
    AbstractGameManager* createGameManager(bool verbose) {
        return new MyGameManager(verbose);
    }
    
    void destroyGameManager(AbstractGameManager* gm) {
        delete gm;
    }
}
