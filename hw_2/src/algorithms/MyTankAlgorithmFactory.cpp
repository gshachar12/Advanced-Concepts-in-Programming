#include "MyTankAlgorithmFactory.h"

#include "BfsAlgorithm.h"
#include "SimpleAlgorithm.h"

unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(int player_index, int tank_index) const {
    if (player_index % 2 == 0) {
        return std::make_unique<SimpleAlgorithm>(player_index, tank_index);
    }
    return std::make_unique<BfsAlgorithm>(player_index, tank_index);
}
