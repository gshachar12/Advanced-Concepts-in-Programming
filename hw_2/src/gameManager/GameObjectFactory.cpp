#include "GameObjectFactory.h"
#include "Wall.h"
#include "WeakWall.h"
#include "Mine.h"
#include "Tank.h"

int GameObjectFactory::tank_algo_count = 0;

std::unique_ptr<GameObject> GameObjectFactory::create(const char symbol, const Position position,
                                                      const size_t num_shells) {
    switch (symbol) {
        case '#': return std::make_unique<Wall>(position);
        case 'W':
        case '=': return std::make_unique<WeakWall>(position);
        case '@': return std::make_unique<Mine>(position);
        case '1': return std::make_unique<Tank>(position, 1, tank_algo_count++, num_shells);
        case '2': return std::make_unique<Tank>(position, 2, tank_algo_count++, num_shells);
        default: return nullptr;
    }
}

