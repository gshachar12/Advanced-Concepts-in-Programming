#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include <memory>

#include "GameObject.h"


class GameObjectFactory {
    static int tank_algo_count;

public:
    static std::unique_ptr<GameObject> create(char symbol, Position position, size_t shells_count);
};


#endif //GAMEOBJECTFACTORY_H
